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
   file: ErrorConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#ifndef __ERROR_CONSOLE_H__
#define __ERROR_CONSOLE_H__
#include "Console.H"
#include "Utilities.H"

class Gui;
class EditorPane;
class TextEditor;
class QUrl;

class ErrorConsole : public Console {

   Q_OBJECT
   
   public:
      ErrorConsole(Gui *parent, EditorPane *editorPane);
      
      void updateSyntaxErrors(SyntaxErrors *s, TextEditor *editor, bool forceUpdate = true);
      virtual void reset();
   
   private slots:
      void anchorClicked(const QUrl &link);
      
   protected:
      QString m_text;
      TextEditor *m_activeEditor; // one for which we're displaying errors
      SyntaxErrors *m_old;
};

#endif // __ERROR_CONSOLE_H__

