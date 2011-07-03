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
   file: MazeParser.H
   auth: Andrew Ayer
   date: 2011-07-03
\* ---------------------------------------------- */
#ifndef __MAZE_PARSER_H__
#define __MAZE_PARSER_H__

#include <string>
#include <iosfwd>
#include "Maze.H"

class MazeParser {
public:
	struct Error {
		int				lineno;
		std::string			message;

		Error (unsigned int l, const std::string& m) : lineno(l), message(m) { }
		explicit Error (const std::string& m) : lineno(-1), message(m) { }
	};

	static void parse (std::istream&, Maze::Info&);

	static void parse (const std::string& filename, Maze::Info&);
};

#endif // __MAZE_PARSER_H__
