#include "Debugger.H"
#include "../gui/Utilities.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "Statement.H"
#include "State.H"
#include <QMutexLocker>

Debugger::Debugger(ParseList* parseList) 
   : m_state(new State()), m_parseList(parseList), m_status(STOPPED), 
     m_terminationReason(T_ABNORMAL)
{
   connect(this, SIGNAL(finished()), this, SLOT(threadTerminated()));
}

int Debugger::getStatus(void) {
   // locks the mutex, automatically unlocked when destructed
   QMutexLocker locker(&m_statusMutex);

   return m_status;
}

State *Debugger::getState() {
   return m_state;
}

void Debugger::threadTerminated(void) {
   cerr << "Debugger::threadTerminated\n";
   
   emit programStatusChanged(STOPPED);
   if (m_terminationReason != T_COMPLETED && m_terminationReason != T_TERMINATED)
      m_terminationReason = T_ABNORMAL;
   emit programTerminated(m_terminationReason);
}

void Debugger::programStop(void) {
   setStatusConditionally(STOPPED);
}

void Debugger::programPause(void) {
   setStatusConditionally(PAUSED);
}

void Debugger::programRun(void) {
   cerr << "Debugger::programRun()\n";

   if (getStatus() != RUNNING) {
      setStatus(RUNNING);
      start();
   }
}

// private
void Debugger::runAnotherStep(void) {
   // Check if we're at the end of the program
   ParseNode *pc = m_state->getPC();
   bool programCompleted = (pc == NULL);

   if (!programCompleted) {
      try { // see if the current instruction is a Done instruction
         Done *d = dynamic_cast<Done*>(m_state->getPC());
         if (d != NULL)
            programCompleted = true;
      } catch(bad_alloc&) { } // no, it's not a done instruction
   }

   if (programCompleted) {
      setStatus(STOPPED);
      m_terminationReason = T_COMPLETED;
      cerr << "<<< Program COMPLETED: " << (void*)pc << endl;
      return;
   }
      
   try {
      // execute another parsenode
//      cerr << m_state->getPC() << ",  " << m_state->getPC()->getAddress() << endl;

      m_state->getPC()->execute(m_state, m_parseList);
   } catch (StateException e) {
      setStatus(STOPPED);
      std::cerr << e.getMessage().toStdString() << endl << endl;
   }
   
   // We run this below as well, so we stop the program immediately
   if (m_state->getPC() == NULL) {
      setStatus(STOPPED);
      cerr << "<<< Program COMPLETED: " << (void*)m_state->getPC() << endl;
      m_terminationReason = T_COMPLETED;
      return;
   }
}

bool Debugger::waitOnDebuggerThread(unsigned long timeout) {
   return wait(timeout);  
}

//private
void Debugger::executionInit(void) {
   
   cerr << "Debugger::run\n";

   if (!m_parseList->isValid()) {
      cerr << "Debugger: parseList isn't valid.\n";
      m_terminationReason = T_INVALID_PROGRAM;
      return;
   }

   if (!m_parseList->initialize(m_state)) {
      cerr << "Debugger: parseList couldn't be initialized.\n";
      m_terminationReason = T_INVALID_PROGRAM;
      return; 
   }  
}

// public
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

void Debugger::programStepBackward() { } // TODO

void Debugger::programStepBackwardToTimestamp(TIMESTAMP stamp) { stamp=0;} // TODO


void Debugger::setParseList(ParseList *list) {
   QMutexLocker locker(&m_statusMutex);
   m_parseList = list;
}

void Debugger::run(void) {
   
   executionInit();   
   
   // let's run!
   cerr << "Debugger::run: now executing the program.\n";
   
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
      cerr << "\tExecuting: " << m_state->getPC() << endl;

      // Check for a breakpoint
      if (m_state->getPC()->containsBreakPoint()) {
         cerr << ">>> BREAKPOINT\n";

         setStatus(PAUSED);
         waitOnStatus(PAUSED);
         if (getStatus() == STOPPED)
            break; // check if waitOnStatus was interrupted
//         continue;
      }
      // execute another parsenode  
      runAnotherStep();
   }
   
   cerr << "Debugger::end of run(); terminationReason = " << m_terminationReason << endl;
}

void Debugger::setStatus(int status) {
   QMutexLocker locker(&m_statusMutex);
   
   m_status = status;
   cerr << "Debugger emitting status: " << 
      (status == STOPPED ? "stopped" : 
       (status == PAUSED ? "paused" : "running")) << endl;
   emit programStatusChanged(status);
   
   m_statusChangedWaitCondition.wakeAll();
}

void Debugger::setStatusConditionally(int status) {
    QMutexLocker locker(&m_statusMutex);
    
    if (m_status != status) {
       m_status = status;
       cerr << "Debugger::emitting status: " << status << endl;
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

