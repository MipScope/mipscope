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
      DebuggerThread(ParseList* parseList, Debugger *debugger);
      void run(void);
      int getStatus(void);
      void setStatus(int status);   
       
      State* m_state;
      ParseList* m_parseList;      
      QMutex m_statusMutex;
      int m_status;
      QWaitCondition m_statusChangedWaitCondition;            
      Debugger *m_debugger;
      
   private:
      void waitOnStatus(int status);
};


class Debugger : public QObject {
   
   Q_OBJECT
   
   public:
      friend class DebuggerThread;
      
      Debugger(ParseList* parseList = NULL);
      bool waitOnDebuggerThread(unsigned long timeout);
      
      int getStatus(void);
      State *getState();
      
      void setParseList(ParseList *list);
      
   public slots:
      void programStop(void);
      void programPause(void);
      void programRun(void);
      void programStepForward(void);
      void programStepBackward();
      void programStepBackwardToTimestamp(TIMESTAMP stamp);
      void threadTerminated(void);       
      
   signals:
      // manage the output stack, caught by the GUI
      //void outputStackPush(QString item);
      //void outputStackPop(void);
      
      // also for the GUI:
      void programStatusChanged(int s);
      
   private:
      DebuggerThread m_debuggerThread;
};

#endif // DEBUGGER_H_

