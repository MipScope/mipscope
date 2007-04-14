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

      bool isModifiable();
      bool isReadOnly();
      
      // Returns true if all tabs closed successfully
      // May return false if user chooses to cancel the operation
      bool closeAllTabs(TextEditor *ignore = NULL);
      QMessageBox::StandardButton saveAllFiles(bool yesToAll = false, TextEditor *ignore = NULL);
   

   signals:
      void isModifiable(bool); // sent out upon read-only change
      void copyAvailabile(bool);
      void undoAvailabile(bool);
      void redoAvailabile(bool);
      void isModified(bool);
      
   private slots:
      void activeEditorChanged(int index);
      
      // Proxy slots for active TextEditor
      void cut();
      void copy();
      void paste();
      void undo();
      void redo();
      void undoAvailabilityModified(bool);
      void redoAvailabilityModified(bool);
      void copyAvailabilityModified(bool);
       
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
      
      bool m_modifiable; // opposite of being read-only
      void setModifiable(bool);
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

   protected:
      EditorPane *m_editorPane;
      QMenu *m_tabMenu;
      QMenu *m_tabBarMenu;
      QAction *m_closeOtherTabs;
      QAction *m_close;
      
      int m_index;
};

#endif // __QEDITOR_PANE_H__

