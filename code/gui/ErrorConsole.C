/* ---------------------------------------------- *\
   file: ErrorConsole.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#include "ErrorConsole.H"
#include "EditorPane.H"
#include "../simulator/Parser.H"
#include "Gui.H"
#include "TextEditor.H"
#include <QTextBrowser>
#include <QScrollBar>


// Error terminal for displaying parsing errors and program exceptions
// -------------------------------------------------------------------
ErrorConsole::ErrorConsole(Gui *gui, EditorPane *editorPane)
   : Console(gui, editorPane, tr("Errors"), false), m_activeEditor(NULL)
{
   QTextBrowser *browser = new QTextBrowser(this);
   
   connect(browser, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(anchorClicked(const QUrl&)));

   m_display = browser;
   m_display->setLineWrapMode(QTextEdit::NoWrap);
   setWidget(browser);
}

void ErrorConsole::updateSyntaxErrors(SyntaxErrors *s, TextEditor *editor, bool forceUpdate) {
   if (s == NULL || editor == NULL) {
      reset();
      return;
   }
   
   if (STATUS_BAR != NULL)
      STATUS_BAR->showMessage(QString("Error: Program %1 contains %2 errors.").arg(m_editorPane->m_activeEditor->fileName(), QString::number(s->size())), STATUS_DELAY + 2000);
   m_activeEditor = editor;
   
   //m_strings.clear();
   m_text = QString("<span style=\"font:10pt courier;\">");
   
   for(SyntaxErrorIterator i = s->begin(); i != s->end(); i++) {
      int lineNo = i->getLineNo();
      QString err;
      
      if (lineNo < 0 && i->getTextBlock() != NULL)
         i->setLineNo((lineNo = editor->lineNumber(*i->getTextBlock())));
      
      if (lineNo >= 0)
         err = QString("<a href=\"%1\">%2</a>) %3<br>").arg(QString::number(lineNo), QString::number(lineNo), *i);
      else err = *i + QString("<br>");
      
      m_text += err;
      //m_strings.append(err);
   }
   
   m_text += "</span>";
   m_display->setHtml(m_text);

//   m_display->setHtml(QString("<span style=\"font:10pt courier;\"><a href=\"test\">This is a test link.</a></span>"));
//   updateDisplay();
   
   if (forceUpdate && !m_visible)
      m_gui->ensureVisibility(this);
}

void ErrorConsole::anchorClicked(const QUrl &link) {
   const QString str = link.toString();
   bool ok;
   
   // hack to keep the Browser at the same position after resetting html
   QScrollBar *scroll = m_display->verticalScrollBar();
   int value = scroll->value();
   m_display->setHtml(m_text);
   m_display->verticalScrollBar()->setValue(value);

   int lineNo = str.toInt(&ok);
   if (!ok || lineNo < 0)
      return;
   
   // jump editor to line no specified
   if (m_activeEditor != NULL) {
      m_editorPane->setActiveEditor(m_activeEditor);
      m_activeEditor->gotoLine(lineNo);
   }
}

void ErrorConsole::reset() {
   m_text = "";
   flush();
}

