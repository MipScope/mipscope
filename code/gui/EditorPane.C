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

EditorPane::EditorPane(Gui *parent, const char *fileName) : QTabWidget()
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
      setActiveEditor(new TextEditor(this, file, m_activeEditor));
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
bool EditorPane::closeAllTabs() {
   unsigned int ret = saveAllFiles();
   
   // Make sure all pending changes get saved if the user wants
   if (ret == QMessageBox::Cancel)
      return false;
   
   // Close all editors
   while(m_activeEditor != NULL)
      m_activeEditor->close(false);
   
   return true;
}

unsigned int EditorPane::saveAllFiles(bool yesToAll) {
   TextEditor *cur = m_activeEditor;
   if (cur == NULL)
      return QMessageBox::Yes;
   
   while(cur->m_prev != NULL) cur = cur->m_prev;
   
   unsigned int buttons = (QMessageBox::YesAll | QMessageBox::NoAll);
   int noModified = 0;
   if (count() > 1) {
      TextEditor *orig = cur;

      while(cur != NULL) {
         noModified += cur->isModified();
         cur = cur->m_next;
      }
      cur = orig;
      if (noModified > 1)
         buttons = 0;
   }
   
   while(cur != NULL) {
      if (cur->isModified()) {
         if (yesToAll) {
            cur->save();
         } else {
            unsigned int ret = cur->promptUnsavedChanges(buttons);

            if (ret == QMessageBox::Cancel || ret == QMessageBox::NoAll)
               return ret;

            yesToAll = (ret == QMessageBox::YesAll);
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
   
   unsigned int ret = m_activeEditor->save();

   if (modified && Accepted(ret)) {
      STATUS_BAR->showMessage(QString("File ") + m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void EditorPane::saveAsAction() {
//   TextEditor *editor = m_contextMenu->m_editor;
   
   unsigned int ret = m_activeEditor->saveAs();
   
   if (Accepted(ret)) {
      STATUS_BAR->showMessage(QString("File ") + m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void EditorPane::closeOtherTabsAction() {
   TextEditor *keep = m_contextMenu->m_editor;
   const QString &f = keep->fileName();
   
   setUpdatesEnabled(false);
   cerr << f.toStdString() << endl;

   while(m_activeEditor != NULL) {

      if (m_activeEditor == keep) {
         if (keep->m_prev != NULL)
            m_activeEditor = keep->m_prev;
         else if (keep->m_next != NULL)
            m_activeEditor = keep->m_next;
         else break;
      }
      
      unsigned int ret = m_activeEditor->close();
      if (ret == QMessageBox::Cancel)
         break;
   }

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
EditorContextMenu::EditorContextMenu(EditorPane *editorPane)
   : QObject(editorPane), m_editorPane(editorPane), m_index(0)
{
   m_tabMenu = new QMenu(editorPane);

   m_tabMenu->addAction("New Blank Tab", editorPane, SLOT(newBlankTabAction()));
   m_tabMenu->addSeparator();
   m_tabMenu->addAction("Save", this, SLOT(saveAction()));
   m_tabMenu->addAction("Save As..", this, SLOT(saveAsAction()));
   m_tabMenu->addSeparator();

   m_closeOtherTabs =  m_tabMenu->addAction("Close Other Tabs", editorPane, SLOT(closeOtherTabsAction()));
   m_close = m_tabMenu->addAction("Close", editorPane, SLOT(closeAction()));
   
   m_tabBarMenu = new QMenu(editorPane);
   m_tabBarMenu->addAction("New Blank Tab", editorPane, SLOT(newBlankTabAction()));
   m_tabBarMenu->addSeparator();
   m_tabBarMenu->addAction("Save All", this, SLOT(saveAllAction()));
}

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

void EditorContextMenu::saveAllAction() {
   m_editorPane->saveAllFiles(true);
}

