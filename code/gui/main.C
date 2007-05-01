/* ---------------------------------------------- *\
   file: main.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */

#include <iostream>
using namespace std;

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
   QApplication app(argc, argv);
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
      
      TextGui ui(args);
      ui.executeProgram(&app);
   }
   else {
      // use the full GUI
      Gui ui(args);
      return app.exec();
   }

   
}

