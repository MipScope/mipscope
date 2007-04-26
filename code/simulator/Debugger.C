#include <iostream>
#include <QMutexLocker>

#include "State.H"
#include "StateException.H"
#include "ParseList.H"
#include "Debugger.H"
#include "ParseNode.H"

// constructor
Debugger::Debugger(ParseList* parseList) : m_debuggerThread(parseList) {
   QObject::connect(&m_debuggerThread, SIGNAL(finished(void)), this, SLOT(threadTerminated(void)));
}  

Status Debugger::getStatus(void) { 
   return m_debuggerThread.getStatus();
}

bool Debugger::waitOnDebuggerThread(unsigned long timeout) {
   return m_debuggerThread.wait(timeout);  
}

// slot
void Debugger::threadTerminated(void) {
   emit stopped();
}

// slot
void Debugger::programStop(void) {
   
   if (getStatus() == STOPPED) return;
   
   m_debuggerThread.setStatus(STOPPED);   
   emit stopped();   
}

// slot
void Debugger::programPause(void) {
   
   if (getStatus() == PAUSED) return;
   
   m_debuggerThread.setStatus(PAUSED);
   emit paused();
}

// slot
void Debugger::programRun(void) {
   cerr << "Debugger::programRun()\n";
   
   if (getStatus() == RUNNING) return;
   if (getStatus() == STOPPED) {
      m_debuggerThread.start();      
   }
   m_debuggerThread.setStatus(RUNNING);
   emit running();   
}


// slot
void Debugger::programStepForward(void) { } // TODO

// slot
void Debugger::programStepBackwardToTimestamp(TIMESTAMP stamp) { } // TODO




// ******************************************
// DebuggerThread
// ******************************************

DebuggerThread::DebuggerThread(ParseList* parseList): m_state(NULL),
                                          m_parseList(parseList), 
                                          m_statusMutex(),
                                          m_status(STOPPED),
                                          m_statusChangedWaitCondition()
{ }   

// the main wook of the thread
void DebuggerThread::run(void) {
   
   cerr << "DebuggerThread::run\n";
   
   m_state = new State();
   
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
      
      if (m_state->getPC() == NULL) {
         setStatus(STOPPED);
         break;
      }
      cerr << "\tExecuting: " << m_state->getPC() << endl;
      
      // execute another parsenode  
      
      try {
         m_state->getPC()->execute(m_state);
      } catch (StateException e) {
         setStatus(STOPPED);
         std::cout << e.getMessage().toStdString();  
      }    
   }
   
//   delete m_state;
}

// protected
void DebuggerThread::setStatus(Status status) {
   QMutexLocker locker(&m_statusMutex);
   m_status = status;
   m_statusChangedWaitCondition.wakeAll();
}

Status DebuggerThread::getStatus(void) { 
   QMutexLocker locker(&m_statusMutex); // locks the mutex, automatically unlocked when destructed
   return m_status;
}

// private
void DebuggerThread::waitOnStatus(Status status) {
   m_statusMutex.lock();
   while (m_status == status)
      m_statusChangedWaitCondition.wait(&m_statusMutex);
   m_statusMutex.unlock();  
}
