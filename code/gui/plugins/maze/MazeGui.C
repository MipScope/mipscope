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
   file: MazeGui.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#include "MazeGui.H"
#include "../../Utilities.H"
#include "../../Gui.H"
#include "MazePlugin.H"
#include "MazeParser.H"
#include "QtCell.H"
#include <QtGui>

MazeGui::MazeGui(Gui *gui, MazePlugin *plugin) : 
   QWidget(), Maze(), m_gui(gui), m_plugin(plugin)
{
//   cerr << "con: " << QThread::currentThreadId() << endl;
}

MazeGui::~MazeGui() { }

bool MazeGui::initializeMaze() {
   bool success = false;

   //cerr << "init: " << QThread::currentThreadId() << endl;
   //cerr << "mine: " << (this->thread() == QThread::currentThread()) << endl;
   if (getenv("MAZE_DEFAULT"))
      success = MazeParser::parse(getenv("MAZE_DEFAULT"), this);
   else success = loadFile();

   if (success)
      setupCells();

   return success;
}

bool MazeGui::loadFile() {
//   cerr << "thread: " << QThread::currentThreadId() << endl;

   const QString &fileName = QFileDialog::getOpenFileName(
         m_gui,
         tr("Choose a Maze file to open"),
         tr("./"),
         tr("Maze files (*.mze);;All files (*)"));
   //const QString &fileName = "test.mze";
   
   if (fileName.isEmpty())
      return false;

   return MazeParser::parse(fileName, this);
}

void MazeGui::setupCells() {
   m_mazeGui = this;
   // holds number of nodes visited text
   QGridLayout *mainLay = new QGridLayout();
   
   m_cellPanel = new QWidget();
   
   QGridLayout *lay = new QGridLayout(m_cellPanel);
   lay->setMargin(0);
   lay->setSpacing(0);
   lay->setSizeConstraint(QLayout::SetFixedSize);

   m_cells = new QtCell*[m_width * m_height];
   for(int i = 0; i < m_height; ++i) {
      for(int j = 0; j < m_width; ++j) {
         QtCell *cur = new QtCell(this, i, j, get(i, j));
         //cerr << "QtCell(" << i << ", " << j << ")\n";

         m_cells[i * m_width + j] = cur;

         lay->addWidget(cur, i, j);
      }
   }
   
   //QLabel *l = new QLabel(QString("Nodes Visited: %1").arg(nodesVisited));
//   m_noVisited = new QLabel(QString("Nodes Visited: %1").arg(nodesVisited));
   updateNodesVisited();
   mainLay->setSizeConstraint(QLayout::SetFixedSize);
   mainLay->addWidget(&m_noVisited, 0, 0, Qt::AlignRight);
   mainLay->addWidget(m_cellPanel, 1, 0, Qt::AlignCenter);
   setLayout(mainLay);
   
   //setMinimumSize(sizeHint());

   setWindowTitle(tr("Maze"));
   setVisible(true);
}

void MazeGui::closeEvent(QCloseEvent *event) {
   m_gui->stopCurrentProgram(); // quit Maze
   
   QWidget::closeEvent(event);
}

void MazeGui::updateNodesVisited() {
   m_noVisited.setText(QString("Nodes Visited: %1").arg(nodesVisited));
}

/*int MazeGui::moveCurrent(point loc) {
   int res = Maze::moveCurrent(loc);

   //m_noVisited->setText(QString("Nodes Visited: ").arg(QString::number(nodesVisited)));
   
   return res;
}

int MazeGui::undoMove(point loc) {
   int res = Maze::undoMove(loc);

//   m_noVisited->setText(QString("Nodes Visited: ").arg(QString::number(nodesVisited)));
   
   return res;
}*/

/*int move(point loc){
	if(!validRoom(loc))
      exitWithPopup("You passed an invalid cell id to draw_arrow");

   grid[currCell.y][currCell.x].setStatus(SEARCHED);
   grid[loc.y][loc.x].setStatus(CURRENT);
   currCell = loc;
   //All this is just for the counter
   nodesVisited++;

   // TODO:  update m_noVisited counter

   //string label = "Nodes visited: ";

   return 1;
}

int isGoal(point loc){
   if(!validRoom(loc))
      exitWithPopup("You passed an invalid cell id to is_goal");

   return isGoal(loc);
}

point roomAt(point curr,Direction dir){
   if(!validRoom(curr))
      exitWithPopup("You passed an invalid cell id to get_neighbors");

   return roomAt(curr,dir);
}

int isSearched(point loc){
   if(!validRoom(loc))
      exitWithPopup("You passed an invalid cell id to is_searched");

   return isSearched(loc);
}*/

