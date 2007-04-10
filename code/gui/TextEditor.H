/* ---------------------------------------------- *\
   file: TextEditor.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __TEXTEDITOR_H__
#define __TEXTEDITOR_H__
#include <QTextEdit>

class SyntaxHighlighter;
class EditorPane;
class QKeyEvent;
class QFile;

class TextEditor : public QTextEdit {
   
   Q_OBJECT
   
   public:
      friend class EditorPane;
      
      TextEditor(EditorPane *parent = NULL, QFile *file = NULL, TextEditor *prev = NULL);
	   virtual ~TextEditor(void);
      
      void setupEditor();
      bool openFile(QFile *file);
      void save();
      void saveAs();
      
// returns actual filename of open file
      QString fileName();


   private slots:
      void codeChanged();
      
   protected:
      EditorPane *m_parent;
      
      // double-linked list of TextEditors for EditorPane to keep track of
      TextEditor *m_prev, *m_next;
      QFile *m_file;
      
      // true if a document has been loaded; false otherwise
      bool m_loaded;

      SyntaxHighlighter *m_syntaxHighligher;
      
      // @overridden to deal w/ showing syntax help
      virtual void keyReleaseEvent(QKeyEvent *e);
};

#endif // __TEXTEDITOR_H__

