#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <QObject>
#include <QTextBlock>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "typedefs.H"

class ParseList;
class State;
class Debugger;

enum Status { STOPPED, PAUSED, RUNNING };

/* The Debugger class is the interface to the debugger. It's a normal QObject,
 * and uses signals and slots to communicate with the GUI. 
 * 
 * DebuggerThread is a thread that the Debugger manages. When you call programStop()
 * on Debugger, it changes a member variable of DebuggerThread so that, upon 
 * completion of the current instruction, DebuggerThread stops executing the parseList.
 * 
 */

// don't use this, use Debugger (below)
class DebuggerThread : public QThread {
   
   friend class Debugger;
   
   Q_OBJECT
   
   protected:
      DebuggerThread(ParseList* parseList);
      void run(void);
      Status getStatus(void);
      void setStatus(Status status);   
   
      State* m_state;      
      ParseList* m_parseList;      
      QMutex m_statusMutex;
      Status m_status;      
      QWaitCondition m_statusChangedWaitCondition;            
      
      
   private:
      void waitOnStatus(Status status);
};


class Debugger : public QObject {
   
   Q_OBJECT
   
   public:
      Debugger(ParseList* parseList);
      Status getStatus(void);
      bool waitOnDebuggerThread(unsigned long timeout);
      
   public slots:
      void programStop(void);
      void programPause(void);
      void programRun(void);
      void programStepForward(void);
      void programStepBackwardToTimestamp(TIMESTAMP stamp);
      void threadTerminated(void);       
      
   signals:
      // manage the output stack, caught by the GUI
      void outputStackPush(QString item);
      void outputStackPop(void);
      
      // also for the GUI:
      void stopped(void);
      void paused(void);
      void running(void);
      
   private:
      DebuggerThread m_debuggerThread;
      
};



#endif /*DEBUGGER_H_*/
