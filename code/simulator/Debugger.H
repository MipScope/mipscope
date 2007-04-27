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

/* The Debugger class is the interface to the debugger. It's a normal QObject,
 * and uses signals and slots to communicate with the GUI. 
 * 
 * DebuggerThread is a thread that the Debugger manages. When you call programStop()
 * on Debugger, it changes a member variable of DebuggerThread so that, upon 
 * completion of the current instruction, DebuggerThread stops executing the parseList.
 * 
 */

class Debugger: public QThread {

   Q_OBJECT
   
   public:
      Debugger(ParseList* parseList = NULL);
      
      int getStatus();
      State *getState();
      void setParseList(ParseList *list);
      
      void programStop(void);
      void programPause(void);
      void programRun(void);
      void programStepForward(void);
      void programStepBackward();
      void programStepBackwardToTimestamp(TIMESTAMP stamp);
      
   signals:
      // uses enum Status from typedefs.H
      void programStatusChanged(int s);
      // uses enum TerminationReason from typedefs.H
      void programTerminated(int reason);
      
   private slots:
      void threadTerminated();

   protected:
      
      void setStatusConditionally(int desiredStatus);
      void setStatus(int status);
      void run();

      State* m_state;
      ParseList* m_parseList;      
      QMutex m_statusMutex;
      int m_status, m_terminationReason;
      QWaitCondition m_statusChangedWaitCondition;            
   
   private:
      // returns the new status once m_status has changed from the
      // status passed in.
      int waitOnStatus(int status);
};

/*
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
};*/

#endif // DEBUGGER_H_

