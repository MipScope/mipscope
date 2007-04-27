/* ---------------------------------------------- *\
   file: main.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include <QApplication>
#include "Gui.H"
#include "../simulator/Statement.H"

int main(int argc, char** argv) {
   QApplication app(argc, argv);
   Statement::InitializeStatementMaps();

   Gui gui(argc, argv);

   return app.exec();
}

