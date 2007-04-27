#include "Debugger.H"
#include "../gui/Utilities.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "State.H"
#include <QMutexLocker>

Debugger::Debugger(ParseList* parseList) 
   : m_state(new State()), m_parseList(parseList), m_status(STOPPED)
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


// slot
void Debugger::programStepForward(void) { } // TODO

void Debugger::programStepBackward() { } // TODO

// slot
void Debugger::programStepBackwardToTimestamp(TIMESTAMP stamp) { stamp=0;} // TODO


void Debugger::setParseList(ParseList *list) {
   QMutexLocker locker(&m_statusMutex);
   m_parseList = list;
}

void Debugger::run(void) {
   cerr << "Debugger::run\n";

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
      if (getStatus() == STOPPED)
         break; // check if waitOnStatus was interrupted
      
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
   
   cerr << "Debugger::end of run()\n";
}

void Debugger::setStatus(int status) {
   QMutexLocker locker(&m_statusMutex);
   
   m_status = status;
   cerr << "Debugger::emitting status: " << status << endl;
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

void Debugger::waitOnStatus(int status) {
   cerr << "\tentering wait\n";
   
   m_statusMutex.lock();
   while (m_status == status)
      m_statusChangedWaitCondition.wait(&m_statusMutex);
   m_statusMutex.unlock();
   
   cerr << "\texiting wait\n";
}

