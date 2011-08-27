/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

/* ---------------------------------------------- *\
   file: Parser.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#include "Debugger.H"
#include "../gui/Utilities.H"
#include "../gui/SyscallHandler.H"
#include "../gui/Program.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "Statement.H"
#include "State.H"
#include <QMutexLocker>

Debugger::Debugger(SyscallListener *listener, ParseList* parseList) 
   : m_state(new State(listener, this)), m_parseList(parseList), m_status(STOPPED), 
     m_terminationReason(T_ABNORMAL), m_memoryWatchpoints(NULL), 
     m_forceStopped(false), m_syscallListener(listener)
{
   memset(m_registerWatchpoints, 0, sizeof(bool) * register_count);
   connect(this, SIGNAL(finished()), this, SLOT(threadTerminated()));
   
   m_time.start();
   // cannot use signals/slots for watchpoints; has to be direct!
//   connect(m_state, SIGNAL(registerChanged(unsigned int, unsigned int, ParseNode*)), this, SLOT(registerChanged(unsigned int, unsigned int, ParseNode*)));
}

int Debugger::getStatusWithoutLocking() const {
   return m_status; // necessary in the Gui a few places to guard against deadlocks
}

int Debugger::getStatus(void) {
   // locks the mutex, automatically unlocked when destructed
   QMutexLocker locker(&m_statusMutex);

   return m_status;
}

State *Debugger::getState() {
   return m_state;
}

QString Debugger::getDuration() const {
   return m_duration;
}

void Debugger::threadTerminated(void) {
   const unsigned long elapsed = m_time.elapsed();
   const QString &durString  = QString::number(elapsed / 1000); // seconds
   const QString &durString2 = QString::number(elapsed % 1000); // millisecs
   const TIMESTAMP cycles = m_state->getCurrentTimestamp();
   
      //.toString("h:mm:ss.zzz ");
   m_duration = QString("Executed in %1 cycle%2 (%3.%4s)").arg(QString::number(cycles), (cycles > 1 ? QString("s") : QString()), durString, durString2);
   
   if (VERBOSE) cerr << "Debugger::threadTerminated\n";
   
   emit programStatusChanged(STOPPED);
   if (m_terminationReason != T_COMPLETED && m_terminationReason != T_TERMINATED && !m_forceStopped && m_terminationReason != T_UNCAUGHT_EXCEPTION)
      m_terminationReason = T_ABNORMAL;
   
   if (m_forceStopped)
      m_terminationReason = T_FORCE_ROLLBACK;
   
   emit programTerminated(m_terminationReason);
}

void Debugger::programStop(void) {
   setStatusConditionally(STOPPED);
}

void Debugger::programForceStop(void) {
   m_forceStopped = true;
   
   m_terminationReason = T_FORCE_ROLLBACK;
   programStop();
}

void Debugger::programPause(void) {
   setStatusConditionally(PAUSED);
}

void Debugger::programRun(void) {
   if (VERBOSE) cerr << "Debugger::programRun()\n";

   if (getStatus() != RUNNING) {
      setStatus(RUNNING);
      start();
   }
}

// private
void Debugger::runAnotherStep(void) {
   // Check if we're at the end of the program
   if (checkProgramCompleted())
      return;
      
   try {
      // execute another parsenode
//      cerr << m_state->getPC() << ",  " << m_state->getPC()->getAddress() << endl;

      m_state->getPC()->execute(m_state, m_parseList, m_status);
   } catch (const ExitSyscallHandlerCauseIAMTooLazyToWriteThisProperly& e) { // FIXME: this is a hack
      // syscall 10 -> exit
      m_terminationReason = T_COMPLETED;
      setStatus(STOPPED);
   } catch (const StateException& e) {
      m_exception = e; // propogate exception up to gui

      notifyPause(e);
      setStatus(PAUSED);
      std::cerr << "EXCEPTION:  " << e.toStdString() << endl << endl;
   }
   
   // We run this below as well, so we stop the program immediately
   //if (checkProgramCompleted())
   //   return;
   if (m_state->getPC() == NULL) {
      setStatus(STOPPED);
      if (VERBOSE) cerr << "<<< Program COMPLETED (2): " << (void*)m_state->getPC() << endl;
      m_terminationReason = T_COMPLETED;
      return;
   }
}

bool Debugger::checkProgramCompleted() {
   ParseNode *pc = m_state->getPC();
   bool programCompleted = (pc == NULL);

   if (!programCompleted) {
      // see if the current instruction is a Done instruction
      if (dynamic_cast<Done*>(pc->getStatement()))
         programCompleted = true;
   }
   
   if (programCompleted) {
      setStatus(STOPPED);
      m_terminationReason = T_COMPLETED;
      if (VERBOSE) cerr << "<<< Program COMPLETED (1): " << (void*)pc << endl;
      return true;
   }
   
   return false;
}

bool Debugger::waitOnDebuggerThread(unsigned long timeout) {
   return wait(timeout);  
}

//private
void Debugger::executionInit(void) {
   if (VERBOSE) cerr << "Initializing Debugger\n";
   m_forceStopped = false;
   
   if (!m_parseList->isValid()) {
      cerr << "Invalid program. Sorry for this lame error -- check the GUI for hints on what's wrong.\n";
      m_terminationReason = T_INVALID_PROGRAM;
      return;
   }
   
   if (!m_parseList->initialize(m_state)) {
      cerr << "Debugger: parseList couldn't be initialized.\n";
      m_terminationReason = T_INVALID_PROGRAM;
      return; 
   }
   
   // start the program timer!
   m_time.restart();
}

// we remain PAUSED the whole time
void Debugger::programStepForward(void) {
   
   switch (getStatus()) {
      case STOPPED:
         executionInit();
         setStatus(PAUSED);
      case PAUSED:            // fall through
         runAnotherStep(); 
   }
}

void Debugger::programStepBackward() {
   m_state->undoLastInstruction();
}

void Debugger::programStepBackwardToTimestamp(TIMESTAMP stamp) {
   m_state->undoUntilTimestamp(stamp);
}

void Debugger::programStepForward(int noInstructions) {
   if (noInstructions <= 0)
      return;
   
   if (m_status == STOPPED)
      executionInit();
   
   //cerr << "Forward " << noInstructions << " instrs; pc = '" << m_state->getPC() << "'\n";

//   setStatus(RUNNING); // do Not put this here; causes weird glib errors..
   // don't want the Gui to update during a long run of instructions
   //    (causes it to hang and be very unresponsive)
   // and the gui won't update if it thinks the program is 'running'
   m_status = RUNNING;
   
   // remain paused the entire time
   while(noInstructions-- && m_status == RUNNING) {
      if (m_state->getPC() == NULL) {
         setStatus(STOPPED);
         m_terminationReason = T_COMPLETED;
         return;
      }

      //cerr << "Mid " << noInstructions << " instrs; pc = '" << m_state->getPC() << "'\n";
      runAnotherStep();
   }

   //cerr << "End " << noInstructions << " instrs; pc = '" << m_state->getPC() << "'\n";
   
   if (m_status == RUNNING) {
      setStatus(PAUSED);
   }
}

void Debugger::programStepBackward(int noInstructions) {
   if (noInstructions <= 0)
      return;
   
   // don't want the Gui to update during a long run of instructions
   //    (causes it to hang and be very unresponsive)
   // and the gui won't update if it thinks the program is 'running'
   m_status = RUNNING;
   while(noInstructions-- && m_state->undoIsAvailable())
      m_state->undoLastInstruction();

   setStatus(PAUSED);
}

void Debugger::setParseList(ParseList *list) {
   QMutexLocker locker(&m_statusMutex);
   m_parseList = list;
}

ParseList *Debugger::getParseList() const {
   return m_parseList;
}

void Debugger::run(void) {
   
   executionInit();
   
   // let's run!
   if (VERBOSE) cerr << "Debugger::run: now executing the program.\n";
   
   m_terminationReason = T_TERMINATED;
   while (getStatus() != STOPPED) {
      waitOnStatus(PAUSED);
      if (getStatus() == STOPPED)
         break; // check if waitOnStatus was interrupted
      
      // Check if we're at the end of the program
      if (m_state->getPC() == NULL) {
         setStatus(STOPPED);
         m_terminationReason = T_COMPLETED;
         break;
      }
      if (VERBOSE) cerr << "\tExecuting: " << m_state->getPC() << endl;

      // Check for a breakpoint
      if (m_state->getPC()->containsBreakPoint()) {
         if (VERBOSE) cerr << ">>> BREAKPOINT\n";

         //m_pauseReason = QString("Execution paused:  Breakpoint encountered");
         setStatus(PAUSED);
         Program *program = m_parseList->getProgram();
         notifyPause(QString("Execution Paused:  Breakpoint encountered on line %1").arg(program == NULL ? "" : QString::number(1 + program->lineNumber(m_state->getPC()))));
         waitOnStatus(PAUSED);
         if (getStatus() == STOPPED)
            break; // check if waitOnStatus was interrupted
//         continue;
      }
      // execute another parsenode  
      runAnotherStep();
   }
   
   if (VERBOSE) cerr << "Debugger::end of run(); terminationReason = " << m_terminationReason << endl;
}

void Debugger::setStatus(int status) {
   QMutexLocker locker(&m_statusMutex);
   
   m_status = status;
   if (VERBOSE) cerr << "Debugger emitting status: " << 
               (status == STOPPED ? "stopped" : 
               (status == PAUSED ? "paused" : "running")) << endl;
   emit programStatusChanged(status);
   
   m_statusChangedWaitCondition.wakeAll();
}

void Debugger::setStatusConditionally(int status) {
    QMutexLocker locker(&m_statusMutex);
    
    if (m_status != status) {
       m_status = status;
       if (VERBOSE) cerr << "Debugger::emitting status: " << status << endl;
       emit programStatusChanged(status);
    }
    
    m_statusChangedWaitCondition.wakeAll();
}

int Debugger::waitOnStatus(int status) {
//   cerr << "\tentering wait\n";
   int st;
   
   m_statusMutex.lock();
   while (m_status == status)
      m_statusChangedWaitCondition.wait(&m_statusMutex);

   st = m_status;
   m_statusMutex.unlock();
   
//   cerr << "\texiting wait\n";
   return st;
}

void Debugger::setMemoryWatchpoints(QHash<unsigned int, bool> *w) {
   m_memoryWatchpoints = w;
}

/*QString Debugger::getReasonforPause() const {
   return m_pauseReason;
}*/

// check for watchpoints on memory addresses
void Debugger::memoryChanged(unsigned int address, unsigned int value, ParseNode *pc) {
   if (m_memoryWatchpoints != NULL && m_memoryWatchpoints->contains(address) && m_memoryWatchpoints->value(address)) {
      int oldStatus = m_status;

      setStatus(PAUSED); // modified a user-protected memory address
      
      // notifyPause
      notifyPause(QString("%1Protected memory address '%2' modified").arg(
            (oldStatus == RUNNING ? 
             QString("Execution Paused:  ") : 
             QString("")), QString::number(address, 16)));
   }
}

void Debugger::registerChanged(unsigned int reg, unsigned int value, ParseNode *pc) {
//   cerr << "reg changed " << registerAliases[reg] << " protected: " << m_registerWatchpoints[reg] << endl;
   if (m_registerWatchpoints[reg]) {
      int oldStatus = m_status;

      setStatus(PAUSED); // modified a user-protected register
 
      notifyPause(QString("%1Protected register '%2' modified").arg(
               (oldStatus == RUNNING ? 
               QString("Execution Paused:  ") : 
               QString("")), 
               registerAliases[reg]));
   }
}

// called whenever a register watchpoint is enabled/disabled
void Debugger::watchPointModified(unsigned int reg, bool watchPoint) {
//   cerr << "Debugger::watchPointModified: " << reg << " = " << watchPoint << endl;

   m_registerWatchpoints[reg] = watchPoint;
}

void Debugger::setWatchpoints(const bool *buf) {
   memcpy(m_registerWatchpoints, buf, sizeof(bool) * register_count);
}

const StateException &Debugger::getException() const {
   return m_exception;
}
StateException::~StateException() { }

StateException::StateException() : ParseError(QString(), QString()) { }
StateException::StateException(const QString &message, QTextBlock *b, int lineNo)
   : ParseError(QString("Execution halted:  %1!").arg(message), QString(), -1, b, lineNo)
{ }

SyscallListener *Debugger::getSyscallListener() const {
   return m_syscallListener;
}

