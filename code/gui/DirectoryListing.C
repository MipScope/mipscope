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
   file: DirectoryListing.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/26/2007
\* ---------------------------------------------- */
#include "DirectoryListing.H"
#include "EditorPane.H"
#include "Gui.H"
#include <QtGui>

DirectoryListing::DirectoryListing(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("Files"), gui), m_gui(gui), 
   m_editorPane(editorPane), m_dirModel(new QDirModel()) 
{
   setObjectName(tr("Files"));
   
   m_treeView = new QTreeView(this);

   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
   m_treeView->setContextMenuPolicy(Qt::NoContextMenu);

   
   m_contextMenu = new QMenu(this);
   m_openAction = new QAction(QIcon(ICONS"/fileOpen.png"), tr("&Open"), this);
   connect(m_openAction, SIGNAL(triggered()), this, SLOT(openAction()));
   m_contextMenu->addAction(m_openAction);
   
   m_treeView->setModel(m_dirModel);
   m_dirModel->setNameFilters(QStringList() << "*.s" << "..");
   m_dirModel->setFilter(QDir::Files | QDir::Readable | QDir::Writable | QDir::AllDirs);
   m_dirModel->setSorting(QDir::DirsLast);
   //connect(m_dirModel, SIGNAL(rowsInserted(const QModelIndex&,int,int)), this, SLOT(dirModelChanged(const QModelIndex&, int,int)));
   //connect(m_dirModel, SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(dirModelChanged(const QModelIndex&, int,int)));

   m_treeView->setAlternatingRowColors(true);
   m_treeView->setColumnHidden(1, true);
   m_treeView->setColumnHidden(2, true);
   m_treeView->setColumnHidden(3, true);
   m_treeView->header()->hide(); // don't show header titles
   m_treeView->setRootIndex(m_dirModel->index(QDir::currentPath()));
   
   connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex&)), 
         this, SLOT(clicked(const QModelIndex&)));
   //         m_dirModel->removeColumns(1, 2);
   setWidget(m_treeView);
}

//void DirectoryListing::dirModelChanged(const QModelIndex &index, int start, int end) {
//}

void DirectoryListing::clicked(const QModelIndex &index) {
//   cerr << m_dirModel->filePath(index).toStdString() << endl;
   if (m_dirModel->isDir(index))
      m_treeView->setRootIndex(index);
   else loadFile(index);

   //         QStandardItem *item = m_dirModel->itemFromIndex(index);

   //         QColor c(157, 187, 227);
   //         item->setForeground(QBrush(c));
}

void DirectoryListing::contextMenuRequested(const QPoint &pos) {
   m_menuIndex = m_treeView->indexAt(m_treeView->mapFromParent(pos));
   if (m_menuIndex == QModelIndex())
      return;
   m_openAction->setEnabled(!m_dirModel->isDir(m_menuIndex));
   m_contextMenu->popup(mapToGlobal(pos));
}

void DirectoryListing::openAction() {
   clicked(m_menuIndex);
}

void DirectoryListing::loadFile(const QModelIndex &index) const {
   m_editorPane->openFile(m_dirModel->filePath(index));
}

