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
   file: QtCell.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007

   This plugin was written for the Brown University 
   Department of Computer Science (cs031).
\* ---------------------------------------------- */
#ifndef __QT_CELL_H__
#define __QT_CELL_H__

#include <QWidget>
#include <QPixmap>
#include <QImage>

class QPixmap;
class QImage;
class Cell;
class MazeGui;

#define CELL_SIZE    (80)

enum I_ORDER { I_CORNER_WALL, I_CORNER_BLANK, I_CORNER_SEARCHED, 
   I_HORIZONTAL_WALL, I_HORIZONTAL_BLANK, I_HORIZONTAL_SEARCHED, 
   I_VERTICAL_WALL, I_VERTICAL_BLANK, I_VERTICAL_SEARCHED, 
   I_CURRENT_GROUND, I_EMPTY_GROUND, 
   I_SEARCHED_GROUND, I_BACKTRACKED_GROUND, 
   I_FLAG, I_MONEY, I_SCYTHE
};

// Qt-gui version of a single cell in a Maze
class QtCell : public QWidget {
   Q_OBJECT

   public:
      QtCell(MazeGui *parent, int i, int j, Cell *cell);
      
      void updateCell();

   protected:
      
      // @overridden
      virtual void paintEvent(QPaintEvent *e);

      MazeGui *m_parent;
      int m_row, m_col;
      Cell *m_cell;
      int m_oldStatus;
      
};

#endif // __QT_CELL_H__

