/* ---------------------------------------------- *\
   file: Console.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#include "Console.H"
#include "EditorPane.H"
#include "Gui.H"
#include <QDockWidget>
#include <QTextEdit>
#include <QFont>

// Pseudo-Output terminal, capable of printing/undoing output
// ----------------------------------------------------------
Console::Console(Gui *gui, EditorPane *editorPane, const QString &name, bool createDisplay)
   : QDockWidget(name, gui), m_gui(gui), m_editorPane(editorPane), m_display(NULL)
{
   QDockWidget::setObjectName(name);
   
   if (createDisplay) {
      m_display = new QTextEdit(this);
      m_display->setReadOnly(true);
      m_display->setAcceptRichText(false);
      m_display->setFont(*m_editorPane->font());
      
      setWidget(m_display);
   }

   QDockWidget::connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
}

Console::~Console() { }

void Console::flush() {
   m_strings.clear();

   updateDisplay();
}

void Console::fontChanged(const QFont &newFont) {
   m_display->setFont(newFont);
}

void Console::updateDisplay() {
   m_display->setPlainText(QStringList(m_strings.toList()).join(QString("")));
}

