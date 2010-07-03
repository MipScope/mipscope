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
   file: OutputConsole.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#include "OutputConsole.H"
#include "Utilities.H"
#include "EditorPane.H"
#include "Gui.H"
#include "../simulator/State.H"
#include <QtGui>

// Pseudo-Output terminal, capable of printing/undoing output
// ----------------------------------------------------------
OutputConsole::OutputConsole(Gui *gui, EditorPane *editorPane)
   : Console(gui, editorPane, "Output Console"), 
     SyscallHandler(gui->getSyscallListener(), S_PRINT), 
     m_outputHelper(new OutputHelper(this)), m_noUpdates(0)
{
   // initial loadup message displayed in output console
   m_display->setPlainText("Welcome to "PROJECT_NAME" version "PROJECT_VERSION"\n"
	"Copyright (C) 2007 Travis Fischer and Tim O'Donnell\n"
	PROJECT_NAME" comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome "
	"to redistribute it under certain conditions.\nSee the file GPL.txt in the source package for details.");
}

void OutputConsole::push(const QString &newOutput) {
   // TODO:  OPTIMIZE this!!  Bottleneck!  (infinite-loop printout example)
   m_strings.append(newOutput);
/*   QTextCursor c = m_display->textCursor();
   c.movePosition(QTextCursor::End);
   c.insertText(newOutput);*/
   
   //updateDisplay();
}

void OutputConsole::update() {
   updateDisplay();
   
   if (!m_visible)
      m_gui->ensureVisibility(this);
}

void OutputConsole::pop() {
   m_strings.pop_back();
   
   ++m_noUpdates;
   updateBatchSignal();
//   update();
}

void OutputConsole::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   QString output;
   
   //cerr << "syscallNo: " << syscallNo << ", status: " << status << ", a0 = " << valueOfa0 << endl;
   
   switch(syscallNo) {
      case S_PRINT_INT:
         output = QString::number(valueOfa0);
         break;
      case S_PRINT_STRING:
         output = QString(s->getString(valueOfa0));
         //cerr << "syscall print string: '" << output.toStdString() << "'  (status=" << status << endl;

         break;
      case S_PRINT_CHAR:
         {
            unsigned char str[4];
            str[0] = (unsigned char)(valueOfa0 & 0xFF);
            str[1] = '\0';
            output = QString::fromAscii((const char*)str);
         }
         break;
      case S_CLEAR_OUTPUT:
         m_outputActions.push_back(new ClearOutputAction(this, m_strings));
         break;
      case S_PRINT_FLOAT:  // Unimplemented
      case S_PRINT_DOUBLE:
      default:
         return;
   }
   
   m_outputActions.push_back(new NewOutputAction(this, output, syscallNo));
   
   if (status == PAUSED) {
      ++m_noUpdates;
      updateBatchSignal();
   }
}

// should always be called in Gui thread
void OutputConsole::updateBatch() {
   if (--m_noUpdates <= 0) {
      updateDisplay();
      m_noUpdates = 0;
      
      if (!m_visible)
         m_gui->ensureVisibility(this);
   }
}

void OutputConsole::undoSyscall(int syscallNo) {
   if (!m_outputActions.isEmpty() && 
      (syscallNo == S_PRINT_CHAR || syscallNo == S_PRINT_INT || 
       syscallNo == S_PRINT_STRING)) {
      OutputAction *a = m_outputActions.back();
      if (a == NULL)
         return;
      
      a->undo(this);
      m_outputActions.pop_back();
      delete a;
   }
}

void OutputConsole::reset() {
   flush();
   m_noUpdates = 0;

   foreach(OutputAction *a, m_outputActions) {
      if (a != NULL)
         delete a;
   }

   m_outputActions.clear();
}

void OutputConsole::setStrings(QVector<QString> *strings) {
   m_strings = QVector<QString>(*strings);
}

/* used in command-line mode */
TextOutputConsole::TextOutputConsole(SyscallListener* listener) :
     SyscallHandler(listener, S_PRINT, false) // false=we don't handle undo
{
	cout << PROJECT_NAME" version "PROJECT_VERSION"\nCopyright (C) 2007 Travis Fischer and Tim O'Donnell\n\r\n";

}

void TextOutputConsole::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   QString output;
   
   //cerr << "syscallNo: " << syscallNo << ", status: " << status << ", a0 = " << valueOfa0 << endl;
   
   switch(syscallNo) {
      case S_PRINT_INT:
         output = QString::number(valueOfa0);
         break;
      case S_PRINT_STRING:
         output = QString(s->getString(valueOfa0));
         break;
      case S_PRINT_CHAR:
        {
            unsigned char str[4];
            str[0] = (unsigned char)(valueOfa0 & 0xFF);
            str[1] = '\0';
            output = QString::fromAscii((const char*)str);
         }
         break;
      case S_CLEAR_OUTPUT:
      case S_PRINT_FLOAT:  // Unimplemented
      case S_PRINT_DOUBLE:
      default:
         return;
   }
   
   cout << output.toStdString();
}

// unimplemented:
void TextOutputConsole::undoSyscall(int) {}
void TextOutputConsole::reset() {}



OutputAction::OutputAction(int syscallNo) : m_syscallNo(syscallNo) { }
OutputAction::~OutputAction() { }

NewOutputAction::NewOutputAction(OutputConsole *out, const QString &s, int syscallNo) 
   : OutputAction(syscallNo), m_string(s)
{
   out->push(s);
}

void NewOutputAction::undo(OutputConsole *output) {
   output->pop();
}

ClearOutputAction::ClearOutputAction(OutputConsole *out, QVector<QString> strings) 
   : OutputAction(S_CLEAR_OUTPUT), m_strings(new QVector<QString>(strings))
{
   out->flush();
}

ClearOutputAction::~ClearOutputAction() {
   safeDelete(m_strings);
}

void ClearOutputAction::undo(OutputConsole *output) {
   output->setStrings(m_strings);
}



OutputHelper::OutputHelper(OutputConsole *outputConsole) : QObject(outputConsole), 
   m_output(outputConsole)
{
   connect(m_output, SIGNAL(updateBatchSignal()), this, SLOT(updateBatch()));
}

// should always be called in Gui thread
void OutputHelper::updateBatch() {
//   cerr << "OutputHelper::updateBatch\n";
   m_output->updateBatch();
}


InputSyscallHandler::InputSyscallHandler(Gui *gui, SyscallListener *listener)
   : SyscallHandler(listener, S_READ_INT, true, true), m_gui(gui)
{
   m_syscallNo = S_READ_CHAR;
   listener->registerHandler(this);
   m_syscallNo = S_READ_STRING;
   listener->registerHandler(this);
   m_syscallNo = S_READ_FLOAT;
   listener->registerHandler(this);
   m_syscallNo = S_READ_DOUBLE;
   listener->registerHandler(this);
}

void InputSyscallHandler::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   bool okay = false;
   int max   = 2147483647; // max signed int as defined by Qt
   int min   = -max;
   
   QString input;
   
   switch(syscallNo) {
      case S_READ_INT:
         input = "Input integer:";
         break;
      case S_READ_FLOAT:
         input = "Input float:";
         break;
      case S_READ_DOUBLE:
         input = "Input double:";
         break;
      case S_READ_CHAR:
         input = "Input character:";
         max = 256;
         min = -127;
         break;
      case S_READ_STRING:
         input = "Input string:";
         break;
      default:
         break;
   }
   
   m_gui->getOutputConsole()->update();

   if (syscallNo == S_READ_INT || syscallNo == S_READ_CHAR) {
      int result = QInputDialog::getInteger(NULL, "Syscall", input, 0, min, max, 1, &okay);

      if (!okay)
         result = 0;

      s->setRegister((syscallNo == S_READ_CHAR ? a0 : v0), result);
   } else if (syscallNo == S_READ_FLOAT || syscallNo == S_READ_DOUBLE) {
      double result = QInputDialog::getDouble(NULL, "Syscall", input, 0, min, max, 1, &okay);

      if (!okay)
         result = 0;

//      s->setRegister(, result);
   } else { // syscallNo == S_READ_STRING
      QString result = QInputDialog::getText(NULL, "Syscall", input);
      
      // a0 holds address of buffer
      // a1 holds maximum length
      
      if (s->getRegister(a1) <= (unsigned)result.length())
         result = result.left(s->getRegister(a1) - 1);
      
      const QByteArray &data = result.toAscii();
      s->memcpy(valueOfa0, data.constData(), data.size());
   }
}

void InputSyscallHandler::undoSyscall(int syscallNo) { Q_UNUSED(syscallNo); }
void InputSyscallHandler::reset() { }

