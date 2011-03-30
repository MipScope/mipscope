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
   file: Console.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Utilities.H"
#include <QDockWidget>
#include <QVector>
#include <QString>

class Gui;
class EditorPane;
class QTextEdit;
class QFont;

class Console : public QDockWidget {

   Q_OBJECT
   
   public:
      Console(Gui *parent, EditorPane *editorPane, const QString &name, bool createDisplay = true);
      virtual ~Console();

      void setStrings(const QVector<QString>& strings);

   protected slots:
      // Clears the console's display
      void flush();
      
      // Keeps the console's font in sync w/ that of the EditorPane
      void fontChanged(const QFont &newFont);
      
      virtual void visibilityChanged(bool isVisible);
      
   signals:
      void updateBatchSignal();
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      QTextEdit *m_display;
      
      QVector<QString> m_strings;
      bool m_visible;
      
      // Updates the QTextEdit used as a display
      virtual void updateDisplay();
};

#endif // __CONSOLE_H__

