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
   file: TextEditor.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007

      The main source-code editor; displays PC 
   and syntactic/semantic errors during debugging.
\* ---------------------------------------------- */
#ifndef __TEXTEDITOR_H__
#define __TEXTEDITOR_H__
#include <QTextEdit>
#include <QMessageBox>
#include <QLabel>
#include <QVector>
#include <QList>
#include <QMap>
#include <map>
#include "Utilities.H"
using namespace std;

class Program;
class RegisterTip;
class ParseNode;
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
class QPaintEvent;
class ParseNode;
class QPainter;
class QColor;
class QResizeEvent;
class QTimer;

typedef map<int, int> LineToPosMap;
typedef LineToPosMap::const_iterator LineToMapIterator;

typedef QMap<QTextBlock, QTextEdit::ExtraSelection> ExtraSelectionMap;
typedef ExtraSelectionMap::iterator ExtraSelectionMapIterator;

extern bool Accepted(QMessageBox::StandardButton result);

#define UNTITLED     "Untitled"
#define TEMPLATE     "gui/template.s"

class TextEditor : public QTextEdit {
   
   Q_OBJECT
   
   public:
      friend class EditorPane;
      friend class SyntaxTip;
      friend class Program;
      
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
      
      void setModifiable(bool isModifiable);

      // Utility methods
      bool isModifiable() const;
      bool copyIsAvailable() const;
      bool undoIsAvailable() const;
      bool redoIsAvailable() const;
      
      int lineNumber(const QTextBlock &b) const;
      int lineNumber(const QTextCursor &c) const;
      int currentLineNumber() const;
      int noLines() const; // returns the total number of lines
      void gotoLine(int); // moves text cursor to given line, scrolling editor if necessary
      QTextCursor findDeclaration(const QString &identifier);
      bool gotoDeclaration(const QString &identifier);
      bool gotoDeclaration(const QTextCursor &c);
      void gotoNode(ParseNode *node);
      QTextCursor findDeclaration(const QTextCursor &c);
      
      QTextDocument *getTextDocument() const;
      void changeTextCursor(const QTextCursor &cursor);
      
      // Returns the QTextBlock pertaining to the given line number
      // Returns NULL if the given line number is invalid
      QTextBlock *getBlockForLine(int lineNo, QTextBlock *last = NULL);
      
//      void highlightLine(const QTextCursor &c, const QColor &color);
      //const QTextCharFormat &f);
      
      void clearLastInstructions();
      Program *getProgram() const;
      
      void updateSyntaxErrors(SyntaxErrors *errors);
      
      void testMouseMove(QMouseEvent *e, const QPoint &pos);
      
// returns actual filename of open file
      QString fileName() const;
      QFile *file() const { return m_file; }

   signals:
      void jumpTo(const QTextBlock&); // Jump PC to given text block

      // called whenever the modifiability of this texteditor is changed
      void updateModifiability(bool isCurrentlyModifiable);
      
   private slots:
      void codeChanged();
      void modifiabilityChanged(bool);
      void undoAvailabilityModified(bool);
      void redoAvailabilityModified(bool);
      void toggleComment();
      void updateCursorPosition();
      void fontChanged(const QFont&);
      void errorHighlightingEnabledChanged(bool enabled);
      
      // Related to Debugging
      void pcChanged(ParseNode*, bool justRolledBack);
      void programStatusChanged(int);
      void focusPC();

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
//      QList<ExtraSelection> m_highlighted;
      LineToPosMap m_lineToPosMap;
      
      // Debugging and program-Related
      Program *m_program; // proxy to Debugger and ParseList
      ParseNode *m_pc;
      QVector<ParseNode*> m_lastInstructions;
      
      // for highlighting errors
//      ExtraSelectionMap m_errors;

      // selections without ctrl+click selection
      QList<ExtraSelection> m_selections;
      bool m_hasControlSelection;

      RegisterTip *m_registerTip;
      
      // @overridden to deal w/ showing syntax help
      virtual void keyReleaseEvent(QKeyEvent *e);
      //virtual void contextMenuEvent(QContextMenuEvent *e);
      // @overridden to provide custom highlighting
      virtual void paintEvent(QPaintEvent *e);
      // @overridden to update line numbers accordingly
      virtual void resizeEvent(QResizeEvent *e);
      // @overridden to allow for CTRL+clicking to goto declaration
      virtual void mousePressEvent(QMouseEvent *e);
      virtual void mouseMoveEvent(QMouseEvent *e);
      virtual void leaveEvent(QEvent *);
      
      int attemptRegisterParse(QString &text);
      
      void highlightLine(QPainter &p, ParseNode *parseNode, const QColor &color, const int width, const int height);

      void fillInTemplate();

      static unsigned int m_untitledNo;
      unsigned int m_myUntitledNo;
};

class SyntaxTip : public QLabel {
   
   Q_OBJECT
   
   public:
      SyntaxTip(TextEditor *parent);
       
      void show(const QString &tipText, QString &statusText, const QPoint &pos, const QTextCursor&);
      
      // @overridden
      void mousePressEvent(QMouseEvent*);

   public slots:
      void hide();
      void editorScrolled(int);
      void cursorPositionChanged(const QTextCursor&);

   private slots:
      void testCursorPos();

   protected:
      TextEditor *m_parent;
//      QTextCursor *m_cursor;
      QTextBlock *m_block;
//      int m_startPos;
};

class RegisterTip : public QLabel {
   
   Q_OBJECT
   
   public:
      RegisterTip(TextEditor *parent);
       
      void show(const QString &text, const QPoint &pos);
      
      // @overridden
      virtual void mousePressEvent(QMouseEvent*);
      virtual void mouseMoveEvent(QMouseEvent *);

   public slots:
      void hide();

   protected:
      TextEditor *m_parent;

      enum State { S_Hidden, S_Appearing, S_Fading, S_Normal };
      QTimer *m_timer;
      State m_state;
      int m_alpha;
      
      void paintEvent(QPaintEvent *e);
};


#endif // __TEXTEDITOR_H__

