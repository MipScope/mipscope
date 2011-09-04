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
   file: MazeUi.C
   auth: Andrew Brindamour
   acct: ab
   date: 8/31/2008
\* ---------------------------------------------- */

#include "MazeUi.H"
#include "MazeParser.H"

MazeUi::MazeUi() : Maze() {
}

MazeUi::~MazeUi() { }


bool MazeUi::initializeMaze() {
   //cerr << "init: " << QThread::currentThreadId() << endl;
   //cerr << "mine: " << (this->thread() == QThread::currentThread()) << endl;

	if (const char* default_maze = getenv("MAZE_DEFAULT")) {
		load_maze_from_file(default_maze);
	} else {
		if (!loadFile())
			// canceled by user
			return false;
	}
	setupUI();
	return true;
}
