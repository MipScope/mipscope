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
   : QDockWidget(name, gui), m_gui(gui), m_editorPane(editorPane), m_display(NULL), m_visible(false)
{
   QDockWidget::setObjectName(name);
   
   if (createDisplay) {
      m_display = new QTextEdit(this);
      m_display->setReadOnly(true);
      m_display->setAcceptRichText(false);
      m_display->setFont(*m_editorPane->font());
      m_display->setLineWrapMode(QTextEdit::NoWrap);
      
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

void Console::visibilityChanged(bool isVisible) {
   m_visible = isVisible;
}

void Console::updateDisplay() {
   // hack to keep the Console at the same position after resetting html
   //QScrollBar *scroll = m_display->verticalScrollBar();
   //int value = scroll->value();
   
   m_display->setUpdatesEnabled(false);
//   m_display->setPlainText("");
      
  // cerr << "Setting plain text)" << endl;
   m_display->setPlainText(QStringList(m_strings.toList()).join(QString("")));
   /*foreach(const QString &string, m_strings)
      c.insertText(string);*/
   
   QTextCursor c = m_display->textCursor();
   c.movePosition(QTextCursor::End);
   m_display->setTextCursor(c);
   m_display->ensureCursorVisible();
   m_display->setUpdatesEnabled(true);
   //scroll = m_display->verticalScrollBar();
   //int newValue = scroll->value();;
   //if (newValue < value)
   //   m_display->verticalScrollBar()->setValue(value);
   

   
   // TODO!!!!!!!!!!!!!!!  TODO
   
   
   
}

void Console::setStrings(const QVector<QString>& strings)
{
	m_strings = strings;
}
