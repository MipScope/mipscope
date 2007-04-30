/* ---------------------------------------------- *\
   file: SyscallHandler.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/27/2007
\* ---------------------------------------------- */
#ifndef __SYSCALL_HANDLER_H__
#define __SYSCALL_HANDLER_H__

#include "Syscalls.H"
#include <QObject>
#include <QHash>

class Gui;
class EditorPane;
class OutputConsole;
class State;
class SyscallHandler;

/* SyscallListener:
 * ---------------
 * 
 *    Maintains an internal map of syscall numbers to their
 * corresponding SyscallHandlers, all of which have to be 
 * regitered first, with registerHandler().  Acts as a proxy 
 * between an active program and the appropriate SycallHandler.
 *
 */
class SyscallListener : public QObject {
   
   Q_OBJECT
   
   public:
      SyscallListener(Gui *parent);
      
      void registerHandler(SyscallHandler *handler);
      
   public slots:
      void syscall(State *s, int status, int syscallNo, int valueOfa0);
      void undoSyscall(int syscallNo);
      void reset();
      
   protected:
      Gui *m_gui;
      QHash<int, SyscallHandler*> m_syscallMap;
};

/* SyscallHandler:
 * ---------------
 * 
 *    Generic interface for communicating with an active 
 * program's syscalls.  Any class wishing to listen to a 
 * specific syscall number must subclass SyscallHandler 
 * and provide it with a valid SyscallListener.
 *
 */
class SyscallHandler : public QObject{
   
   Q_OBJECT
   
   public:
      // Initializes and registers this SyscallHandler with the given SyscallListener
      SyscallHandler(SyscallListener *listener, int mySyscallNo, bool handlesUndo = true);
      virtual ~SyscallHandler();
      
      int getSyscallNo() const;
      bool handlesUndo() const;
      
      // Core functionality:
      
      // Should execute this syscall
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0) = 0;

      // Should undo the last call to syscall
      // Note:  Implementation left up to the implementing SyscallHandler.
      virtual void undoSyscall(int syscallNo) = 0;
      virtual void reset() = 0;
      
   protected:
      int m_syscallNo;
      bool m_handlesUndo;
};

#endif // __SYSCALL_HANDLER_H__

