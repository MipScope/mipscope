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
class ExitHandler;

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
      
   //public slots:  // no longer slots; must occur in debugger thread
      void syscall(State *s, int status, int syscallNo, int valueOfa0);
      void undoSyscall(int syscallNo);

   public slots:
      void reset();
      
   protected:
      Gui *m_gui;
      QHash<int, SyscallHandler*> m_syscallMap;
      ExitHandler *m_exitHandler;
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
class SyscallHandler {
   
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

class ExitHandler : public SyscallHandler {

   public:
      ExitHandler(SyscallListener *listener);
      
      void syscall(State *s, int status, int syscallNo, int valueOfa0);

      void undoSyscall(int syscallNo);
      void reset();
};


#endif // __SYSCALL_HANDLER_H__

