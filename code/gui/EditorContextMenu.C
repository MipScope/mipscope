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

