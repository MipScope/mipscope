/* ---------------------------------------------- *\
   file: EditorContextMenu.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __EDITOR_CONTEXT_MENU_H__
#define __EDITOR_CONTEXT_MENU_H__
#include <QMenu>

class QPoint;
class TextEditor;
class Gui;
class EditorPane;

class EditorContextMenu : public QObject {
   
   Q_OBJECT
   
   public:
      EditorContextMenu(EditorPane *editorPane);
      void showAt(const QPoint &pos, int index);
      
      TextEditor *m_editor;
      
   private slots:
      void saveAction();
      void saveAsAction();

   protected:
      EditorPane *m_editorPane;
      QMenu *m_tabMenu;
      QMenu *m_tabBarMenu;
      QAction *m_closeOtherTabs;
      QAction *m_close;
      
      int m_index;
};

#endif // __EDITOR_CONTEXT_MENU_H__

