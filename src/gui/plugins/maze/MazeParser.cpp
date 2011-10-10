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
   file: MazeParser.cpp
   auth: Andrew Ayer
   date: 2011-07-03
\* ---------------------------------------------- */
#include "MazeParser.H"
#include "Maze.H"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>


namespace {
	std::string	strip_line (const std::string& line)
	{
		std::string::size_type	end = line.find_first_of(';');
		if (end == 0) {
			// whole line is a comment
			return "";
		}

		end = line.find_last_not_of(" \t", end - 1);
		if (end == std::string::npos) {
			// whole line is whitespace
			return "";
		}

		return line.substr(0, end + 1);
	}

}


void	MazeParser::parse (const std::string& filename, Maze::Info& maze)
{
	std::ifstream	file(filename.c_str());
	if (!file)
		throw Error("Unable to open " + filename);
	parse(file, maze);

}

void	MazeParser::parse (std::istream& file, Maze::Info& maze)
{
	maze.width = 0;
	int		row = -1;

	std::string	line;
	for (unsigned int lineno = 1; std::getline(file, line); ++lineno) {
		line = strip_line(line);
		if (line.empty())
			continue;
		
		if (line[0] == '@') {
			// e.g. "@ 5 2 TRAP"
			std::istringstream	line_in(line);
			std::string		trash;
			point			p;
			std::string		type;
			line_in >> trash >> p.x >> p.y >> type;

			Cell::Info&		cell(maze.cells[p]);

			if (cell.type != Cell::NORMAL)
				throw Error(lineno, "More than one item placed on a single cell");

			cell.type = Cell::parse_type_string(type.c_str());

			if (cell.type == -1)
				throw Error(lineno, "Invalid cell type");

			std::string		arg;
			while (line_in >> arg)
				cell.arguments.push_back(arg);

			if (cell.arguments.size() != Cell::type_nbr_arguments(cell.type))
				throw Error(lineno, "Incorrect number of arguments for this cell type");

			continue;
		}

		for (std::string::size_type i = 0; i < line.length(); ++i) {
			char		c = line[i];
			unsigned int	col = i / 2;
			if (i % 2 == 0) {
				// even column - vertical wall information ('|' or ' ')

				if (row == -1) {
					if (c != ' ') {
						throw Error(lineno, "Top row must have spaces for vertical walls");
					}
				} else {
					if (col > maze.width) {
						throw Error(lineno, "Too many columns");
					}

					if (c == '|') {
						if (col < maze.width)
							maze.cells[point(col, row)].walls |= WEST;
						if (col > 0)
							maze.cells[point(col - 1, row)].walls |= EAST;
					} else if (c != ' ') {
						throw Error(lineno, "Invalid vertical wall syntax");
					}
				}
			} else {
				// odd column - horizontal wall information ('_' or ' ')

				if (row == -1) {
					if (c != '#' && c != '_') {
						throw Error(lineno, "Top row must have walls");
					}
					++maze.width;
				} else {
					if (col >= maze.width) {
						throw Error(lineno, "Too many columns");
					}
				}

				if (c == '_' || c == '*') {
					if (row != -1)
						maze.cells[point(col, row)].walls |= SOUTH;
					maze.cells[point(col, row + 1)].walls |= NORTH;
					if (c == '*')
						maze.start_point = point(col, row);
				} else if (c == '#') {
					if (row != -1) {
						throw Error(lineno, "# only allowed in top row");
					}
					maze.cells[point(col, row + 1)].type = Cell::GOAL;
					maze.cells[point(col, row + 1)].walls |= NORTH;
				} else if (c != ' ') {
					throw Error(lineno, "Invalid syntax for horizontal wall");
				}
			}
		}

		++row;
	}

	maze.height = row;

	if (maze.width == 0) {
		throw Error("Maze too small");
	}
	if (maze.height == 0) {
		throw Error("Maze too small");
	}
	if (maze.start_point.y < 0) {
		throw Error("No start point defined");
	}
	if (static_cast<unsigned int>(maze.start_point.y) != maze.height - 1) {
		throw Error("Start point must be in bottom row");
	}

	for (unsigned int x = 0; x < maze.width; ++x) {
		if (!(maze.cells[point(x, 0)].walls & NORTH)) {
			throw Error("Top of maze not walled in");
		}
		if (!(maze.cells[point(x, maze.height-1)].walls & SOUTH)) {
			throw Error("Bottom of maze not walled in");
		}
	}
	for (unsigned int y = 0; y < maze.height; ++y) {
		if (!(maze.cells[point(0, y)].walls & WEST)) {
			throw Error("Left side of maze not walled in");
		}
		if (!(maze.cells[point(maze.width-1, y)].walls & EAST)) {
			throw Error("Right side of maze not walled in");
		}
	}
}

