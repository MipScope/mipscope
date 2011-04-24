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
   file: MazePlugin.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007

   This plugin was written for the Brown University 
   Department of Computer Science (cs031).
\* ---------------------------------------------- */
#ifndef __MAZE_PLUGIN_H__
#define __MAZE_PLUGIN_H__

#include "../PluginHandler.H"
#include "../../SyscallHandler.H"
#include "Maze.H"
#include <QStack>
#include <vector>

class Maze;
class MazeUi;
class State;

enum MazeSyscalls {
   // Maze-Specific (Brown University cs031 Project)
   // ----------------------------------------------
   S_MAZE_INIT = 30,
   S_MAZE_DEFEAT = 31,
   S_MAZE_VICTORY = 32,
   S_MAZE_IS_GOAL = 33,       // a0=room id
   S_MAZE_NEIGHBOR = 34,      // a0=room id, a1=address of buf
   S_MAZE_DRAW_ARROW = 35,    // a0=room id, a1=parent room id  ; aka move pascal
   S_MAZE_IS_SEARCHED = 36,   // a0=room id, out: v0 = 1 for searched, 0 for unsearched
   S_MAZE_READ_ROOM_DATA = 37,// a0=room id, a1=buffer, a2=buffer length
   S_MAZE_VICTORY_MONEY = 38, // a0=money earned
};


/* MazePlugin:
 * -----------
 * 
 *    Main interface between MipScope and the Maze gui plugin.
 * Acts as a proxy between all syscalls and their handling.
 */
class MazePlugin : public Plugin, private SyscallHandler {
   
   public:
		MazePlugin(UI *parent, bool textOnly = false);
      virtual void resetPlugin();

      // From SyscallHandler:
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undo_syscall(int syscallNo);
      virtual void reset();
		
		QString getMazePath();
		void setMazePath(QString path);
      
   protected:
		
      // --------------------------------------
      // Maze Interface (accessed via syscalls)
      // --------------------------------------
      int init_maze(State *s);
      void draw_arrow(State *s, int status, int room, int parentRoom);
      void undo_draw_arrow();
      int is_goal(State *s, int room);
      int is_searched(State *s, int room);
      void victory();
      void victory_money(int money_earned);
      void defeat();
      void showEndMessage(const QString &str);
      void get_neighbors(State *s, int start, int* buf);
      std::vector<int> get_room_data(State *s, int room_id);

      MazeUi *m_maze;
      QStack<int> m_searchStack;
		
		QString m_lastMazeDir;
		bool m_textOnly;
};

#endif // __MAZE_PLUGIN_H__

