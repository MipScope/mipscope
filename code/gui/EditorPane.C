/* ---------------------------------------------- *\
   file: EditorPane.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "EditorPane.H"
#include "SyntaxHighlighter.H"
#include "TextEditor.H"
#include "Gui.H"
#include "Utilities.H"
#include <QtGui>

EditorPane::EditorPane(Gui *parent, const char *fileName) : QTabWidget(), m_modifiable(true)
{
   m_activeEditor = new TextEditor(this);
   //m_highlighter = new SyntaxHighlighter(m_activeEditor->document());
   m_parent = parent;
   m_contextMenu = new EditorContextMenu(this);

   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
   connect(this, SIGNAL(currentChanged(int)), this, SLOT(activeEditorChanged(int)));
   
   addTab(m_activeEditor, "");

   if (fileName != NULL)
      openFile(QString(fileName));
}

EditorPane::~EditorPane(void) { }

void EditorPane::openFile(const QString &fileName) {
   if (!QFile::exists(fileName))
      return;
   
   QFile *file = new QFile(fileName);
   
   if (m_activeEditor != NULL && !m_activeEditor->document()->isModified() && !m_activeEditor->m_loaded) {
      if (m_activeEditor->openFile(file))
         setActiveEditor(m_activeEditor);
      
      // else error
   } else { // open a tab for a new TextEditor
      TextEditor *editor = new TextEditor(this, NULL, m_activeEditor);
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
   QTabBar *tabbar = tabBar();
   tabbar->setTabTextColor(indexOf(m_activeEditor), Qt::black);
   tabbar->setTabTextColor(index, Qt::blue);
   
   m_activeEditor = (TextEditor*) widget(index);
   
   // Send out signals to update (enable/disable) certain Gui Actions
   copyAvailabile(m_activeEditor->copyIsAvailable());
   undoAvailabile(m_activeEditor->undoIsAvailable());
   redoAvailabile(m_activeEditor->redoIsAvailable());
   isModifiable(m_activeEditor->isModifiable());
   isModified(m_activeEditor->isModified());
   
   //m_highlighter->setDocument(m_activeEditor->document());
}

void EditorPane::contextMenuRequested(const QPoint & pos) {
   QTabBar *tabbar = tabBar();

   int i, count = tabbar->count();
   
   if (geometry().contains(pos))
      return; // no friekin idea why this works; seems backwards to me.. huzzah!
   
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
      if (noModified <= 1)
         buttons = QMessageBox::NoButton;
   }
   
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
   TextEditor *blank = new TextEditor(this, NULL, m_activeEditor);
   setActiveEditor(blank);
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
      m_activeEditor = new TextEditor(this);
      addTab(m_activeEditor, "");
   }
}

// Right-click context-menu for tabs
// ---------------------------------
EditorContextMenu::EditorContextMenu(EditorPane *editorPane)
   : QObject(editorPane), m_editorPane(editorPane), m_index(0)
{
   m_tabMenu = new QMenu(editorPane);
   
   QAction *fileNew = new QAction(QIcon(ICONS"/fileNew.png"), "New Blank Tab", editorPane);
   connect(fileNew, SIGNAL(triggered()), editorPane, SLOT(newBlankTabAction()));
   m_tabMenu->addAction(fileNew);
   
   m_tabMenu->addSeparator();
   m_tabMenu->addAction(editorPane->m_parent->getSaveAction());
   m_tabMenu->addAction(QIcon(ICONS"/fileSave.png"), "Save As..", this, SLOT(saveAsAction()));
   m_tabMenu->addSeparator();

   m_closeOtherTabs =  m_tabMenu->addAction("Close Other Tabs", editorPane, SLOT(closeOtherTabsAction()));
   m_close = m_tabMenu->addAction(QIcon(ICONS"/closeTab.png"), "Close", editorPane, SLOT(closeAction()));
   
   m_tabBarMenu = new QMenu(editorPane);
   m_tabBarMenu->addAction(fileNew);
   m_tabBarMenu->addSeparator();
   m_tabBarMenu->addAction(editorPane->m_parent->getSaveAllAction());
}

// Show the right-click menu for the tabbed area
// ---------------------------------------------
void EditorContextMenu::showAt(const QPoint &pos, int index) {
   m_index = index;

   if (index >= 0) {
      m_editor = (TextEditor*)m_editorPane->widget(index);
      int count = m_editorPane->count();
      
      m_close->setEnabled(true);
      if (count <= 1) {
         m_closeOtherTabs->setEnabled(false);

         if (m_editor->file() == NULL && !m_editor->isModified())
            m_close->setEnabled(false);
      } else m_closeOtherTabs->setEnabled(true);
      
      m_tabMenu->popup(pos);
   } else {
      m_editor = NULL;
      m_tabBarMenu->popup(pos);
   }
}

void EditorContextMenu::saveAction() {
   m_editorPane->setActiveEditor(m_editor);
   m_editorPane->saveAction();
}

void EditorContextMenu::saveAsAction() {
   m_editorPane->setActiveEditor(m_editor);
   m_editorPane->saveAsAction();
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

void EditorPane::redo(){
   m_activeEditor->redo();
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

