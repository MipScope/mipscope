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
   file: SPIMSyscalls.cpp
   auth: Andrew Ayer
   date: 2011-03-28
\* ---------------------------------------------- */

#include "Syscalls.H"
#include "SPIMSyscalls.H"
#include "IOConsole.H"
#include "../simulator/State.H"
#include <sstream>
#include <cassert>

SPIMSyscalls::SPIMSyscalls ()
{
	io = NULL;
}

SPIMSyscalls::SPIMSyscalls (IOConsole* arg_io, SyscallListener* listener)
{
	io = arg_io;
	if (listener)
		register_with(listener);
}

void SPIMSyscalls::register_with (SyscallListener* listener)
{
	assert(listener);
	assert(io);

	listener->register_syscall(S_PRINT_INT, this, io->can_undo_output(), false);
	listener->register_syscall(S_PRINT_STRING, this, io->can_undo_output(), false);
	listener->register_syscall(S_PRINT_CHAR, this, io->can_undo_output(), false);
	listener->register_syscall(S_CLEAR_OUTPUT, this, io->can_undo_output(), false);
	listener->register_syscall(S_PRINT_FLOAT, this, io->can_undo_output(), false);
	listener->register_syscall(S_PRINT_DOUBLE, this, io->can_undo_output(), false);

	listener->register_syscall(S_READ_INT, this, false, true);
	listener->register_syscall(S_READ_CHAR, this, false, true);
	listener->register_syscall(S_READ_STRING, this, false, true);
	listener->register_syscall(S_READ_FLOAT, this, false, true);
	listener->register_syscall(S_READ_DOUBLE, this, false, true);

	listener->register_syscall(S_EXIT, this, false, false);
}

void SPIMSyscalls::syscall (State* s, int status, int syscall, int valueOfa0)
{
	switch (syscall) {
	case S_CLEAR_OUTPUT:
		io->clear_output(s, status);
		break;

	case S_PRINT_INT:
	case S_PRINT_STRING:
	case S_PRINT_CHAR:
	case S_PRINT_FLOAT:
	case S_PRINT_DOUBLE:
		output_syscall(s, status, syscall, valueOfa0);
		break;

	case S_READ_INT:
	case S_READ_CHAR:
	case S_READ_STRING:
	case S_READ_FLOAT:
	case S_READ_DOUBLE:
		input_syscall(s, status, syscall, valueOfa0);
		break;

	case S_EXIT:
		s->exit();
		break;
	}
}

void SPIMSyscalls::output_syscall (State* s, int status, int syscall, int valueOfa0)
{
	std::ostringstream	output;

	switch (syscall) {
	case S_PRINT_INT:
		output << valueOfa0;
		break;
	case S_PRINT_STRING:
		output << qPrintable(s->getString(valueOfa0));
		break;
	case S_PRINT_CHAR:
		output << char(valueOfa0 & 0xFF);
		break;
	case S_PRINT_FLOAT:
	case S_PRINT_DOUBLE:
		// floating point not implemented in MipScope
		output << "???";
		break;
	default:
		return;
	}

	io->output_string(s, status, output.str());
}

void SPIMSyscalls::input_syscall (State* s, int status, int syscall, int valueOfa0)
try {
	switch (syscall) {
	case S_READ_INT:
		s->setRegister(v0, io->prompt_for_int(s, status, "Input integer"));
		break;
	case S_READ_CHAR:
		s->setRegister(a0, io->prompt_for_int(s, status, "Input character", -127, 255)); // XXX: signedness?
		break;
	case S_READ_FLOAT:
		io->prompt_for_double(s, status, "Input float");
		// floating point not implemented in MipScope
		break;
	case S_READ_DOUBLE:
		io->prompt_for_double(s, status, "Input double");
		// floating point not implemented in MipScope
		break;
	case S_READ_STRING:
		// a0 holds address of buffer
		// a1 holds maximum length (including null terminator)
		read_string(s, status, valueOfa0, s->getRegister(a1));
		break;
	}

} catch (IOConsole::InputError) {
	s->syscallError("Input error");
}

void SPIMSyscalls::read_string (State* s, int status, unsigned int buffer_address, size_t max_length)
{
	if (max_length == 0) {
		s->syscallError("read_string called with a max length ($a1) of 0");
		return;
	}

	std::string	result(io->prompt_for_string(s, status, "Input string", max_length - 1));

	// add 1 to result.size() so we also copy in the null terminator, which is present
	// in the string returned by result.c_str()
	s->memcpy(buffer_address, result.c_str(), result.size() + 1);
}

void SPIMSyscalls::undo_syscall (int syscall)
{
	if (syscall == S_PRINT_INT ||
			syscall == S_PRINT_STRING ||
			syscall == S_PRINT_CHAR ||
			syscall == S_CLEAR_OUTPUT ||
			syscall == S_PRINT_FLOAT ||
			syscall == S_PRINT_DOUBLE) {
		io->undo_output();
	}
}

void SPIMSyscalls::reset ()
{
	io->reset();
}

