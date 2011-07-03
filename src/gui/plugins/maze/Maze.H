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
#include <vector>
#include <map>
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

struct point {
	int x;
	int y;

	point () { x = y = -1; }
	point (int arg_x, int arg_y) : x(arg_x), y(arg_y) { }

	bool operator< (const point& other) const
	{
		if (x < other.x) return true;
		if (x > other.x) return false;
		return y < other.y;
	}

	bool operator== (const point& other) const { return x == other.x && y == other.y; }
};

class Cell {
public:
	enum {
		NORMAL = 0,
		TREASURE = 1,
		TRAP = 2,
		GOAL = 3
	};
	static int	parse_type_string (const char*);

	// return the necessary number of arguments (see Info struct below) for the given type
	static unsigned int type_nbr_arguments (int);

	struct Info {
		int				type;
		WallSet				walls;
		std::vector<std::string>	arguments;

		Info () { walls = 0; type = NORMAL; }
	};

	static Cell*	new_cell (MazeStatus, const Info&);



      Cell(MazeStatus,WallSet);
      Cell();
      Cell& operator=(const Cell&);
      virtual ~Cell () { }
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

      virtual I_ORDER get_icon () const = 0; // returns the icon for the cell
      virtual int get_room_type () const = 0; // returns the integer ID as used in room data 
      virtual void get_room_data (std::vector<int>&) const = 0; // puts the room data for this cell at the end of the given array (not including type ID and neighbors)
   
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

class NormalCell : public Cell {
public:
      NormalCell(MazeStatus ms ,WallSet ws) : Cell(ms, ws) { }
      NormalCell() { }

      virtual I_ORDER get_icon () const { return I_EMPTY_GROUND; }
      virtual int get_room_type () const { return NORMAL; }
      virtual void get_room_data (std::vector<int>&) const { }
};

class TreasureCell : public Cell {
      int money;
public:
      TreasureCell(MazeStatus ms ,WallSet ws, int arg_money) : Cell(ms, ws), money(arg_money) { }
      TreasureCell() { money = 0; }

      virtual I_ORDER get_icon () const { return I_MONEY; }
      virtual int get_room_type () const { return TREASURE; }
      virtual void get_room_data (std::vector<int>& data) const { data.push_back(money); }
};

class TrapCell : public Cell {
public:
      TrapCell(MazeStatus ms ,WallSet ws) : Cell(ms, ws) { }
      TrapCell() { }

      virtual I_ORDER get_icon () const { return I_SCYTHE; }
      virtual int get_room_type () const { return TRAP; }
      virtual void get_room_data (std::vector<int>&) const { }
};

class GoalCell : public Cell {
public:
      GoalCell(MazeStatus ms ,WallSet ws) : Cell(ms, ws) { }
      GoalCell() { }

      virtual I_ORDER get_icon () const { return I_FLAG; }
      virtual int get_room_type () const { return GOAL; }
      virtual void get_room_data (std::vector<int>&) const { }
};

class MazeGui;

class Maze {
public:
	struct Info {
		unsigned int			width;
		unsigned int			height;
		point				start_point;
		std::map<point, Cell::Info>	cells;

		Info () { width = height = 0; }
	};

      Maze(int,int);
      Maze() : nodesVisited(0) {}
      void init(int,int);
      void init(const Info&);
      int getSource () const { return sourceCell.x; } // DEPRECIATED
      void setSource (int col) { setSource(point(col, getHeight()-1)); } // DEPRECIATED
      void setSource(point cell){
         sourceCell = cell; 
	 currCell = cell;
      }
      Cell *getCell(int row, int col){return m_grid[row * m_width + col];}
      void setCell(int row, int col, Cell *cell){m_grid[row * m_width + col] = cell;}
      void setCell (point p, Cell* cell) { setCell(p.y, p.x, cell); }
      Cell *getCurrentCell(){return getCell(currCell.y, currCell.x); }
      point getCurrCellLoc(){return currCell;}
      const char *to_s();
      int getWidth(){return m_width; }
      int getHeight(){return m_height; }

      Cell *get(int i, int j) { return getCell(i, j); }

      int moveCurrent(point loc);
      int undoMove(point loc);
      point getMazeSource() { return sourceCell; }
      bool isGoal(point); // depreciated
      point roomAt(point,Direction);
      int isSearched(point);
      bool validRoom(const point& p);

      void load_maze_from_file (const std::string& filename);

   protected:
		
		virtual void onNodesVisitedChanged();
		
      // array of cells
      Cell **m_grid;
      //vector<vector<Cell> > grid;
      point currCell;
      point sourceCell;
      int nodesVisited;
      int m_width, m_height;
};

#endif // __MAZE_H__

