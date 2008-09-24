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
   file: MazeTui.C
   auth: Andrew Brindamour
   acct: ab
   date: 8/31/2008
\* ---------------------------------------------- */
#include "MazeTui.H"
#include "../../Utilities.H"
#include "MazePlugin.H"
#include "MazeParser.H"

MazeTui::MazeTui(MazePlugin *plugin) : 
   MazeUi(), m_plugin(plugin)
{
//   cerr << "con: " << QThread::currentThreadId() << endl;
}

MazeTui::~MazeTui() { }



bool MazeTui::loadFile() {
	
	
	cout <<endl << "Please enter maze filename: ";
	std::string fileNameStd;
	
	cin >> fileNameStd;
	
   const QString &fileName = QString(fileNameStd.c_str()); //QString("/course/cs031/asgn/maze/test.mze");
   
   if (fileName.isEmpty())
      return false;
	
	m_plugin->setMazePath(fileName.left(fileName.lastIndexOf(QString("/"))));

   return MazeParser::parse(fileName, this);
}

void MazeTui::setupUI() {}

void MazeTui::showMessage(QString title, QString message) {
	cout << title.toStdString() << ": " << message.toStdString() <<endl;
}

