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
   : Console(gui, editorPane, tr("Errors"), false), m_activeEditor(NULL), 
   m_old(NULL)
{
   QTextBrowser *browser = new QTextBrowser(this);
   
   connect(browser, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(anchorClicked(const QUrl&)));

   m_display = browser;
   m_display->setLineWrapMode(QTextEdit::NoWrap);
   setWidget(browser);
}

void ErrorConsole::updateSyntaxErrors(SyntaxErrors *s, TextEditor *editor, bool forceUpdate) {
   if (s == NULL || editor == NULL || m_old == s) {
      reset();
      return;
   }
   
//   bool possiblyTheSame = (m_old != NULL && m_activeEditor == editor && m_old->size() == s->size());
   m_old = s;
   m_activeEditor = editor;
   
   //m_strings.clear();
   QString text = QString("<span style=\"font:10pt courier;\">");
   
   int index = 0;
   for(SyntaxErrorIterator i = s->begin(); i != s->end(); ++i, ++index) {
      int lineNo = i->getLineNo();
      QString err;
      
      if (lineNo < 0 && i->getTextBlock() != NULL)
         i->setLineNo((lineNo = (editor->lineNumber(*i->getTextBlock()))));
      
//      if (possiblyTheSame && m_old->at(index) != *i)
//         possiblyTheSame = false;
      
      if (lineNo >= 0) {
         const QString &lineNumberStr = QString::number(lineNo + 1);

         err = QString("<a href=\"%1\">%2</a>) %3<br>").arg(lineNumberStr, lineNumberStr, *i);
      } else err = *i + QString("<br>");
      
      text += err;
      //m_strings.append(err);
   }
   
   text += "</span>";
   if (/*!possiblyTheSame && */m_text != text) {
      m_text = text;
      m_display->setHtml(m_text);

      if (STATUS_BAR != NULL)
         STATUS_BAR->showMessage(QString("Error: Program %1 contains %2 error%3.").arg(m_editorPane->m_activeEditor->fileName(), QString::number(s->size()), (s->size() > 1 ? "s" : "")));

      //   m_display->setHtml(QString("<span style=\"font:10pt courier;\"><a href=\"test\">This is a test link.</a></span>"));
      //   updateDisplay();

      if (forceUpdate && !m_visible)
         m_gui->ensureVisibility(this);
   }
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
      m_activeEditor->gotoLine(lineNo - 1);
   }
}

void ErrorConsole::reset() {
   if (m_old != NULL) {
      m_text = "";
      m_old = NULL;
      flush();
   }
}

