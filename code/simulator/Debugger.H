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

enum Status { STOPPED, PAUSED, RUNNING };

/* The Debugger is its own thread.
 * 
 */

// to start this thread, call debugger.exec()
//
class Debugger : public QThread {
   
   Q_OBJECT
   
   public:
      Debugger(ParseList* parseList);
      Status getStatus(void);
      void run(void); // the work of the thread
      void terminateWhenProgramStops(void);  
      
   public slots:
      void stop(void);
      void pause(void);
      void start(void);
      void stepForward(void);
      void stepBackwardToTimestamp(TIMESTAMP stamp);            
      
   signals:
      // manage the output stack, caught by the GUI
      void outputStackPush(QString item);
      void outputStackPop(void);
      
      void stopped(void);
      void paused(void);
      void running(void);
      
   private:
      void waitOnStatus(Status status);
      void setStatus(Status status);
   
      QMutex m_stateMutex;
      State* m_state;
      
      QMutex m_parseListMutex;
      ParseList* m_parseList;
      
      QMutex m_statusMutex;
      Status m_status;
      
      QWaitCondition m_statusChangedWaitCondition;
            
      bool m_terminateWhenProgramStops;
      
};

#endif /*DEBUGGER_H_*/
