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
   file: MazePlugin.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#include "MazePlugin.H"
#include "../../Utilities.H"
#include "../../UI.H"
#include "../../Gui.H"
#include "../../SyscallHandler.H"
#include "../../../simulator/State.H"
#include "MazeGui.H"
#include "MazeTui.H"
#include "MazeUi.H"
#include <QtGui>

#ifdef Q_OS_WIN32
// For Sleep()
#include <windows.h>
#endif

// Main interface between MipScope and the Maze gui plugin.
// --------------------------------------------------------
MazePlugin::MazePlugin(UI *ui, bool textOnly) : 
   Plugin(ui), SyscallHandler(ui->getSyscallListener(), S_MAZE_INIT, true, true), 
   m_maze(NULL),
	m_textOnly(textOnly)
{
   SyscallListener *listener = ui->getSyscallListener();

   // Register this plugin as the default handler for all maze syscalls
   m_syscallNo = S_MAZE_DEFEAT;
   listener->registerHandler(this);
   m_syscallNo = S_MAZE_VICTORY;
   listener->registerHandler(this);
   m_syscallNo = S_MAZE_IS_GOAL;
   listener->registerHandler(this);
   m_syscallNo = S_MAZE_NEIGHBOR;
   listener->registerHandler(this);
   m_syscallNo = S_MAZE_DRAW_ARROW;
   listener->registerHandler(this);
   m_syscallNo = S_MAZE_IS_SEARCHED;
   listener->registerHandler(this);
}

void MazePlugin::resetPlugin() {
   reset();
}

namespace {
	inline void maze_error (State* s, const char* str)
	{
		std::cerr << "Maze Error: " << str << std::endl;
		s->breakError(str);
	}
}

void MazePlugin::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   Q_UNUSED(status);

   switch(syscallNo) {
      case S_MAZE_INIT:
         s->setRegister(v0, init_maze(s));
         break;
      case S_MAZE_DEFEAT:
         defeat();
         break;
      case S_MAZE_VICTORY:
         victory();
         break;
      case S_MAZE_IS_GOAL:
         s->setRegister(v0, is_goal(s, valueOfa0));
         break;
      case S_MAZE_NEIGHBOR:
         {
            int results[4];
            get_neighbors(s, valueOfa0, results);
            unsigned int addr = s->getRegister(a1);

            for(int i = 0; i < 4; ++i)
               s->setMemoryWord(addr + (i << 2), results[i]);
         }
         break;
      case S_MAZE_DRAW_ARROW:
         draw_arrow(s, status, valueOfa0, s->getRegister(a1));
         break;
      case S_MAZE_IS_SEARCHED:
         s->setRegister(v0, is_searched(s, valueOfa0));
         break;
      default:
         break;
   }
}

void MazePlugin::undoSyscall(int syscallNo) {
   switch(syscallNo) {
      case S_MAZE_INIT:
         resetPlugin();
         break;
      case S_MAZE_DEFEAT:
      case S_MAZE_VICTORY:
      case S_MAZE_IS_GOAL:
      case S_MAZE_NEIGHBOR:
      case S_MAZE_IS_SEARCHED: // no undo necessary
         break;
      case S_MAZE_DRAW_ARROW:
         undo_draw_arrow();
         break;
      default:
         break;
   }

}

void MazePlugin::reset() {
   
	delete m_maze;
   m_maze = NULL;
   m_searchStack.clear();
}



/* We use half-size integers, storing two of them into one int, to return for the "cell ID" */
static const int halfint_shift = sizeof(int) * 8 / 2;
/**
 * Compact two ints into one. Obviously this only works with small positive ints. We add 1 so that zero can
 * stand for no room
 */
int cellID(point loc){
   const int halfint_shift = sizeof(int) * 8 / 2;
   return ((loc.x << halfint_shift) | loc.y) + 1;
}

/*
 * Extracts the x and y coords from the id. if you were doing this with large numbers you might
 * run into wacky arthithmatic vs logical shift issues, but we should be safe here
 */
point cellLoc(int id) {
   const int lowhalf_mask = (1 << halfint_shift) - 1;
   --id;	// We store it +1 because we can't use cellid 0
   point p = {id >> halfint_shift, id & lowhalf_mask};
   return p;
}

static ostream& operator<< (ostream& s, const point& pt) {
   return s << cellID(pt) << " [room " << pt.x << "," << pt.y << "]";
}


// --------------------------------------
// Maze Interface (accessed via syscalls)
// --------------------------------------

/*
 * spawns the maze GUI in a separate process/thread. waits for it to initialize, then requests the starting room and returns it
 */ 
int MazePlugin::init_maze(State *s) {
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call init_maze()" << endl;

   if (m_maze != NULL)
      maze_error(s, "Attempted to initialize Maze twice");
   
	
	//TODO add text-only option here
   if (m_textOnly) {
		m_maze = new MazeTui(this);
	}
	else {
		m_maze = new MazeGui((Gui*) m_ui, this);
	}
//   QEventLoop *eventLoop = new QEventLoop();
//   eventLoop->exec();

   if (!m_maze->initializeMaze())
      return false;

   point src = m_maze->getMazeSource();

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG init_maze() -> " << src << endl;

   return cellID(src);
}

void printPoint(point *p) {
   cerr << "point: " << p->x << ", " << p->y;
}

/*
 * instructs the GUI to draw the path from the previous (parent) node to this one. Pathsoare never erased
 */ 
void MazePlugin::draw_arrow(State *s, int status, int room, int parentRoom) {
   point roomP = cellLoc(room);
   point parentRoomP = cellLoc(parentRoom);
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call draw_arrow(" << roomP << ", " << parentRoomP << ")" << endl;
   
   if (m_maze == NULL)
      maze_error(s, "MAZE: call to draw_arrow on uninitialized maze");
   
   /*printPoint(&parentRoomP);
   cerr << " -> ";
   printPoint(&roomP);
   cerr << endl;*/

   m_searchStack.push(cellID(m_maze->getCurrCellLoc()));//parentRoom);

   m_maze->moveCurrent(roomP);

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: draw_arrow() -> void" << endl;
   if(getenv("MAZE_SLOW") && status == RUNNING) {
      
		// Loop 1000 times, sleeping for 1000 milliseconds each time,
		// for a net sleep of 1 second.
		// Use system call usleep so we don't need to create our own QThread.
		// Use multiple sleeps so that the GUI still responds while sleeping.
		// (If we used a single sleep of 1 second, the GUI would lock up.)
		for (int i=0; i < 1000; i++) {
#ifdef Q_OS_WIN32
			Sleep(1);
#else
			usleep(1000);
#endif

		}
   }
}

void MazePlugin::undo_draw_arrow() {
   if (m_searchStack.size() < 1)
      return;
   int parentRoom = m_searchStack.pop();

   if (m_maze != NULL)
      m_maze->undoMove(cellLoc(parentRoom));
}

/*
 * returns true if the given room is the goal
 */ 
int MazePlugin::is_goal(State *s, int room){
   point roomP = cellLoc(room);

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call is_goal(" << roomP << ")" << endl;
   
   if (m_maze == NULL)
      maze_error(s, "MAZE: call to is_goal on uninitialized maze");

   int answer = m_maze->isGoal(cellLoc(room));
   
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: is_goal() -> " << answer << endl;

   return answer;

}

int MazePlugin::is_searched(State *s, int room){
   point roomP = cellLoc(room);

   if (m_maze == NULL)
      maze_error(s, "MAZE: call to is_searched on uninitialized maze");

   return m_maze->isSearched(roomP); 
}

/*
 * declares victory (ending the program but perhaps leaving the window open?)
 */ 
void MazePlugin::victory(){
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call victory()" << endl;
   
   showEndMessage("Victory - You Win!");

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: victory() -> void" << endl;
}

/*
 * declares defeat (same deal?)
 */ 
void MazePlugin::defeat(){
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call defeat()" << endl;

   showEndMessage("Defeat - You Lose!");

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: defeat() -> void" << endl;
}

void MazePlugin::showEndMessage(const QString &str) {
	m_maze->showMessage("Pascal's Fate", str);
	reset();
}


/*
 * Returns the accessible neighbors of the room identified by start (accessible meaning there's no wall in the way).
 * Fills buf[0]..buf[3] with the four room IDs for accessible rooms north, west, south, and east respectively. Stores 
 * the value zero in a location if there is no room there or if it is inaccessible from the start room.
 * 
 */ 
void MazePlugin::get_neighbors(State *s, int start, int* buf){
   point curr = cellLoc(start);
   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: call get_neighbors(" << curr << ", <buf>)" << endl;
   
   if (m_maze == NULL)
      maze_error(s, "MAZE: call to get_neighbors on uninitialized maze");

   point northRoom = m_maze->roomAt(curr,NORTH);
   buf[0] = northRoom.x == -1 ? 0 : cellID(northRoom);
   point westRoom = m_maze->roomAt(curr,WEST);
   buf[1] =  westRoom.x == -1 ? 0 : cellID(westRoom);
   point southRoom = m_maze->roomAt(curr,SOUTH);
   buf[2] = southRoom.x == -1 ? 0 : cellID(southRoom);
   point eastRoom = m_maze->roomAt(curr,EAST);
   buf[3] = eastRoom.x == -1 ? 0 : cellID(eastRoom);

   if(getenv("MAZE_DEBUG"))
      cerr << "MAZE_DEBUG: get_neighbors -> {" 
         << northRoom << ", " 
         << westRoom  << ", "
         << southRoom << ", "
         << eastRoom  << ")" 
         << endl;
}

QString  MazePlugin::getMazePath() {
	return m_lastMazeDir.isEmpty() ? QString("./") : m_lastMazeDir;\
}

void MazePlugin::setMazePath(QString path) {
	m_lastMazeDir = path;
}



