#include "Parser.H"
#include "State.H"
#include "Statement.H"
#include "ParseNode.H"
#include <iostream>
using namespace std;

#include <QTextDocument>
#include <QFile>
#include <QString>

bool VERBOSE;
int main(int argc, char** argv) {
   QTextDocument p();
   VERBOSE = false;
   
   if (argc < 2) {
      cerr << "Usage: " << argv[0] << " textFile" << endl;
      return 1;
   }
   
   if (argc >= 3)
      VERBOSE = (bool)atoi(argv[2]);
   
   QFile *f = new QFile(QString(argv[1]));
   if (!f->exists()) {
      cerr << "error, cannot find file " << argv[1] << endl;
      return 1;
   }
   
   if (!f->open(QIODevice::ReadOnly | QFile::Text)) {
      cerr << "error opening file " << argv[1] << endl;
      return 1;
   }
   
   QTextDocument *doc = new QTextDocument();
   
   doc->setPlainText(f->readAll());
   f->close();

   cerr << "Parsing file: " << f->fileName().toStdString() << endl;
   cerr << "No Lines: " << doc->blockCount() << endl << endl;
   
   // initialize Instruction and Directive maps
   Statement::InitializeStatementMaps();
   ParseList* parseList = Parser::parseDocument(doc);
   
   return 0;
}

