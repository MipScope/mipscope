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
   : QDockWidget(tr("Output Console"), gui), 
     SyscallHandler(gui->getSyscallListener(), S_PRINT), 
   m_gui(gui), m_editorPane(editorPane)
{
   QDockWidget::setObjectName(tr("OutputConsole"));

   m_display = new QTextEdit(this);
   m_display->setReadOnly(true);
   m_display->setAcceptRichText(false);
   m_display->setFont(*m_editorPane->font());
   QDockWidget::connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   
   m_display->setPlainText("Welcome to "PROJECT_NAME"!\nWritten by Travis Fischer and Tim O'Donnell\n\n");

   setWidget(m_display);
}

void OutputConsole::push(const QString &newOutput) {
   m_strings.append(newOutput);
   
   updateDisplay();
}

void OutputConsole::pop() {
   m_strings.pop_back();
   
   updateDisplay();
}

void OutputConsole::flush() {
   m_strings.clear();

   updateDisplay();
}

void OutputConsole::fontChanged(const QFont &newFont) {
   m_display->setFont(newFont);
}

void OutputConsole::updateDisplay() {
   m_display->setPlainText(QStringList(m_strings.toList()).join(QString("")));
}

void OutputConsole::syscall(State *s, int status, int syscallNo) {
   QString output;
   
   cerr << "syscallNo: " << syscallNo << ", status: " << status << endl;
   
   switch(syscallNo) {
      case S_PRINT_INT:
         output = QString::number(s->getRegister(a0));
         break;
      case S_PRINT_STRING:
         output = QString(s->getString(s->getRegister(a0)));
         break;
      case S_PRINT_CHAR:
         output = QString::number((char)(s->getRegister(a0) & 0xFF));
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
}

