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
   file: QtCell.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#include "QtCell.H"
#include <QtGui>
#include <QVector>
#include "Maze.H"
#include "MazeGui.H"
#include "../../Utilities.H"

QPixmap getPixmap(const char *str) {
   QImage i = QImage(str);
   return QPixmap::fromImage(i);
}

QVector<QPixmap> *MazeImages = NULL;

QtCell::QtCell(MazeGui *parent, int i, int j, Cell *cell) :
   m_parent(parent), m_row(i), m_col(j), m_cell(cell)
{
   //connect(m_cell, SIGNAL(stateChanged()), this, SLOT(update()));
   m_cell->m_parent = this;
   
   if (MazeImages==NULL) {
      MazeImages = new QVector<QPixmap>();


#define CORNER_WALL IMAGES "cornerwall.png"
      MazeImages->append(getPixmap(CORNER_WALL));
#define CORNER_BLANK IMAGES "cornerblank.png"
      MazeImages->append(getPixmap(CORNER_BLANK));
#define CORNER_SEARCHED IMAGES "cornersearched.png"
      MazeImages->append(getPixmap(CORNER_SEARCHED));
#define HORIZONTAL_WALL IMAGES "horizontalwall.png"
      MazeImages->append(getPixmap(HORIZONTAL_WALL));
#define HORIZONTAL_BLANK IMAGES "horizontalblank.png"
      MazeImages->append(getPixmap(HORIZONTAL_BLANK));
#define HORIZONTAL_SEARCHED IMAGES "horizontalsearched.png"
      MazeImages->append(getPixmap(HORIZONTAL_SEARCHED));
#define VERTICAL_WALL IMAGES "verticalwall.png"
      MazeImages->append(getPixmap(VERTICAL_WALL));
#define VERTICAL_BLANK IMAGES "verticalblank.png"
      MazeImages->append(getPixmap(VERTICAL_BLANK));
#define VERTICAL_SEARCHED IMAGES "verticalsearched.png"
      MazeImages->append(getPixmap(VERTICAL_SEARCHED));
#define CURRENT_GROUND IMAGES "active.png"
      MazeImages->append(getPixmap(CURRENT_GROUND));
#define EMPTY_GROUND IMAGES "ground.png"
      MazeImages->append(getPixmap(EMPTY_GROUND));
#define SEARCHED_GROUND IMAGES "searched.png"
      MazeImages->append(getPixmap(SEARCHED_GROUND));
#define BACKTRACKED_GROUND IMAGES "backtracked.png"
      MazeImages->append(getPixmap(BACKTRACKED_GROUND));
#define FLAG_IMAGE IMAGES "flag.png"
      MazeImages->append(getPixmap(FLAG_IMAGE));
   }

   setFixedSize(CELL_SIZE, CELL_SIZE);
   setFocusPolicy(Qt::NoFocus);
   setAutoFillBackground(false);
   setAttribute(Qt::WA_NoSystemBackground);
}

// update background to reflect current cell status (repaint)
void QtCell::updateCell() {
   update();
}

// @overridden
void QtCell::paintEvent(QPaintEvent *e) {
   QPainter p(this);
   //cerr << rect().width() << ", " << rect().height() << endl;

   /*const QRect &bounds = rect();
   bool resizeH = (bounds.width() > r.width());
   bool resizeV = (bounds.height() > r.height());

   // ensure background pic always takes up whole frame, scaling if necessary
   if (resizeH || resizeV) {
      m_pixMap = QPixmap::fromImage(m_background.scaled(resizeH ? bounds.width() : r.width(), resizeV ? bounds.height() : r.height()));

      r.setSize(m_pixMap.size());
   }*/

   //r.moveCenter(rect().center());
   
   MazeStatus status = m_cell->getMazeStatus();
   bool northWall = m_cell->hasWall(NORTH),southWall = m_cell->hasWall(SOUTH),
   westWall = m_cell->hasWall(WEST),eastWall = m_cell->hasWall(EAST);
   
#define addWallIfExists(x,y,exists,wall,blank) \
     p.drawPixmap((x), (y), (*MazeImages)[(exists) ? (wall) : (blank)]);
   
   int top=0, mid=8, bot = CELL_SIZE - 8;
   
   //p.fillRect(rect(), QBrush(QColor(255, 0, 0)));
   
   if(status == SEARCHED || status == BACKTRACKED){
      addWallIfExists(top,top,northWall||westWall,I_CORNER_WALL,I_CORNER_SEARCHED);
      addWallIfExists(mid,top,northWall,I_HORIZONTAL_WALL,I_HORIZONTAL_SEARCHED);
      addWallIfExists(bot,top,northWall||eastWall,I_CORNER_WALL,I_CORNER_SEARCHED);
      addWallIfExists(top,mid,westWall,I_VERTICAL_WALL,I_VERTICAL_SEARCHED);
      
      addWallIfExists(mid,mid,(status==SEARCHED), I_SEARCHED_GROUND, I_BACKTRACKED_GROUND);

      addWallIfExists(bot,mid,eastWall,I_VERTICAL_WALL,I_VERTICAL_SEARCHED);
      addWallIfExists(top,bot,southWall||westWall,I_CORNER_WALL,I_CORNER_SEARCHED);
      addWallIfExists(mid,bot,southWall,I_HORIZONTAL_WALL,I_HORIZONTAL_SEARCHED);
      addWallIfExists(bot,bot,southWall||eastWall,I_CORNER_WALL,I_CORNER_SEARCHED);
   }else{
      addWallIfExists(top,top,northWall||westWall,I_CORNER_WALL,I_CORNER_BLANK);
      addWallIfExists(mid,top,northWall,I_HORIZONTAL_WALL,I_HORIZONTAL_BLANK);
      addWallIfExists(bot,top,northWall||eastWall,I_CORNER_WALL,I_CORNER_BLANK);
      addWallIfExists(top,mid,westWall,I_VERTICAL_WALL,I_VERTICAL_BLANK);
      
      point me={m_col,m_row};
      addWallIfExists(mid,mid,(status==CURRENT), I_CURRENT_GROUND, 
         (m_parent->isGoal(me)?I_FLAG:I_EMPTY_GROUND));

      addWallIfExists(bot,mid,eastWall,I_VERTICAL_WALL,I_VERTICAL_BLANK);
      addWallIfExists(top,bot,southWall||westWall,I_CORNER_WALL,I_CORNER_BLANK);
      addWallIfExists(mid,bot,southWall,I_HORIZONTAL_WALL,I_HORIZONTAL_BLANK);
      addWallIfExists(bot,bot,southWall||eastWall,I_CORNER_WALL,I_CORNER_BLANK);
   }
}

