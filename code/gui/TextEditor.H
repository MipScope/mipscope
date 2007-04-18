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
#include <QLabel>
#include <QList>
#include <map>
using namespace std;

class QTextBlock;
class QFont;
class SyntaxHighlighter;
class EditorPane;
class QKeyEvent;
class QFile;
class QPoint;
class SyntaxTip;
class QTextCharFormat;
class QShortcut;
class QContextMenuEvent;

typedef map<int, int> LineToPosMap;
typedef LineToPosMap::const_iterator LineToMapIterator;

extern bool Accepted(QMessageBox::StandardButton result);

// userState() in QTextBlock
enum BlockStates { B_DEFAULT = -1, B_NORMAL = 0, B_BREAKPOINT = 1 };
#define UNTITLED     "Untitled"

class TextEditor : public QTextEdit {
   
   Q_OBJECT
   
   public:
      friend class EditorPane;
      friend class SyntaxTip;
      
      TextEditor(EditorPane *parent, QFont *font, QFile *file = NULL, TextEditor *prev = NULL);
	   virtual ~TextEditor(void);
      
      void setupEditor(QFont *font);
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

      int lineNumber(const QTextBlock &b) const;
      int lineNumber(const QTextCursor &c) const;
      int currentLineNumber() const;
      int noLines() const; // returns the total number of lines
      
      // Returns the QTextBlock pertaining to the given line number
      // Returns NULL if the given line number is invalid
      QTextBlock *getBlockForLine(int lineNo, QTextBlock *last = NULL);
      
      void highlightLine(const QTextCursor &c, const QColor &color);
      //const QTextCharFormat &f);
      
// returns actual filename of open file
      QString fileName() const;
      QFile *file() const { return m_file; }

   private slots:
      void codeChanged();
      void modifiabilityChanged(bool);
      void undoAvailabilityModified(bool);
      void redoAvailabilityModified(bool);
      void toggleComment();
      
   protected:
      EditorPane *m_parent;
      
      // double-linked list of TextEditors for EditorPane to keep track of
      TextEditor *m_prev, *m_next;
      QFile *m_file;
      SyntaxTip *m_syntaxTip;
      QShortcut *m_commentLine;
      
      // true if a document has been loaded; false otherwise
      bool m_loaded, m_modified, m_undoAvailable, m_redoAvailable;

      SyntaxHighlighter *m_syntaxHighligher;
      QList<ExtraSelection> m_highlighted;
      LineToPosMap m_lineToPosMap;
      
      // @overridden to deal w/ showing syntax help
      virtual void keyReleaseEvent(QKeyEvent *e);
      //virtual void contextMenuEvent(QContextMenuEvent *e);
};

class SyntaxTip : public QLabel {
   
   Q_OBJECT
   
   public:
      SyntaxTip(TextEditor *parent);
       
      void show(const QString &tipText, const QString &statusText, const QPoint &pos, const QTextCursor&);

      // @overridden
      void mousePressEvent(QMouseEvent*);

   public slots:
      void hide();
      void editorScrolled(int);

   private slots:
      void cursorPositionChanged(const QTextCursor&);
      void testCursorPos();

   protected:
      TextEditor *m_parent;
//      QTextCursor *m_cursor;
      QTextBlock *m_block;
//      int m_startPos;
};

#endif // __TEXTEDITOR_H__

