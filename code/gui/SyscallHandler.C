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
   file: SyscallHandler.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/27/2007
\* ---------------------------------------------- */
#include "SyscallHandler.H"
#include "Utilities.H"
#include "Gui.H"
#include "OutputConsole.H"
#include "../simulator/State.H"
#include "SyncSyscall.H"

// Handles all syscalls outputted by a Program
// -------------------------------------------
   SyscallListener::SyscallListener(Gui *gui) 
: QObject(gui), m_gui(gui)
{
   m_exitHandler = new ExitHandler(this);
   m_mainProxy = new MainSyscallProxy();
}

void SyscallListener::registerHandler(SyscallHandler *handler) {
   if (handler == NULL)
      return;

   int syscallNo = handler->getSyscallNo();

   if (syscallNo == S_PRINT) { // convenience for OutputConsole
      m_syscallMap[S_PRINT_INT]    = handler;
      m_syscallMap[S_PRINT_FLOAT]  = handler;
      m_syscallMap[S_PRINT_DOUBLE] = handler;
      m_syscallMap[S_PRINT_STRING] = handler;
      m_syscallMap[S_PRINT_CHAR]   = handler;
   } else m_syscallMap[syscallNo] = handler;
}

void SyscallListener::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   if (m_syscallMap.contains(syscallNo))
   {
      SyscallHandler *handler = m_syscallMap[syscallNo];

      if (handler->isSynchronous())
      {
         LocalSyscallProxy(handler, m_mainProxy).syscall(s, status, syscallNo, valueOfa0);
      }
      else
      {
         handler->syscall(s, status, syscallNo, valueOfa0);
      }
   }

}

void SyscallListener::undoSyscall(int syscallNo) {
   if (m_syscallMap.contains(syscallNo))
   {
      SyscallHandler *handler = m_syscallMap[syscallNo];

      if (handler->isSynchronous())
      {           
         LocalSyscallProxy(m_syscallMap[syscallNo], m_mainProxy).undoSyscall(syscallNo);
      }
      else
      {
         handler->undoSyscall(syscallNo);
      }
   }
}

void SyscallListener::reset() {
   foreach(SyscallHandler *sh, m_syscallMap.values())
   {
      if (sh->isSynchronous())
      {
         LocalSyscallProxy(sh, m_mainProxy).reset();
      }
      else
      {
         sh->reset();
      }
   }
   //m_syscallMap.clear();
}

SyscallHandler::SyscallHandler(SyscallListener *listener, int mySyscallNo, bool handlesUndo, bool synchronous) : 
   m_syscallNo(mySyscallNo), 
   m_handlesUndo(handlesUndo),
   m_synch(synchronous)
{
   listener->registerHandler(this);
}

SyscallHandler::~SyscallHandler() { }

int SyscallHandler::getSyscallNo() const {
   return m_syscallNo;
}

bool SyscallHandler::handlesUndo() const {
   return m_handlesUndo;
}

bool SyscallHandler::isSynchronous() const
{
   return m_synch;
}

ExitHandler::ExitHandler(SyscallListener *listener) : SyscallHandler(listener, S_EXIT, FALSE) { }

void ExitHandler::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   Q_UNUSED(s);
   Q_UNUSED(status);
   Q_UNUSED(syscallNo);
   Q_UNUSED(valueOfa0);

   s->exit();
}

void ExitHandler::undoSyscall(int syscallNo) { Q_UNUSED(syscallNo); }
void ExitHandler::reset() { }

