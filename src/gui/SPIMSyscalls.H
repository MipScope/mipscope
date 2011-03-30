/*
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
 */

/* ---------------------------------------------- *\
   file: SPIMSyscalls.H
   auth: Andrew Ayer
   date: 2011-03-28
\* ---------------------------------------------- */
#ifndef __SPIMSYSCALLS_H__
#define __SPIMSYSCALLS_H__

#include "SyscallHandler.H"

class IOConsole;
class SyscallListener;

class SPIMSyscalls : public SyscallHandler {
	IOConsole*		io;

	void output_syscall (State*, int status, int syscall, int valueOfa0);
	void input_syscall (State*, int status, int syscall, int valueOfa0);
	void read_string (State*, int status, unsigned int buffer_address, size_t max_length);
public:
	SPIMSyscalls ();
	explicit SPIMSyscalls (IOConsole*, SyscallListener* =NULL);

	void set_io_console (IOConsole* arg_io) { io = arg_io; }
	void register_with (SyscallListener*);

	virtual void syscall (State*, int status, int syscall, int valueOfa0);
	virtual void undo_syscall (int syscall);
	virtual void reset ();
};

#endif // __SPIMSYSCALLS_H__
