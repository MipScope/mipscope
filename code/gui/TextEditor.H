/* ---------------------------------------------- *\
   file: TextEditor.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __TEXTEDITOR_H__
#define __TEXTEDITOR_H__
#include <QTextEdit>
#include <QMessageBox>

class SyntaxHighlighter;
class EditorPane;
class QKeyEvent;
class QFile;

extern bool Accepted(QMessageBox::StandardButton result);

class TextEditor : public QTextEdit {
   
   Q_OBJECT
   
   public:
      friend class EditorPane;
      
      TextEditor(EditorPane *parent = NULL, QFile *file = NULL, TextEditor *prev = NULL);
	   virtual ~TextEditor(void);
      
      void setupEditor();
      bool openFile(QFile *file);
      void resetTabText(bool modified = false);
      
      // returns true upon successful closure; false otherwise
      QMessageBox::StandardButton close(bool promptForSave = true);
      QMessageBox::StandardButton promptUnsavedChanges(QMessageBox::StandardButtons extraButtons = 0);
      // returns true if no pending changes after executing this method
      QMessageBox::StandardButton save(bool forceSave = false);
      QMessageBox::StandardButton saveAs();
      // proxy for document()->isModified()
      bool isModified() const;

      // Utility methods
      bool isModifiable() const;
      bool copyIsAvailable() const;
      bool undoIsAvailable() const;
      bool redoIsAvailable() const;
      
// returns actual filename of open file
      QString fileName() const;
      QFile *file() const { return m_file; }

   private slots:
      void codeChanged();
      void modifiabilityChanged(bool);
      void undoAvailabilityModified(bool);
      void redoAvailabilityModified(bool);
      
   protected:
      EditorPane *m_parent;
      
      // double-linked list of TextEditors for EditorPane to keep track of
      TextEditor *m_prev, *m_next;
      QFile *m_file;
      
      // true if a document has been loaded; false otherwise
      bool m_loaded, m_modified, m_undoAvailable, m_redoAvailable;

      SyntaxHighlighter *m_syntaxHighligher;
      
      // @overridden to deal w/ showing syntax help
      virtual void keyReleaseEvent(QKeyEvent *e);
};

#endif // __TEXTEDITOR_H__

