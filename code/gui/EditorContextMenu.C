/* ---------------------------------------------- *\
   file: EditorContextMenu.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "EditorContextMenu.H"
#include "Gui.H"
#include "EditorPane.H"
#include "Utilities.H"
#include <QtGui>

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
      bool readOnly = !(m_editorPane->isModifiable());
      
      m_close->setEnabled(true);
      if (count <= 1 || readOnly) {
         m_closeOtherTabs->setEnabled(false);

         if (readOnly || (m_editor->file() == NULL && !m_editor->isModified()))
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

