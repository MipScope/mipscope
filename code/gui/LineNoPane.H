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
   file: LineNoPane.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __LINE_NO_PANE_H__
#define __LINE_NO_PANE_H__
#include <QMenu>
#include <QLabel>
#include <list>
#include <QPixmap>
using  namespace std;

class QSize;
class Gui;
class EditorPane;
class QLabel;
class QFont;
class TextEditor;
class QWidget;
class LineNoDisplay;
class QMouseEvent;
class LineNo;
class QTextBlock;

typedef list<LineNo*> LineList;
typedef LineList::iterator LineListIterator;

class LineNoPane : public QWidget {
   
   Q_OBJECT
   
   public:
      LineNoPane(Gui *parent, EditorPane *editorPane);

   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      LineNoDisplay *m_label;
};

class LineNoDisplay : public QWidget {
   
   Q_OBJECT
   
   public:
      LineNoDisplay(EditorPane *editorPane);
      void addToBounds(const QRect &r);

   private slots:
      void fontChanged(const QFont&);
      void editorScrolled(TextEditor*,int);
      void updateLineNumbers();
      void contentChanged(TextEditor *active);
      
   protected:
      EditorPane *m_editorPane;
      TextEditor *m_active;
      LineList m_labels;
      QRect m_rect;
      bool m_first;
      
      // @override
      virtual QSize sizeHint() const;
      virtual QSize minimumSize() const;
};

extern QPixmap *m_breakPoint;
class LineNo : public QLabel {
    
   Q_OBJECT
   
   public:
      friend class LineNoDisplay;
      
      LineNo(EditorPane *editorPane, LineNoDisplay *parent);
      
      QPixmap mergeSideBySide(const QPixmap& pix, const QString &txt);
      void setLineNo(int);
      void resetDisplay(QTextBlock *b = NULL);
      
   protected:
      EditorPane *m_editorPane;
      int m_lineNo;
      QTextBlock *m_last;
      LineNoDisplay *m_parent;
      
      // @override
      virtual void mousePressEvent(QMouseEvent *);
      virtual void mouseReleaseEvent(QMouseEvent *);
      
      // @override
      virtual QSize sizeHint() const;
      virtual QSize minimumSize() const;
};

#endif // __LINE_NO_PANE_H__

