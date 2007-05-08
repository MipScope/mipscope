#include <QtGui>

#include <stdlib.h>

#include "TextGui.H"
#include "OutputConsole.H"
#include "../simulator/Parser.H"
#include "../simulator/State.H"
#include "../simulator/Statement.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Debugger.H"

#include <iostream>

extern bool VERBOSE;

TextGui::TextGui(QStringList args) : m_debugger(NULL), m_syscallListener(new SyscallListener(NULL)), m_output( new TextOutputConsole(m_syscallListener)) {
   
   QString filename = args.at(1);
      
   QFile *f = new QFile(filename);
   if (!f->exists()) {
      cerr << "error, cannot find file " << filename.toStdString() << endl;
      exit(1);
   }
   
   if (!f->open(QIODevice::ReadOnly | QFile::Text)) {
      cerr << "error opening file " << filename.toStdString() << endl;
      exit(1);
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
   


   // TIM, the NULL in the Debugger's constructor should be an instance of SyscallListener.
   // No more connections since it's all happening in the debugger thread.
   // Good news is it should be really easy to subclass SycallHandler and register it w/ 
   // an instance of SyscallListener to get the same functionality (when we have more time...)


   
   m_debugger = new Debugger(NULL, parseList);
//   QObject::connect(m_debugger->getState(), SIGNAL(syscall(int, int)), this, SLOT(syscallReceived(int,int)));
   QObject::connect(m_debugger, SIGNAL(programTerminated(int)), this, SLOT(programTerminated(int)));
}

// blocks while the program executes
void TextGui::executeProgram(QApplication* app) {
   
   m_debugger->programRun();
   
   app->exec();
   
}

// slot
void TextGui::syscallReceived(int no, int valueOfa0) {
   m_syscallListener->syscall(m_debugger->getState(), RUNNING, no, valueOfa0);
}

// slot
void TextGui::programTerminated(int reason) {
   
   if (m_debugger->wait(PROGRAM_TIMEOUT * 1000)) {
      cout << "\nProgram terminated normally.\n";
   }
   else {
      cout << "\nExecution timed-out.\n";
   }
   
   exit(0);
}
