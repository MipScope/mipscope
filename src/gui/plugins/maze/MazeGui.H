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
   file: MazeGui.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007

   This plugin was written for the Brown University 
   Department of Computer Science (cs031).
\* ---------------------------------------------- */
#ifndef __MAZE_GUI_H__
#define __MAZE_GUI_H__

#include <QWidget>
#include <QLabel>
#include "MazeUi.H"

class Gui;
class MazePlugin;
class QCloseEvent;

class MazeGui : public QWidget, public MazeUi {
   public:
      MazeGui(Gui *gui, MazePlugin *plugin);
      virtual ~MazeGui();

      void updateNodesVisited();
		virtual void showMessage(QString title, QString message);
      
      // @overridden
      //virtual int moveCurrent(point loc);
      //virtual int undoMove(point loc);

   protected:
      // @overridden
      virtual void closeEvent(QCloseEvent *event);
      virtual bool loadFile();
      virtual void onNodesVisitedChanged();

      void setupUI();
      
      QLabel m_noVisited;
      QWidget *m_cellPanel;
      QtCell **m_cells;

      Gui *m_gui;
      MazePlugin *m_plugin;
};

#endif // __MAZE_GUI_H__

