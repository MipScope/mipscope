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
   file: MazeTui.H
   auth: Andrew Brindamour
   acct: ab
   date: 8/31/2008

   This plugin was written for the Brown University 
   Department of Computer Science (cs031).
\* ---------------------------------------------- */
#ifndef __MAZE_TUI_H__
#define __MAZE_TUI_H__

#include "MazeUi.H"

class MazePlugin;
class QCloseEvent;

class MazeTui : public MazeUi {
   public:
      MazeTui(MazePlugin *plugin);
      virtual ~MazeTui();
		virtual void showMessage(QString title, QString message);

   protected:
		virtual bool loadFile();

      void setupUI();
      
      MazePlugin *m_plugin;
};

#endif // __MAZE_GUI_H__

