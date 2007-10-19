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
   file: Parser.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <QObject>
#include <QTextBlock>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QTime>
#include <QHash>
#include <QString>

#include "typedefs.H"

class ParseList;
class State;
class SyscallListener;

extern bool VERBOSE;

#include "Parser.H"
class StateException : public ParseError
{
	public:
		StateException();
		StateException(const QString &message, QTextBlock *b = NULL, int lineNo = -1);
      virtual ~StateException();
};

/* The Debugger class is the interface to the debugger. It's a normal QObject,
 * and uses signals and slots to communicate with the GUI. 
 * 
 * DebuggerThread is a thread that the Debugger manages. When you call programStop()
 * on Debugger, it changes a member variable of DebuggerThread so that, upon 
 * completion of the current instruction, DebuggerThread stops executing the parseList.
 * 
 */

class Debugger : public QThread {

   Q_OBJECT
   
   public:
      Debugger(SyscallListener *listener, ParseList* parseList = NULL);
      
      SyscallListener *getSyscallListener() const;
      int getStatusWithoutLocking() const;
      int getStatus();
      State *getState();
      void setParseList(ParseList *list);
      ParseList *getParseList() const;
      
      QString getDuration() const;
      
      void programStop(void);
      void programForceStop(void);
      void programPause(void);
      void programRun(void);
      void programStepForward();
      void programStepBackward();
      void programStepForward(int noInstructions);
      void programStepBackward(int noInstructions);
      void programStepBackwardToTimestamp(TIMESTAMP stamp);
      
      // called whenever a register watchpoint is enabled/disabled
      void watchPointModified(unsigned int reg, bool watchPoint);
      void setWatchpoints(const bool *buf);
      
      bool waitOnDebuggerThread(unsigned long timeout);
      
      const StateException &getException() const;
      
      void setMemoryWatchpoints(QHash<unsigned int, bool> *w);
      
//      QString getReasonforPause() const;

   signals:
      // uses enum Status from typedefs.H
      void programStatusChanged(int s);
      // uses enum TerminationReason from typedefs.H
      void programTerminated(int reason);
      
      void notifyPause(const QString &reason);
      
   private slots:
      void threadTerminated();
      
   public slots:
      // for observing register watchpoints;
      // this must be executed right away, as opposed to being in another thread
      void registerChanged(unsigned int register, unsigned int value, ParseNode *pc);
      void memoryChanged(unsigned int address, unsigned int value, ParseNode *pc);
      
   protected:
      bool checkProgramCompleted();
      
      void setStatusConditionally(int desiredStatus);
      void setStatus(int status);
      void run();

      State* m_state;
      ParseList* m_parseList;      
      QMutex m_statusMutex;
      int m_status, m_terminationReason;
      QWaitCondition m_statusChangedWaitCondition;
      
      // flags denoting which registers are protected by memory watchpoints
      bool m_registerWatchpoints[register_count];
      QHash<unsigned int, bool> *m_memoryWatchpoints;
      StateException m_exception;
      
      // for keeping track of how long programs ran for
      QTime m_time;
      QString m_duration;

   private:
      // returns the new status once m_status has changed from the
      // status passed in.
      int waitOnStatus(int status);
      void runAnotherStep(void);
      void executionInit(void);
      
      // set when user rolls program back past program entry point
      //    (ex. by deleting the main: label...)
      bool m_forceStopped;
      SyscallListener *m_syscallListener;
//      QString m_pauseReason;
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

