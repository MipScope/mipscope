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
   file: Maze.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim, drapp
   date: 8/14/2007
\* ---------------------------------------------- */
#include "Maze.H"
#include "../../Utilities.H"
#include "../../Gui.H"
#include "../../../simulator/State.H"
#include "QtCell.H"
#include "MazeGui.H"
#include "MazeParser.H"
#include <string.h>
#include <stdio.h>

// Windows does not have strcasecmp
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

Cell::Cell(MazeStatus status,WallSet walls){
   this->status = status;
   this->oldStatus=EMPTY;
   //We extract the walls bitwise from
   //the walls argument
   northWall = walls & NORTH;
   southWall = walls & SOUTH;
   eastWall  = walls & EAST;
   westWall  = walls & WEST;
   int numWalls = 0;
   if(northWall) numWalls++;
   if(southWall) numWalls++;
   if(eastWall) numWalls++;
   if(westWall) numWalls++;
   node = numWalls < 2;
   deadEnd = numWalls > 2;
   m_parent = NULL;
}

Cell::Cell(){
   //An empty uninteresting Cell
   status = EMPTY;
   oldStatus = EMPTY;
   northWall = 0;
   southWall = 0;
   eastWall = 0;
   westWall = 0;
   node = 0;
   deadEnd = 0;
   m_parent = NULL;
}

/*
 * just for testing
 */
const char * Cell::to_s(){
   //Print out the wall and status
   //information in an awkward form
   string toReturn = "";
   if(westWall) toReturn += "|";
   if(southWall) toReturn += "_";
   switch(status){
      case EMPTY    : toReturn += "Empty"; break;
      case SEARCHED : toReturn += "Searched"; break;
      case CURRENT  : toReturn += "Current"; break;
      case BACKTRACKED:toReturn+= "Backtracked"; break;
   }
   if(northWall) toReturn += "-";
   if(eastWall) toReturn += "| ";
   return toReturn.c_str();
}

/*
 * returns if there's a wall in the given direction
 */
bool Cell::hasWall(Direction where){
   switch(where){
      case NORTH : return northWall;
      case SOUTH : return southWall;
      case EAST  : return eastWall;
      case WEST  : return westWall;
      default    : return 0;
   }
}


Cell &Cell::operator=(const Cell &other){
   status = other.status;
   oldStatus = other.oldStatus;
   northWall = other.northWall;
   southWall = other.southWall;
   eastWall = other.eastWall;
   westWall = other.westWall;
   node = other.node;
   deadEnd = other.deadEnd;

   if (m_parent != NULL)
      m_parent->updateCell();
   return *this;
}

/*
 * Create a maze, which is a 2D vector of Cells,
 * along with info about source and destination
 */
Maze::Maze(int width, int height) : m_grid(NULL) {
   init(width, height);
}

void Maze::init(int width, int height) {
/*   const vector<Cell> row(width);
   for(unsigned int i = 0; i < grid.size(); i++){
      //There ought to be a better way
      //to do this, but nothing else
      //worked
      grid[i] = row;
   }*/
   //These aren't set in this constructor
   nodesVisited = 1;
   m_width = width;
   m_height = height;

   m_grid = new Cell*[m_width * m_height+1];
   int i;
   for(i = m_width * m_height; i--;)
      m_grid[i] = NULL;
   
   //cerr << "Maze::init(" << getWidth() << ", " << getHeight() << ")\n";
}

void Maze::init (const Maze::Info& info)
{
	init(info.width, info.height);
	setSource(info.start_point);

	for (unsigned int x = 0; x < info.width; ++x) {
		for (unsigned int y = 0; y < info.height; ++y) {
			point	p(x, y);
			Cell*	cell = 0;

			std::map<point, Cell::Info>::const_iterator it(info.cells.find(p));
			if (it != info.cells.end()) {
				cell = Cell::new_cell(it->first == info.start_point ? CURRENT : EMPTY, it->second);
			}

			if (cell)
				setCell(p, cell);
			else
				setCell(p, new NormalCell);
		}
	}
}


#define exitWithPopup(str,e) { cerr << str << endl; return (e); }

/*
 * moves the little guy to the right point
 */
int Maze::moveCurrent(point loc) {
   if (!validRoom(loc))
      exitWithPopup("You passed an invalid cell id to draw_arrow",0);
   
   //cerr << "moveCurrent: " <<  currCell.y << ", " << currCell.x;
//   cerr << ", (" << getCurrentCell() << ", " << getCell(loc.y, loc.x) << ")\n";

   getCurrentCell()->setMazeStatus(SEARCHED);
//   cerr << "moveCurrent: " << getCurrentCell()->getMazeStatus() << endl;
//   m_grid[currCell.y][currCell.x].setMazeStatus(SEARCHED);
   getCell(loc.y, loc.x)->setMazeStatus(CURRENT);
//   m_grid[loc.y][loc.x].setMazeStatus(CURRENT);
   currCell = loc;
   ++nodesVisited;
   
   onNodesVisitedChanged();
//   cerr << ";moveCurrent\n";
   // TODO:  update m_noVisited counter

   return 1;
}

int Maze::undoMove(point loc) {
   if (!validRoom(loc))
      return 0;
   
   // TODO:  possibly not just empty, but last maze status before moveCurrent()
//   cerr << "currCell: " << currCell.y << ", " << currCell.x << endl;
 //  cerr << "undo: " << (currCell.x==loc.x&&currCell.y==loc.y);

   getCurrentCell()->setMazeStatus(getCurrentCell()->getOldMazeStatus());
   getCell(loc.y, loc.x)->setMazeStatus(CURRENT);
//   grid[currCell.y][currCell.x].setMazeStatus(EMPTY);
//   grid[loc.y][loc.x].setMazeStatus(CURRENT);
   currCell = loc;
   --nodesVisited;

  // cerr << "End-currCell: " << currCell.x << ", " << currCell.y << endl;
	onNodesVisitedChanged();
   // TODO:  update m_noVisited counter
   
   return 1;
}

bool Maze::isGoal(point loc) // DEPRECIATED - kept around for backwards compatibility
{
	if(!validRoom(loc))
		exitWithPopup("You passed an invalid cell id to is_goal",0);

	return dynamic_cast<GoalCell*>(getCell(loc.y, loc.x));
}

/*
 * Returns the room in the given direction, taking into account walls and
 * the edge of the maze. If a room is successfully returned it will be marked
 * so the solver knows it's been looked at
 */
point Maze::roomAt(point curr,Direction dir){
   point nowhere(-1,-1);

   if (!validRoom(curr))
     exitWithPopup("You passed an invalid cell id to get_neighbors",nowhere);
   
   if(getCell(curr.y, curr.x)->hasWall(dir)){
      return nowhere;
   }else{
      point end=curr;
      switch(dir){
         case NORTH : end.y--; break;
         case SOUTH : end.y++; break;
         case EAST  : end.x++; break;
         case WEST  : end.x--; break;
         default    : return nowhere;
      }
      if(validRoom(end)) return end;
      else return nowhere;
   }
}

/*
 * Have we been here before? returns essentially if the
 * specified square is now purple
 */
int Maze::isSearched(point loc){
	if(!validRoom(loc))
      exitWithPopup("You passed an invalid cell id to is_searched",0);

	return (getCell(loc.y, loc.x)->getMazeStatus() == SEARCHED);
}

/*
 * Check if the cell index is within the bounds of the maze
 */
bool Maze::validRoom(const point& p) {
   return  (p.x >= 0 && p.x < m_width &&
      p.y >= 0 && p.y < m_height);
}

void Maze::onNodesVisitedChanged() {}

int	Cell::parse_type_string (const char* s)
{
	if (strcasecmp(s, "treasure") == 0)
		return TREASURE;
	if (strcasecmp(s, "trap") == 0)
		return TRAP;
	if (strcasecmp(s, "goal") == 0)
		return GOAL;
	if (strcasecmp(s, "normal") == 0 || strcasecmp(s, "cell") == 0 || strcasecmp(s, "room") == 0)
		return NORMAL;
	return -1;
}

Cell*	Cell::new_cell (MazeStatus status, const Info& info)
{
	if (info.arguments.size() != type_nbr_arguments(info.type))
		return 0;

	switch (info.type) {
	case NORMAL:
		return new NormalCell(status, info.walls);
	case TREASURE:
		return new TreasureCell(status, info.walls, atoi(info.arguments[0].c_str()));
	case TRAP:
		return new TrapCell(status, info.walls);
	case GOAL:
		return new GoalCell(status, info.walls);
	}
	return 0;
}

unsigned int Cell::type_nbr_arguments (int t)
{
	switch (t) {
	case NORMAL:	return 0;
	case TREASURE:	return 1;
	case TRAP:	return 0;
	case GOAL:	return 0;
	}
	return 0;
}

void Maze::load_maze_from_file (const std::string& filename)
{
	Info info;
	MazeParser::parse(filename, info);
	init(info);
}

