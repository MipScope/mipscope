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
#include <QFont>

class QAction;
class QPoint;
class SyntaxHighlighter;
class TextEditor;
class EditorContextMenu;
class LineNoPane;
class LineNoDisplay;
class Gui;

class EditorPane : public QTabWidget {
   
   Q_OBJECT
   
   public:
      friend class Gui;
      friend class EditorContextMenu;
      friend class TextEditor;
      friend class LineNoDisplay;
      friend class LineNo;
      
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
      void fontChanged(const QFont&);
      void editorScrolled(TextEditor*, int);
      void activeEditorChanged(TextEditor*);
      
   private slots:
      void activeEditorChanged(int index);
      void editorScrolled(int);
      
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
      LineNoDisplay *m_lineNoDisplay;
      QFont *m_font; // font used by all TextEditors
      
      bool m_modifiable; // opposite of being read-only
      void setModifiable(bool);
};

#endif // __QEDITOR_PANE_H__

