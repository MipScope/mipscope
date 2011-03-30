/*
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * Copyright (C) 2011 Andrew Ayer
 *
 * This file is part of MipScope <http://mipscope.cs.brown.edu>
 * 
 * MipScope is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * MipScope is distributed in the hope that it will be useful,
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
   auth: Travis Fischer, Tim O'Donnell, Andrew Ayer
   date: 2007-04-27, 2011-03-29
\* ---------------------------------------------- */
#ifndef __SYSCALL_HANDLER_H__
#define __SYSCALL_HANDLER_H__

#include "Syscalls.H"
#include "SyncSyscall.H"
#include <QObject>
#include <map>
#include <memory>

class Gui;
class EditorPane;
class OutputConsole;
class State;
class SyscallHandler;
class MainSyscallProxy;

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
      
	void register_syscall (int syscall_number, SyscallHandler* handler, bool is_undoable =true, bool is_synchronous =false);
      
//public slots:  // no longer slots; must occur in debugger thread
	void syscall(State *s, int status, int syscallNo, int valueOfa0) const;
	void undoSyscall(int syscallNo) const;

public slots:
	void reset();
      
private:
	struct Syscall {
		SyscallHandler*       handler;
		bool                  is_undoable;
		bool                  is_synchronous;

		Syscall () { handler = NULL; is_undoable = false, is_synchronous = false; }

		void init (SyscallHandler* handler, bool is_undoable, bool is_synchronous);
	};

	Gui*                             m_gui;
	std::map<int, Syscall>           m_syscalls;
	std::auto_ptr<MainSyscallProxy>  m_main_proxy;
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
	virtual ~SyscallHandler () { }

	// Should execute this syscall
	virtual void syscall (State*, int status, int syscallNo, int valueOfa0) = 0;

	// Should undo the last call to syscall
	// Note:  Implementation left up to the implementing SyscallHandler.
	virtual void undo_syscall(int syscall) = 0;
	virtual void reset() = 0;
};

#endif // __SYSCALL_HANDLER_H__

