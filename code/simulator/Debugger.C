#include <QMutexLocker>

#include "State.H"
#include "ParseList.H"
#include "Debugger.H"
#include "ParseNode.H"

Debugger::Debugger(ParseList* parseList): m_stateMutex(),
                                          m_state(NULL),
                                          m_parseListMutex(),
                                          m_parseList(parseList), 
                                          m_statusMutex(),
                                          m_status(STOPPED),
                                          m_statusChangedWaitCondition(),
                                          m_terminateWhenProgramStops(false)
{ }   

// this starts the thread
void Debugger::run(void) {
   
   while (1) {
      
      waitOnStatus(STOPPED);
      
      m_state = new State();
      
      if (m_parseList->isValid() && m_parseList->initialize(m_state)) {
         // let's run!
         
         while (getStatus() != STOPPED) {            
            waitOnStatus(PAUSED);
            
            // execute another parsenode  
            m_state->getPC()->execute(m_state);            
         }
                       
      }
      else {
         assert(false); // for now
      }
      
      delete m_state;  
   }
} 


Status Debugger::getStatus(void) { 
   QMutexLocker locker(&m_statusMutex); // locks the mutex, automatically unlocked when destructed
   return m_status;
}

void Debugger::terminateWhenProgramStops(void) {
   m_terminateWhenProgramStops = true;
}

// private
void Debugger::setStatus(Status status) {
   QMutexLocker locker(&m_statusMutex);
   m_status = status;
   m_statusChangedWaitCondition.wakeAll();
}

// slot
void Debugger::stop(void) {
   setStatus(STOPPED);   
   emit stopped();   
}

// slot
void Debugger::pause(void) {
   setStatus(PAUSED);
   emit paused();
}

// slot
void Debugger::start(void) {
   setStatus(RUNNING);
   emit paused();
} 

// slot
void Debugger::stepForward(void) { } // TODO

// slot
void Debugger::stepBackwardToTimestamp(TIMESTAMP stamp) { } // TODO

// private
void Debugger::waitOnStatus(Status status) {
   m_statusMutex.lock();
   while (getStatus() == status)
      m_statusChangedWaitCondition.wait(&m_statusMutex);
   m_statusMutex.unlock();  
}

