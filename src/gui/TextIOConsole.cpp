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
   file: TextIOConsole.cpp
   auth: Andrew Ayer
   date: 2011-03-28
\* ---------------------------------------------- */
#include "TextIOConsole.H"
#include <ostream>
#include <istream>

TextIOConsole::TextIOConsole (std::istream& arg_in, std::ostream& arg_out, std::ostream& arg_prompt_out)
: in(arg_in), out(arg_out), prompt_out(arg_prompt_out)
{
}

std::string		TextIOConsole::prompt_for_string (State* state, int status, std::string prompt_message, size_t max_length)
{
	std::string	input;
	prompt_out << prompt_message << ": ";
	if (!std::getline(in, input))
		throw InputError();
	return input.substr(0, max_length);
}

int		TextIOConsole::prompt_for_int (State* state, int status, std::string prompt_message, int min, int max) // TODO: handle min/max
{
	int		input;
	prompt_out << prompt_message << ": ";
	in >> input;
	if (!in)
		throw InputError();
	return input;
}

double		TextIOConsole::prompt_for_double (State* state, int status, std::string prompt_message, double min, double max) // TODO: handle min/max
{
	double		input;
	prompt_out << prompt_message << ": ";
	in >> input;
	if (!in)
		throw InputError();
	return input;
}

void	TextIOConsole::output_string (State* state, int status, std::string str)
{
	out << str;
}


void	TextIOConsole::clear_output (State* state, int status)
{
}

void	TextIOConsole::reset ()
{
}

