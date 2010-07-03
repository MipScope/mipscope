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
   file: EditorPane.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __QEDITOR_PANE_H__
#define __QEDITOR_PANE_H__
#include <QTabWidget>
#include "TextEditor.H"
#include <QMenu>
#include <QFont>

class QAction;
class QPoint;
class SyntaxHighlighter;
class TextEditor;
class EditorContextMenu;
class LineNoPane;
class LineNoDisplay;
class Gui;
class ErrorConsole;
class FindDialog;

class EditorPane : public QTabWidget {
   
   Q_OBJECT
   
   public: // yeah for good design :)  Huzzah!
      friend class Gui;
      friend class EditorContextMenu;
      friend class TextEditor;
      friend class LineNoDisplay;
      friend class LineNo;
      friend class ErrorConsole;
      friend class FindDialog;
      friend class StatementListing;
      
      EditorPane(Gui *parent, const char *fileName = NULL);
	   virtual ~EditorPane(void);

      void setActiveEditor(TextEditor *newlyActive);
      void openFile(const QString &fileName);

      bool isModifiable();
      bool isReadOnly();

      void showContextMenu(QContextMenuEvent *e);
      
      // Returns true if all tabs closed successfully
      // May return false if user chooses to cancel the operation
      bool closeAllTabs(TextEditor *ignore = NULL);
      QMessageBox::StandardButton saveAllFiles(bool yesToAll = false, TextEditor *ignore = NULL);
      
      QFont *font() { return m_font; }

   signals:
      void isModifiable(bool); // sent out upon read-only change
      void copyAvailabile(bool);
      void undoAvailabile(bool);
      void redoAvailabile(bool);
      void isModified(bool);
      void fontChanged(const QFont&);
      void updateLineNumbers(TextEditor*, int);
      void activeEditorChanged(TextEditor*);
      // emitted whenever content in current editor has changed
      void contentChanged(TextEditor*);
      
   private slots:
      void activeEditorChanged(int index);
      void updateLineNumbers(int);
      void contentChangedProxy();
      void toggleComment();
      void selectAll();
      void resetModified();
      // called whenever the modifiability of a texteditor is changed
      void modifiabilityChanged(bool isModifiable);
      
      void currentFontChanged(const QFont&);

      // Proxy slots for active TextEditor
      void cut();
      void copy();
      void paste();
      void undo();
      void redo();
      void gotoLine();
      void gotoDeclaration();
      void undoAvailabilityModified(bool);
      void redoAvailabilityModified(bool);
      void copyAvailabilityModified(bool);
      
      void find();
      void findAndReplace();
      
      // Context-menu related
      void contextMenuRequested(const QPoint & pos);
      void newBlankTabAction();
      void saveAction();
      void saveAsAction();
      void closeOtherTabsAction();
      void closeAction();
      
      
   protected:
      TextEditor *m_activeEditor;
      SyntaxHighlighter *m_highlighter;
      EditorContextMenu *m_contextMenu;
      Gui *m_parent;
      LineNoDisplay *m_lineNoDisplay;
      QFont *m_font; // font used by all TextEditors
      FindDialog *m_findDialog;
      
      bool m_modifiable; // opposite of being read-only
      void setModifiable(bool);
};

#endif // __QEDITOR_PANE_H__

