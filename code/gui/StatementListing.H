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
   file: StatementListing.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */
#ifndef __STATEMENT_LISTING_H__
#define __STATEMENT_LISTING_H__
#include <QDockWidget>
#include <QTreeWidget>

class Gui;
class EditorPane;
class QAction;
class QMenu;
class QModelIndex;
class TreeListing;

class StatementListing : public QDockWidget {
   
   Q_OBJECT
   
   public:
      StatementListing(Gui *gui, EditorPane *editorPane);

      QString fix(const char *str);

   private slots:
      void clicked(const QModelIndex &index);
      void contextMenuRequested(const QPoint & pos);
      void insertAction();
      
      void showAllInstructionsChanged(bool);
      void updateListing();

   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      TreeListing *m_tree;

      QAction *m_insertAction;
      QMenu *m_contextMenu;
      QModelIndex m_menuIndex;

      bool isHeading(const QModelIndex &index);
};

class TreeListing : public QTreeWidget {
   public:
      TreeListing(StatementListing *listing);

      QTreeWidgetItem *getItem(const QModelIndex &index);
      QModelIndex getIndex(QTreeWidgetItem *item);

   protected:
      StatementListing *m_listing;
};

#endif // __STATEMENT_LISTING_H__

