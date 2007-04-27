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

