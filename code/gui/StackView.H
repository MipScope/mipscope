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
   file: StackView.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/29/2007
\* ---------------------------------------------- */
#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "Utilities.H"
#include <QDockWidget>
#include <QTextEdit>
#include <QVector>
#include <QPixmap>
#include <QImage>
#include <QHash>

class ParseNode;
// For keeping track of the last line to modify: register/memory/stack
typedef QHash<unsigned int, ParseNode*> ModifedMap;
typedef ModifedMap::iterator ModifedMapIterator;

class Gui;
class EditorPane;
class State;
class ParseNode;
class CustomTextEdit;
class QFont;
class QPaintEvent;

class StackView : public QDockWidget {

   Q_OBJECT
   
   public:
      StackView(Gui *parent, EditorPane *editorPane);
      
      void updateDisplay(State *s, int state);
      
   public slots:
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc);
      void memoryChanged(unsigned int address, unsigned int value, ParseNode *pc);
      
   private slots:
      void reset();
      
      // Keeps the console's font in sync w/ that of the EditorPane
      void fontChanged(const QFont &newFont);
      
   protected:
      QVector<int> m_stack; // values on the stack
      
      Gui *m_gui;
      EditorPane *m_editorPane;
      
      CustomTextEdit *m_display;
      bool m_dirty;
      unsigned int m_sp; // current (always) value of stack pointer register
      
      // Keeps track of which ParseNode is responsible for last modifying 
      // a specific memory address
      ModifedMap m_modifiedMap;
};

class CustomTextEdit : public QTextEdit {
   public:
      CustomTextEdit(StackView *parent, const QString &background);

   protected:
      StackView *m_stackView;
      QPixmap m_pixMap;
      QImage m_background;

      // @overridden
      virtual void paintEvent(QPaintEvent *e);
};

#endif // __STACK_VIEW_H__

