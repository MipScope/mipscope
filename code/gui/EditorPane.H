/* ---------------------------------------------- *\
   file: EditorPane.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __QEDITOR_PANE_H__
#define __QEDITOR_PANE_H__
#include <QTabWidget>
#include "TextEditor.H"
#include <QMenu>

class QAction;
class QPoint;
class SyntaxHighlighter;
class TextEditor;
class EditorContextMenu;
class Gui;

class EditorPane : public QTabWidget {
   
   Q_OBJECT
   
   public:
      friend class Gui;
      friend class EditorContextMenu;
      friend class TextEditor;
      
      EditorPane(Gui *parent, const char *fileName = NULL);
	   virtual ~EditorPane(void);

      void setActiveEditor(TextEditor *newlyActive);
      void openFile(const QString &fileName);
      
      // Returns true if all tabs closed successfully
      // May return false if user chooses to cancel the operation
      bool closeAllTabs();
      unsigned int saveAllFiles(bool yesToAll = false);
   //signals:
      
      
   private slots:
      void activeEditorChanged(int index);
      
      // Context-menu related
      void contextMenuRequested(const QPoint & pos);
      void newBlankTabAction();
      void saveAction();
      void saveAsAction();
      void closeOtherTabsAction();
      void closeAction();
      
   protected:
      TextEditor *m_activeEditor;
      SyntaxHighlighter *m_highlighter;
      EditorContextMenu *m_contextMenu;
      Gui *m_parent;
};

class EditorContextMenu : public QObject {
   
   Q_OBJECT
   
   public:
      EditorContextMenu(EditorPane *editorPane);
      void showAt(const QPoint &pos, int index);
      
      TextEditor *m_editor;
      
   private slots:
      void saveAction();
      void saveAsAction();
      void saveAllAction();

   protected:
      EditorPane *m_editorPane;
      QMenu *m_tabMenu;
      QMenu *m_tabBarMenu;
      QAction *m_closeOtherTabs;
      QAction *m_close;
      
      int m_index;
};

#endif // __QEDITOR_PANE_H__

