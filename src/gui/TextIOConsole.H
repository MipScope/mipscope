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
   file: TextIOConsole.H
   auth: Andrew Ayer
   date: 2011-03-28
\* ---------------------------------------------- */
#ifndef __TEXTIOCONSOLE_H__
#define __TEXTIOCONSOLE_H__

#include "IOConsole.H"
#include <string>
#include <iosfwd>

class TextIOConsole : public IOConsole {
	std::istream&		in;
	std::ostream&		out;
	std::ostream&		prompt_out;
public:
	TextIOConsole (std::istream& in, std::ostream& out, std::ostream& prompt_out);

	virtual std::string	prompt_for_string (State*, int, std::string prompt_message, size_t max_length);
	virtual int		prompt_for_int (State*, int, std::string prompt_message, int min, int max);
	virtual double		prompt_for_double (State*, int, std::string prompt_message, double min, double max);
	virtual void		output_string (State*, int, std::string message);
	virtual void		clear_output (State*, int);

	virtual void		reset ();

	virtual void		undo_output () { }
	virtual bool		can_undo_output () const { return false; }
};

#endif // __TEXTIOCONSOLE_H__
