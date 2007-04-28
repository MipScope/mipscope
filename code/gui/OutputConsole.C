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
#include <QTextCursor>

// Pseudo-Output terminal, capable of printing/undoing output
// ----------------------------------------------------------
OutputConsole::OutputConsole(Gui *gui, EditorPane *editorPane)
   : Console(gui, editorPane, tr("Output Console")), 
     SyscallHandler(gui->getSyscallListener(), S_PRINT), m_first(true)
{
   m_display->setPlainText("Welcome to "PROJECT_NAME"!\nWritten by Travis Fischer and Tim O'Donnell\n\n");
}

void OutputConsole::push(const QString &newOutput) {
   // TODO:  OPTIMIZE this!!  Bottleneck!  (infinite-loop printout example)
   m_strings.append(newOutput);
   QTextCursor c = m_display->textCursor();
   c.movePosition(QTextCursor::End);
   c.insertText(newOutput);
   
   //updateDisplay();
   if (m_first) {
      m_first = false;
      m_gui->ensureVisibility(this);
   }
}

void OutputConsole::pop() {
   m_strings.pop_back();
   
   updateDisplay();
   m_gui->ensureVisibility(this);
}

void OutputConsole::syscall(State *s, int status, int syscallNo, int valueOfa0) {
   QString output;
   
   cerr << "syscallNo: " << syscallNo << ", status: " << status << ", a0 = " << valueOfa0 << endl;
   
   switch(syscallNo) {
      case S_PRINT_INT:
         output = QString::number(valueOfa0);
         break;
      case S_PRINT_STRING:
         output = QString(s->getString(valueOfa0));
         break;
      case S_PRINT_CHAR:
         output = QString::number((char)(valueOfa0 & 0xFF));
         break;
      case S_PRINT_FLOAT:  // Unimplemented
      case S_PRINT_DOUBLE:
      default:
         return;
   }
   
   push(output);
}

void OutputConsole::undoSyscall(int syscallNo) {
   if (syscallNo == S_PRINT_CHAR || syscallNo == S_PRINT_INT || 
       syscallNo == S_PRINT_STRING)
      pop();
}

void OutputConsole::reset() {
   flush();
   m_first = true;
}

