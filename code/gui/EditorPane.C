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
//   m_highlighter = new SyntaxHighlighter(m_activeEditor->document());
   m_parent = parent;

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
   
   if (!m_activeEditor->document()->isModified() && !m_activeEditor->m_loaded) {
      if (m_activeEditor->openFile(file))
         setActiveEditor(m_activeEditor);
      
      // else error
   } else { // open a tab for a new TextEditor
      setActiveEditor(new TextEditor(this, file, m_activeEditor));
   }
}

void EditorPane::setActiveEditor(TextEditor *newlyActive) {
   if (indexOf(newlyActive) < 0)
      addTab(newlyActive, "");
   
   QTabBar *tabbar = tabBar();
   tabbar->setTabTextColor(indexOf(m_activeEditor), Qt::black);
   m_activeEditor = newlyActive;
   
   setCurrentWidget(m_activeEditor);
//   m_highlighter->setDocument(newlyActive->document());
}

void EditorPane::activeEditorChanged(int index) {
   QTabBar *tabbar = tabBar();
   tabbar->setTabTextColor(indexOf(m_activeEditor), Qt::black);
   tabbar->setTabTextColor(index, Qt::blue);
   
   m_activeEditor = (TextEditor*) widget(index);
}

void EditorPane::contextMenuRequested(const QPoint & pos) {
   QTabBar *tabbar = tabBar();

   int i, count = tabbar->count();
   
   for(i = count; i--;) {
      const QRect &bounds = tabbar->tabRect(i);

      if (bounds.contains(pos)) {
         cerr << "\tRectangle " << i << " contains point\n";
         return;
      }
   }
}

// Returns true if all tabs closed successfully
// May return false if user chooses to cancel the operation
bool EditorPane::closeAllTabs() {
   unsigned int ret = saveAllFiles();
   
   // Make sure all pending changes get saved if the user wants
   if (ret == QMessageBox::Cancel)
      return false;
   
   // Close all editors
   while(m_activeEditor != NULL) {
      m_activeEditor->close(false);
      if (m_activeEditor->m_prev != NULL)
         m_activeEditor = m_activeEditor->m_prev;
      else m_activeEditor = m_activeEditor->m_next;
   }
   
   return true;
}

unsigned int EditorPane::saveAllFiles(bool yesToAll) {
   TextEditor *cur = m_activeEditor;
   if (cur == NULL)
      return Accepted;
   
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
   
   return Accepted;
}

