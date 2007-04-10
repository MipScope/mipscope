/* ---------------------------------------------- *\
   file: main.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include <QApplication>
#include "Gui.H"

int main(int argc, char** argv) {
   QApplication app(argc, argv);
   Gui gui(argc, argv);

   return app.exec();
}

