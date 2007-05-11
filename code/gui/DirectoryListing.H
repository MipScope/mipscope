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
   file: DirectoryListing.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/26/2007
\* ---------------------------------------------- */
#ifndef __DIRECTORY_LISTING_H__
#define __DIRECTORY_LISTING_H__
#include <QDockWidget>
#include <QModelIndex>

class Gui;
class EditorPane;
class QDirModel;
class QTreeView;
class QAction;
class QMenu;

class DirectoryListing : public QDockWidget {
   
   Q_OBJECT
   
   public:
      DirectoryListing(Gui *gui, EditorPane *editorPane);

   private slots:
      void clicked(const QModelIndex &index);
      void contextMenuRequested(const QPoint & pos);
      void openAction();
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      QTreeView *m_treeView;
      QDirModel *m_dirModel;
      QAction *m_openAction;
      QMenu *m_contextMenu;
      QModelIndex m_menuIndex;

      void loadFile(const QModelIndex &index) const;
};

#endif // __DIRECTORY_LISTING_H__

