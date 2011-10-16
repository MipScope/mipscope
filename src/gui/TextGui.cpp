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

#include <QtGui>

#include <stdlib.h>

#include "TextGui.H"
#include "OutputConsole.H"
#include "../simulator/Parser.H"
#include "../simulator/State.H"
#include "../simulator/Statement.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Debugger.H"
#include "plugins/PluginHandler.H"

#include <iostream>

extern bool VERBOSE;

TextGui::TextGui(QStringList args)
: m_debugger(NULL),
  m_syscallListener(NULL),
  m_console(std::cin, std::cout, std::cerr),
  m_spim_syscalls(&m_console, &m_syscallListener)
{
   
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
	
	
	setupPlugins();
   
   if (VERBOSE) cerr << "Executing the program.\n";
   


   // TIM, the NULL in the Debugger's constructor should be an instance of SyscallListener.
   // No more connections since it's all happening in the debugger thread.
   // Good news is it should be really easy to subclass SycallHandler and register it w/ 
   // an instance of SyscallListener to get the same functionality (when we have more time...)
   
   // ... later, never mind, it should work no; just don't need the signals/slots

   
   m_debugger = new Debugger(&m_syscallListener, parseList);
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
   m_syscallListener.syscall(m_debugger->getState(), RUNNING, no, valueOfa0);
}

// slot
void TextGui::programTerminated(int reason) {
   
   if (!m_debugger->wait(PROGRAM_TIMEOUT * 1000)) {
      cerr << "\nExecution timed-out.\n";
      exit(1);
   }
   
   exit(0);
}

void TextGui::setupPlugins() {
   new PluginHandler(this);
}

SyscallListener *TextGui::getSyscallListener() {
	return &m_syscallListener;
}
void TextGui::stopCurrentProgram() {
	m_debugger->programStop();
}
