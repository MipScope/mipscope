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
      cout << "Welcome to MipScope.\n"; 
      
      
      if (args.size() < 2) {
         cerr << "Usage: " << args.at(0).toStdString() << " textFile" << endl;
         return 1;
      }
            
      QString filename = args.at(1);
      
      QFile *f = new QFile(filename);
      if (!f->exists()) {
         cerr << "error, cannot find file " << filename.toStdString() << endl;
         return 1;
      }
      
      if (!f->open(QIODevice::ReadOnly | QFile::Text)) {
         cerr << "error opening file " << filename.toStdString() << endl;
         return 1;
      }
      
      QTextDocument p();
      QTextDocument *doc = new QTextDocument();
      
      doc->setPlainText(f->readAll());
      f->close();
      
      if (VERBOSE) {
         cerr << "Parsing file: " << f->fileName().toStdString() << endl;
         cerr << "No Lines: " << doc->blockCount() << endl << endl;
      }
      
      ParseList* parseList = Parser::parseDocument(doc);
      
      if (VERBOSE) cerr << "Executing the program.\n";
      
      Debugger debugger(parseList);
      debugger.programRun();
      
      if (debugger.wait(10 * 1000)) { // timeout after 10 seconds.
         cerr << "\nProgram terminated normally.\n";
      }
      else {
         cerr << "\nExecution timed-out.\n";
      }
            
   }
   else {
      // use the full GUI
      Gui gui(argc, argv);
      return app.exec();
   }

   
}

