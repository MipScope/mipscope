#include <iostream>
#include <QMutexLocker>

#include "State.H"
#include "StateException.H"
#include "ParseList.H"
#include "Debugger.H"
#include "ParseNode.H"
#include "../gui/Utilities.H"

// constructor
Debugger::Debugger(ParseList* parseList) : m_debuggerThread(parseList, this)
{
   connect(&m_debuggerThread, SIGNAL(finished(void)), this, SLOT(threadTerminated(void)));
}

int Debugger::getStatus(void) { 
   return m_debuggerThread.getStatus();
}

State *Debugger::getState() {
   return m_debuggerThread.m_state;
}

bool Debugger::waitOnDebuggerThread(unsigned long timeout) {
   return m_debuggerThread.wait(timeout);  
}

// slot
void Debugger::threadTerminated(void) {
   cerr << "Debugger::threadTerminated\n";

   emit programStatusChanged(STOPPED);
}

// slot
void Debugger::programStop(void) {
   if (getStatus() == STOPPED) return;

   m_debuggerThread.setStatus(STOPPED);   
}

// slot
void Debugger::programPause(void) {
   if (getStatus() == PAUSED) return;

   m_debuggerThread.setStatus(PAUSED);
}

// slot
void Debugger::programRun(void) {
   cerr << "Debugger::programRun()\n";

   if (getStatus() == RUNNING) return;
   //   if (getStatus() == STOPPED)

   m_debuggerThread.setStatus(RUNNING);

   m_debuggerThread.start();
}


// slot
void Debugger::programStepForward(void) { } // TODO

void Debugger::programStepBackward() { } // TODO

// slot
void Debugger::programStepBackwardToTimestamp(TIMESTAMP stamp) { stamp=0;} // TODO


void Debugger::setParseList(ParseList *list) {
   m_debuggerThread.m_parseList = list;
}


// ******************************************
// DebuggerThread
// ******************************************

DebuggerThread::DebuggerThread(ParseList* parseList, Debugger *parent)
   : m_state(new State()), m_parseList(parseList), m_status(STOPPED), 
   m_debugger(parent)
{ }

// the main wook of the thread
void DebuggerThread::run(void) {
   cerr << "DebuggerThread::run\n";

   if (!m_parseList->isValid()) {
      cerr << "Debugger: parseList isn't valid.\n";
      return;
   }

   if (!m_parseList->initialize(m_state)) {
      cerr << "Debugger: parseList couldn't be initialized.\n";
      return; 
   }

   // let's run!
   cerr << "Debugger::run: now executing the program.\n";

   while (getStatus() != STOPPED) {
      waitOnStatus(PAUSED);
      
      // Check if we're at the end of the program
      if (m_state->getPC() == NULL) {
         setStatus(STOPPED);
         break;
      }
      cerr << "\tExecuting: " << m_state->getPC() << endl;

      // Check for a breakpoint
      if (m_state->getPC()->containsBreakPoint()) {
         cerr << ">>> BREAKPOINT\n";

         setStatus(PAUSED);
         continue;
      }

      try {
         // execute another parsenode  
         m_state->getPC()->execute(m_state);
      } catch (StateException e) {
         setStatus(STOPPED);
         std::cerr << e.getMessage().toStdString();  
      }    
   }

   cerr << "DebuggerThread::end of run()\n";
   //   delete m_state;
}

// protected
void DebuggerThread::setStatus(int status) {
   QMutexLocker locker(&m_statusMutex);
   m_status = status;
   cerr << "DebuggerThread::emitting status: " << status << endl;
   m_debugger->programStatusChanged(status);
   m_statusChangedWaitCondition.wakeAll();
}

int DebuggerThread::getStatus(void) { 
   QMutexLocker locker(&m_statusMutex); // locks the mutex, automatically unlocked when destructed
   return m_status;
}

// private
void DebuggerThread::waitOnStatus(int status) {
   m_statusMutex.lock();
   while (m_status == status)
      m_statusChangedWaitCondition.wait(&m_statusMutex);
   m_statusMutex.unlock();  
}
