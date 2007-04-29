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
      case S_CLEAR_OUTPUT:
         m_outputActions.push_back(new ClearOutputAction(this, m_strings));
         break;
      case S_PRINT_FLOAT:  // Unimplemented
      case S_PRINT_DOUBLE:
      default:
         return;
   }
   
   m_outputActions.push_back(new NewOutputAction(this, output, syscallNo));
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
   m_first = true;

   foreach(OutputAction *a, m_outputActions) {
      if (a != NULL)
         delete a;
   }

   m_outputActions.clear();
}

void OutputConsole::setStrings(QVector<QString> *strings) {
   m_strings = QVector<QString>(*strings);
}

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

