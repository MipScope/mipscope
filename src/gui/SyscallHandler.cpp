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
#include <cassert>
#include <sstream>

// Handles all syscalls outputted by a Program
// -------------------------------------------

SyscallListener::SyscallListener(Gui *gui) 
: QObject(gui), m_gui(gui), m_main_proxy(new MainSyscallProxy)
{
}

void SyscallListener::register_syscall (int syscall_number, SyscallHandler* handler, bool is_undoable, bool is_synchronous)
{
	assert(handler);

	m_syscalls[syscall_number].init(handler, is_undoable, is_synchronous);
}

void SyscallListener::syscall(State *s, int status, int syscallNo, int valueOfa0) const
{
	std::map<int, Syscall>::const_iterator	it(m_syscalls.find(syscallNo));

	if (it == m_syscalls.end()) {
		std::ostringstream	error;
		error << syscallNo << ": Invalid syscall number";
		s->breakError(error.str().c_str());
		return;
	}

	const Syscall&		syscall(it->second);

	if (syscall.is_synchronous) {
		LocalSyscallProxy(syscall.handler, m_main_proxy.get()).syscall(s, status, syscallNo, valueOfa0);
	} else {
		syscall.handler->syscall(s, status, syscallNo, valueOfa0);
	}
}

void SyscallListener::undoSyscall(int syscallNo) const
{
	std::map<int, Syscall>::const_iterator	it(m_syscalls.find(syscallNo));

	assert(it != m_syscalls.end()); // we shouldn't be undoing a syscall if we weren't able to execute it in the first place!

	const Syscall&		syscall(it->second);

	if (syscall.is_synchronous) {
		LocalSyscallProxy(syscall.handler, m_main_proxy.get()).undoSyscall(syscallNo);
	} else {
		syscall.handler->undo_syscall(syscallNo);
	}
}

void SyscallListener::reset()
{
	std::map<int, Syscall>::const_iterator	it(m_syscalls.begin());
	while (it != m_syscalls.end()) {
		const Syscall&	syscall(it->second);
		if (syscall.is_synchronous) {
			LocalSyscallProxy(syscall.handler, m_main_proxy.get()).reset();
		} else {
			syscall.handler->reset();
		}
		++it;
	}
}

void SyscallListener::Syscall::init (SyscallHandler* arg_handler, bool arg_is_undoable, bool arg_is_synchronous)
{
	handler = arg_handler;
	is_undoable = arg_is_undoable;
	is_synchronous = arg_is_synchronous;
}
