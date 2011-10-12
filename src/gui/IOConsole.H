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
   file: IOConsole.H
   auth: Andrew Ayer
   date: 2011-03-28
\* ---------------------------------------------- */
#ifndef __IOCONSOLE_H__
#define __IOCONSOLE_H__

#include <string>
#include <limits>

class State;

class IOConsole {
public:
	virtual ~IOConsole () { }

	virtual std::string	prompt_for_string (State*, int status, std::string prompt_message, size_t max_length) = 0;
	virtual int		prompt_for_int (State*, int status, std::string prompt_message, int min, int max) = 0;
	virtual double		prompt_for_double (State*, int status, std::string prompt_message, double min, double max) = 0;
	virtual void		output_string (State*, int status, std::string message) = 0;
	virtual void		clear_output (State*, int status) = 0; // clear the console

	virtual void		reset () = 0;

	virtual std::string	prompt_for_string (State* state, int status, std::string prompt_message) { return prompt_for_string(state, status, prompt_message, std::string::npos); }
	virtual int		prompt_for_int (State* state, int status, std::string prompt_message) { return prompt_for_int(state, status, prompt_message, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); }
	virtual double		prompt_for_double (State* state, int status, std::string prompt_message) { return prompt_for_double(state, status, prompt_message, std::numeric_limits<double>::min(), std::numeric_limits<double>::max()); }

	// Undo the last output (including clears) which was printed to the console
	// You can call undo_output multiple times to undo the last several outputs
	virtual void		undo_output () = 0;
	virtual bool		can_undo_output () const = 0;

	// Exception may be thrown if the user aborts input
	class InputError { };
};


#endif // __IOCONSOLE_H__
