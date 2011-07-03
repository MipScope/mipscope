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
#include <QString>

MazeGui::MazeGui(Gui *gui, MazePlugin *plugin) : 
   QWidget(), MazeUi(), m_gui(gui), m_plugin(plugin)
{
//   cerr << "con: " << QThread::currentThreadId() << endl;
}

MazeGui::~MazeGui() { }



bool MazeGui::loadFile ()
{
	QString fileName = QFileDialog::getOpenFileName(
				m_gui,
				tr("Choose a Maze file to open"),
				m_plugin->getMazePath(),
				tr("Maze files (*.mze);;All files (*)"));
   
	if (fileName.isEmpty())
		return false;
	
	m_plugin->setMazePath(fileName.left(fileName.lastIndexOf(QString("/"))));

	load_maze_from_file(fileName.toStdString());

	return true;
}

void MazeGui::setupUI() {
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
   onNodesVisitedChanged();
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

void MazeGui::onNodesVisitedChanged() {
   m_noVisited.setText(QString("Nodes Visited: %1").arg(nodesVisited));
}

void MazeGui::showMessage(QString title, QString message) {
	QMessageBox::information(this, title, message);
}


