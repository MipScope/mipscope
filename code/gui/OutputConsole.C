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
#include <QtGui>

// Pseudo-Output terminal, capable of undoing/redoing output
// ---------------------------------------------------------
OutputConsole::OutputConsole(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("Output Console"), gui), m_gui(gui), m_editorPane(editorPane)
{
   setObjectName(tr("OutputConsole"));

   m_display = new QTextEdit(this);
   m_display->setReadOnly(true);
   m_display->setAcceptRichText(false);
   m_display->setFont(*m_editorPane->font());
   connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   
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

