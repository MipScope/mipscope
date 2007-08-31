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
   file: Maze.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007

   This plugin was written for the Brown University 
   Department of Computer Science (cs031).
\* ---------------------------------------------- */
#ifndef __MAZE_H__
#define __MAZE_H__

#include <QObject>
//#include <vector>
enum MazeStatus {EMPTY,SEARCHED,CURRENT,BACKTRACKED};
#include "QtCell.H"
//using namespace std;
#include "../../Utilities.H"

class Cell;
class Maze;
class QtCell;
enum {NORTH= 1,SOUTH= 2,EAST= 4,WEST= 8};
typedef int WallSet;
typedef int Direction;

typedef struct {
   int x;
   int y;
} point;

class Cell {

   public:
      Cell(MazeStatus,WallSet);
      Cell();
      Cell& operator=(const Cell&);
      const char *to_s();
      bool hasWall(Direction);
      MazeStatus getMazeStatus(){return status;}
      void setMazeStatus(MazeStatus s){
         
         //cerr<<"oldSt: " << status << " vs " << s << "  (old: " << oldStatus << ")\n";
         
         //if (s==SEARCHED)
         //   oldStatus=CURRENT;
         //else 
         oldStatus=status;
         status = s; if (m_parent!=NULL) m_parent->updateCell(); }
      MazeStatus getOldMazeStatus() { return oldStatus; }
      void setOldStatus(MazeStatus s) { oldStatus = s; }
      bool isNode(){return node;}
      bool isDeadEnd(){return deadEnd;}
   
      QtCell *m_parent;
   protected:
      MazeStatus status, oldStatus;
      bool northWall;
      bool southWall;
      bool eastWall;
      bool westWall;
      bool node;
      bool deadEnd;
};

class MazeGui;
class Maze {
   public:
      Maze(int,int);
      Maze() : nodesVisited(0) {}
      void init(int,int);
      int getSource(){return sourceCol;}
      void setSource(int col){
         sourceCol = col; 
         currCell.x = col;
         currCell.y = getHeight() - 1;
      }
      int getDest(){return destCol;}
      void setDest(int col){destCol = col;}
      Cell *getCell(int row, int col){return m_grid[row * m_width + col];}
      void setCell(int row, int col, Cell *cell){m_grid[row * m_width + col] = cell;}
      Cell *getCurrentCell(){return getCell(currCell.y, currCell.x); }
      point getCurrCellLoc(){return currCell;}
      const char *to_s();
      int getWidth(){return m_width; }
      int getHeight(){return m_height; }

      Cell *get(int i, int j) { return getCell(i, j); }

      int moveCurrent(point loc);
      int undoMove(point loc);
      point getMazeSource();
      int isGoal(point);
      point roomAt(point,Direction);
      int isSearched(point);
      bool validRoom(const point& p);

   protected:
      // array of cells
      Cell **m_grid;
      //vector<vector<Cell> > grid;
      point currCell;
      int sourceCol;
      int destCol;
      int nodesVisited;
      int m_width, m_height;
      MazeGui *m_mazeGui;
};

#endif // __MAZE_H__

