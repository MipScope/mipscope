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
   file: main.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */

#include <iostream>
#include <vector>
#include <cstring>

#include <QApplication>
#include <QTextDocument>
#include <QFile>
#include <QString>

#include "Gui.H"
#include "TextGui.H"
#include "../simulator/Parser.H"
#include "../simulator/State.H"
#include "../simulator/Statement.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Debugger.H"

bool VERBOSE = false;
int main(int argc, char** argv) {
   bool nox = false;
   //Disable X if -nox specified
   for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i],"-nox") == 0) nox = true;
   }
   QApplication app(argc, argv, !nox);
   Statement::InitializeStatementMaps();
   
   int index;
   QStringList args = app.arguments();
   
   /* handle verbosity */
   if ( (index = args.indexOf("-v")) != -1 ) {
         args.removeAt(index);
         VERBOSE = true;
         cout << "Verbosity on.\n";
   }
   
   if ( (index = args.indexOf("-nox")) != -1 ) {
      // run in the command line
      args.removeAt(index); 
      
      if (args.size() < 2) {
         cerr << "Usage: " << args.at(0).toStdString() << " textFile" << endl;
         return 1;
      }
            
      try {
         TextGui ui(args);
         ui.executeProgram(&app);
      } catch(std::vector<ParseError, std::allocator<ParseError> > &e) {
         cerr << "Parse errors:" << endl;
         for (std::vector<ParseError, std::allocator<ParseError> >::const_iterator iter = e.begin();
              iter != e.end();
              iter++) {
            cerr << "   [" << iter->getLineNo()+1 << "] " << iter->toStdString() << endl;
         }
      } 
   }
   else {
      Q_INIT_RESOURCE(images);
      Q_INIT_RESOURCE(icons);
      
      // use the full GUI
      Gui ui(args);
      return app.exec();
   }

   
}

