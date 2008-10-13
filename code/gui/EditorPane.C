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
   file: EditorPane.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "EditorPane.H"
#include "EditorContextMenu.H"
#include "SyntaxHighlighter.H"
#include "LineNoPane.H"
#include "TextEditor.H"
#include "FindDialog.H"
#include "Gui.H"
#include "Program.H"
#include "ErrorConsole.H"
#include "Utilities.H"
#include <QtGui>

#include <iostream>

EditorPane::EditorPane(Gui *parent, const char *fileName) : QTabWidget(), m_parent(parent), m_font(new QFont(Options::getFont())), m_findDialog(new FindDialog(this)), m_modifiable(true)
{
   m_font->setFixedPitch(true);
   m_activeEditor = new TextEditor(this, m_font);
   //m_highlighter = new SyntaxHighlighter(m_activeEditor->document());
   m_lineNoDisplay = NULL;
   m_contextMenu = new EditorContextMenu(this);

   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
   connect(this, SIGNAL(currentChanged(int)), this, SLOT(activeEditorChanged(int)));
   
   addTab(m_activeEditor, "");
   m_activeEditor->setFocus();

   if (fileName != NULL)
      openFile(QString(fileName));
   else m_activeEditor->openFile(NULL); // open a default, template file
}

EditorPane::~EditorPane(void) { }

void EditorPane::openFile(const QString &fileName) {
   if (!QFile::exists(fileName))
      return;
   
   m_parent->addRecentFile(fileName);
   QFile *file = new QFile(fileName);
   
   if (m_activeEditor != NULL && !m_activeEditor->document()->isModified() && !m_activeEditor->m_loaded) {
      if (m_activeEditor->openFile(file)) {
         setActiveEditor(m_activeEditor);
			
			//Bug fix, hack: TextEditor briefly thinks it has changed and
			//has fired off a changed event causing signal isModified(true).
			//Resync with actual isModified() property.
			isModified(m_activeEditor->isModified());
		}
      
//      contentChanged(m_activeEditor);
      // else error
   } else { // open a tab for a new TextEditor
      TextEditor *editor = new TextEditor(this, m_font, NULL, m_activeEditor);
      setActiveEditor(editor);
      editor->openFile(file);
   }
	
	
}

void EditorPane::setActiveEditor(TextEditor *newlyActive) {
/*   if (newlyActive == m_activeEditor)
      return;*/
   
   bool newEditor = false;
   if (newlyActive != NULL) {
      //m_highlighter->setDocument(newlyActive->document());

      if ((newEditor = indexOf(newlyActive) < 0)) {
         //setUpdatesEnabled(false);
         addTab(newlyActive, "");
      }
      
      QTabBar *tabbar = tabBar();
      tabbar->setTabTextColor(indexOf(m_activeEditor), Qt::black);
   }
   m_activeEditor = newlyActive;
   setCurrentWidget(m_activeEditor);
  
   /*if (newEditor) {
      setUpdatesEnabled(true);
      update();
   }*/
}

void EditorPane::activeEditorChanged(int index) {

	//cerr << "active changed to: " << index << endl;
	
	if (index == -1) {
		//we don't care, abort
		return;
	}

   QTabBar *tabbar = tabBar();
   tabbar->setTabTextColor(indexOf(m_activeEditor), Qt::black);
   tabbar->setTabTextColor(index, Qt::blue);
   
   m_activeEditor = (TextEditor*) widget(index);
   
   // Send out signals to update (enable/disable) certain Gui Actions
   if (!m_activeEditor->isModifiable()) {
      isModifiable(m_activeEditor->isModifiable());
   } else {
      copyAvailabile(m_activeEditor->copyIsAvailable());
      undoAvailabile(m_activeEditor->undoIsAvailable());
      redoAvailabile(m_activeEditor->redoIsAvailable());
   }
   
   // notify main Gui first
   m_parent->activeEditorChanged(m_activeEditor);
   
   // notify other listeners
   activeEditorChanged(m_activeEditor);
   QTimer::singleShot(250, this, SLOT(resetModified()));
   isModified(m_activeEditor->isModified());
   
   Program *program = m_activeEditor->getProgram();
   if (program)
      program->setRunnable(program->isRunnable());

//   cerr << m_activeEditor->isModified() << ", ";// << m_parent->m_editUndoAction->isEnabled() << endl;
   
   //m_highlighter->setDocument(m_activeEditor->document());
}

void EditorPane::resetModified() {
	
	//cerr << "resetModified" << endl;
   isModified(m_activeEditor->isModified());
}

void EditorPane::contextMenuRequested(const QPoint & pos) {
   QTabBar *tabbar = tabBar();

   int i, count = tabbar->count();
   
   bool inTabBar = (tabbar->geometry().contains(pos));
   bool inBarOrTextEdit = (geometry().contains(pos));
//   bool inActive = m_activeEditor->rect().contains(pos);
   bool inY = (pos.y() < geometry().height() - m_activeEditor->rect().height());
   
//   cerr << "(" << pos.x() << ", " << pos.y() << ")" << " \t" << geometry().height() << " \t" << m_activeEditor->rect().height() << endl;

//   cerr << inTabBar << ", " <<  inBarOrTextEdit << ", " << inY << endl;
   if (!inTabBar && (!inBarOrTextEdit || !inY)) {
      // Show context menu in active text editor
      m_parent->getEditMenu()->exec(mapToGlobal(pos));
      
      return; // no friekin idea why this works; seems backwards to me.. huzzah!
   }
   for(i = count; i--;) {
      const QRect &bounds = tabbar->tabRect(i);

      if (bounds.contains(pos)) {
         //cerr << "\tRectangle " << i << " contains point\n";
         TextEditor *hit = (TextEditor*) widget(i);
         if (m_activeEditor != hit)
            setActiveEditor(hit);
         
         m_contextMenu->showAt(mapToGlobal(pos), i);
         return;
     }
   }

   m_contextMenu->showAt(mapToGlobal(pos), -1);
}

// Returns true if all tabs closed successfully
// May return false if user chooses to cancel the operation
bool EditorPane::closeAllTabs(TextEditor *ignore) {
   QMessageBox::StandardButton ret = saveAllFiles(false, ignore);
   
   // Make sure all pending changes get saved if the user wants
   if (ret == QMessageBox::Cancel)
      return false;
   
   // Close all editors
   while(m_activeEditor != NULL) {
      // possibly allow one editor to stay open
      // (for ex: see closeOtherTabsAction()
      if (m_activeEditor == ignore) {
         if (ignore->m_prev != NULL)
            m_activeEditor = ignore->m_prev;
         else if (ignore->m_next != NULL)
            m_activeEditor = ignore->m_next;
         else break;
      }

      m_activeEditor->close(false);
   }
   
   return true;
}

void EditorPane::showContextMenu(QContextMenuEvent *e) {
//   cerr << "Showing context menu\n";
   m_parent->getEditMenu()->exec(e->globalPos());
}

QMessageBox::StandardButton EditorPane::saveAllFiles(bool yesToAll, TextEditor *ignore) {
   TextEditor *cur = m_activeEditor;
   if (cur == NULL)
      return QMessageBox::Yes;
   
   while(cur->m_prev != NULL) cur = cur->m_prev;
   
   QMessageBox::StandardButtons buttons = (QMessageBox::YesToAll | QMessageBox::NoToAll);
   int noModified = 0;
   if (count() > 1) {
      TextEditor *orig = cur;

      while(cur != NULL) {
         if (cur != ignore)
            noModified += cur->isModified();

         cur = cur->m_next;
      }
      
      cur = orig;
   }
   
   if (noModified <= 1)
      buttons = QMessageBox::NoButton;

   while(cur != NULL) {
      if (cur != ignore && cur->isModified()) {
         if (yesToAll) {
            cur->save();
         } else {
            QMessageBox::StandardButton ret = cur->promptUnsavedChanges(buttons);
            
            //cerr << ret << endl;
            if (ret == QMessageBox::Cancel || ret == QMessageBox::NoToAll)
               return ret;

            yesToAll = (ret == QMessageBox::YesToAll);
         }
      }

      cur = cur->m_next;
   }
   
   return QMessageBox::Yes;
}

// --------------------
// Context-menu related
// --------------------
void EditorPane::newBlankTabAction() {
   TextEditor *blank = new TextEditor(this, m_font, NULL, m_activeEditor);
   setActiveEditor(blank);
   blank->resetTabText();
}

void EditorPane::saveAction() {
   //TextEditor *editor = m_contextMenu->m_editor;
   bool modified = m_activeEditor->isModified();
   
   if (!modified) {
      STATUS_BAR->showMessage(QString("File ") + m_activeEditor->fileName() + 
            QString(" does not need to be saved."), STATUS_DELAY);
      return;
   }
   
   QMessageBox::StandardButton ret = m_activeEditor->save();

   if (modified && Accepted(ret)) {
      STATUS_BAR->showMessage(QString("File ") + m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void EditorPane::saveAsAction() {
//   TextEditor *editor = m_contextMenu->m_editor;
   
   QMessageBox::StandardButton ret = m_activeEditor->saveAs();
   
   if (Accepted(ret)) {
      STATUS_BAR->showMessage(QString("File ") + m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void EditorPane::closeOtherTabsAction() {
   TextEditor *keep = m_contextMenu->m_editor;
   
   setUpdatesEnabled(false);
   closeAllTabs(keep);
   
//   const QString &f = keep->fileName();
//   cerr << f.toStdString() << endl;

   /*while(m_activeEditor != NULL) {
      if (m_activeEditor == keep) {
         if (keep->m_prev != NULL)
            m_activeEditor = keep->m_prev;
         else if (keep->m_next != NULL)
            m_activeEditor = keep->m_next;
         else break;
      }
      
      QMessageBox::StandardButton ret = m_activeEditor->close();
      if (ret == QMessageBox::Cancel)
         break;
   }*/

   //if (m_activeEditor != NULL)
      //m_highlighter->setDocument(m_activeEditor->document());
   
   setUpdatesEnabled(true);
   update();
}

void EditorPane::closeAction() {
   setActiveEditor(m_contextMenu->m_editor);
   m_activeEditor->close();

   if (m_activeEditor == NULL) {
      m_activeEditor = new TextEditor(this, m_font);
      addTab(m_activeEditor, m_activeEditor->fileName());

      ErrorConsole *err = m_parent->getErrorConsole();
      if (err != NULL)
         err->updateSyntaxErrors(m_activeEditor->m_program->getSyntaxErrors(), m_activeEditor);
   }
}

void EditorPane::currentFontChanged(const QFont &f) {
   m_font = new QFont(f);
   
   fontChanged(f);
}

// Proxy slots for active TextEditor
// ---------------------------------
void EditorPane::cut() {
   m_activeEditor->cut();
}

void EditorPane::copy() {
   m_activeEditor->copy();
}

void EditorPane::paste() {
   m_activeEditor->paste();
}

void EditorPane::undo() {
   m_activeEditor->undo();
}

void EditorPane::redo() {
   m_activeEditor->redo();
}

void EditorPane::toggleComment() {
   m_activeEditor->toggleComment();
}

void EditorPane::gotoLine() {
   int max = m_activeEditor->noLines();
   bool okay = false;
   
   int lineNo = QInputDialog::getInteger(this, "Goto Line", "Line No:", 0, 0, max, 1, &okay);
   if (okay)
      m_activeEditor->gotoLine(lineNo);
}

void EditorPane::gotoDeclaration() {
   QTextCursor c = m_activeEditor->textCursor();
   if (!c.hasSelection())
      c.select(QTextCursor::WordUnderCursor);
   
   m_activeEditor->gotoDeclaration(c);
}

void EditorPane::selectAll() {
   m_activeEditor->selectAll();
}

void EditorPane::find() {
   m_findDialog->show(false, m_activeEditor->textCursor());
}

void EditorPane::findAndReplace() {
   m_findDialog->show(true, m_activeEditor->textCursor());
}


// Proxy signals from active TextEditor to Gui
// -------------------------------------------
void EditorPane::copyAvailabilityModified(bool isAvailable) {
   copyAvailabile(isAvailable);
}

void EditorPane::undoAvailabilityModified(bool isAvailable) {
   undoAvailabile(isAvailable);
}

void EditorPane::redoAvailabilityModified(bool isAvailable) {
   redoAvailabile(isAvailable);
}

bool EditorPane::isModifiable() {
   return m_modifiable;
}

bool EditorPane::isReadOnly() {
   return !m_modifiable;
}

void EditorPane::setModifiable(bool modifiable) {
   m_modifiable = modifiable;
   
   // alert all listeners of change in read-only status
   isModifiable(modifiable);
}
// called whenever the modifiability of a texteditor is changed
void EditorPane::modifiabilityChanged(bool isModifiable) {
   setModifiable(isModifiable);
}

void EditorPane::updateLineNumbers(int val) {
   updateLineNumbers(m_activeEditor, val);
}

void EditorPane::contentChangedProxy() {
   contentChanged(m_activeEditor);
}

