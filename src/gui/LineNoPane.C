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
   file: LineNoPane.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007

   Note:  this file is very, very hacky..
\* ---------------------------------------------- */
#include "LineNoPane.H"
#include "BorderLayout.H"
#include "EditorPane.H"
#include "Gui.H"
#include "Utilities.H"
#include <QtGui>
#include "../simulator/ParseNode.H"

// Right-click context-menu for tabs
// ---------------------------------
LineNoPane::LineNoPane(Gui *gui, EditorPane *editorPane)
   : QWidget(), m_gui(gui), m_editorPane(editorPane)
{
   BorderLayout *layout = new BorderLayout();
   m_label = new LineNoDisplay(m_editorPane);
   
   layout->addWidget(editorPane, BorderLayout::Center);
   layout->addWidget(m_label, BorderLayout::West);
   
   setLayout(layout);
}

LineNoDisplay::LineNoDisplay(EditorPane *editorPane) 
   : QWidget(), m_editorPane(editorPane), 
   m_active(m_editorPane->m_activeEditor)
{  
   for(int i = 0; i < 220; i++) {
      LineNo *cur = new LineNo(m_editorPane, this);
      
      m_labels.push_back(cur);
      //addWidget(cur);
   }
   
   /*setFrameShadow(QFrame::Raised);
   setFrameShape(QFrame::StyledPanel);*/
   setFocusPolicy(Qt::NoFocus);
   m_editorPane->m_lineNoDisplay = this;
   
   connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   connect(m_editorPane, SIGNAL(updateLineNumbers(TextEditor*, int)), this, SLOT(editorScrolled(TextEditor*,int)));
   connect(m_editorPane, SIGNAL(activeEditorChanged(TextEditor*)), this, SLOT(contentChanged(TextEditor*)));
   connect(m_editorPane, SIGNAL(contentChanged(TextEditor*)), this, SLOT(contentChanged(TextEditor*)));
   
   updateLineNumbers();
}

QPixmap *m_breakPoint, *m_currentPC, *m_breakPointCurrentPC;

LineNo::LineNo(EditorPane *editorPane, LineNoDisplay *parent) : QLabel(parent), m_editorPane(editorPane), m_lineNo(-1), m_last(NULL), m_parent(parent)
{
   setFont(*m_editorPane->m_font);
   if (m_breakPoint == NULL)
      m_breakPoint = new QPixmap(IMAGES"/breakPoint.png");
   if (m_currentPC == NULL)
      m_currentPC = new QPixmap(IMAGES"/currentPC.png");
   if (m_breakPointCurrentPC == NULL)
      m_breakPointCurrentPC = new QPixmap(IMAGES"/breakPointCurrentPC.png");
   
   setMargin(0);
   setWordWrap(false);
   hide();
}

void LineNo::mousePressEvent(QMouseEvent *e) { e->ignore(); }

void LineNo::mouseReleaseEvent(QMouseEvent *e) {
   if (rect().contains(e->pos()) && (e->button() == Qt::LeftButton)) {
//      cerr << text().toStdString() << " was pressed\n";
//      QPixmap p = mergeSideBySide(*m_breakPoint, text());
      
      QTextBlock *b = m_last = m_editorPane->m_activeEditor->getBlockForLine(m_lineNo, m_last);
      if (b != NULL) {
         int state = b->userState();
         
//         cerr << state;
         
         ParseNode *node = ParseNode::Node(b);
         bool canContainBP = 
            (node != NULL && node->isExecutable());
         
         // Toggle display of breakpoint
         if (canContainBP && state < 0)
            state = B_BREAKPOINT;
         else if (state & B_BREAKPOINT)
            state &= ~B_BREAKPOINT;
         else if (canContainBP)
            state |= B_BREAKPOINT;
         
         b->setUserState(state);
//         cerr << ", vs " << state << endl;
      }
      
      resetDisplay(b);
   }
}

void LineNo::setLineNo(int val) {
   m_lineNo = val;
   
   resetDisplay();
}

void LineNo::resetDisplay(QTextBlock *b) {
   bool possibleBP = !(b == NULL && (b = m_last = m_editorPane->m_activeEditor->getBlockForLine(m_lineNo, m_last)) == NULL);
   bool bp = possibleBP;
   
   QString text;
 
   if (m_lineNo < 10) {
      text = QString("%1").arg(m_lineNo + 1); // start counting at 1  :)
   } else {
      int max = m_editorPane->m_activeEditor->noLines();
      
      text = QString((max > 99 ? "%1" : "%1")).arg(m_lineNo + 1);
   }
   
   if (possibleBP) {
      int state = b->userState();

      if (state == B_BREAKPOINT)
         setPixmap(mergeSideBySide(*m_breakPoint, text));
      else if (state == B_CURRENT_PC)
         setPixmap(mergeSideBySide(*m_currentPC, text));
      else if (state == B_BREAKPOINT_CURRENT_PC)
         setPixmap(mergeSideBySide(*m_breakPointCurrentPC, text));
      else bp = false;
   }
   
   if (!bp)
      setText(text);
}

QPixmap LineNo::mergeSideBySide(const QPixmap& pix, const QString &txt)
{
   QPainter p;
   QPixmap temp(1, 1);
   p.begin(&temp);
   p.setFont(font());
 
   int strWidth = p.fontMetrics().width( txt );
   int strHeight = p.fontMetrics().height();

   p.end();
   
   int pixWidth  = pix.width();
   int pixHeight = pix.height();
   int maxHeight = (strHeight > pixHeight ? strHeight : pixHeight);
   int maxWidth  = (strWidth  > pixWidth  ? strWidth  : pixWidth);
   
   QPixmap res(maxWidth, maxHeight);

//   res.fill(Qt::white);
   QColor c = palette().window().color();
   res.fill(c);//QColor(207, 207, 207, 255));
   
   p.begin(&res);
   p.setFont(font());
   p.drawText(QRect(0, 0, strWidth, strHeight), 0, txt);
   p.drawPixmap(maxWidth - pixWidth, 0, pix);
   p.end();
   
   if (maxWidth > rect().width()) {
      const QRect &r = geometry();
      
      QRect newRect(r.x(), r.y(), maxWidth, maxHeight);
      setGeometry(newRect);
      m_parent->addToBounds(newRect);
   }
   
   return res;
}

QSize LineNo::sizeHint() const {
   const QPixmap *p = pixmap();
   
   if (p != NULL) {
      return p->size();
   }
   
   return QLabel::sizeHint();
}

QSize LineNo::minimumSize() const {
   return sizeHint();
}

void LineNoDisplay::fontChanged(const QFont &font) {
   for(LineListIterator li = m_labels.begin(); li != m_labels.end(); li++) {
      (*li)->setFont(font);
   }
}

void LineNoDisplay::editorScrolled(TextEditor *active, int val) {
   active = NULL;
   val = 0;
   updateLineNumbers();
}

// called whenever active editor is switched or 
// active editor's content is modified
void LineNoDisplay::contentChanged(TextEditor *active) {
   active = NULL;
   updateLineNumbers();
}

void LineNoDisplay::updateLineNumbers() {
   const TextEditor *active = m_editorPane->m_activeEditor;
   if (active == NULL)
      return;  // always remember to bring a towel ;)
   QRect rect = active->visibleRegion().boundingRect();
   int bottom = rect.bottom();
   rect.adjust(2, 0, 0, -2);
   
   QTextCursor firstVisible = active->cursorForPosition(rect.topLeft());
   const QTextCursor &lastVisible = active->cursorForPosition(rect.bottomLeft());
   
   int firstLineNo = active->lineNumber(firstVisible);
   int lastLineNo  = active->lineNumber(lastVisible);
   
   int noLines = active->noLines(), max = noLines;
   if (firstLineNo == lastLineNo && firstLineNo == 0 && noLines > 1) {
      //cerr << "earlyexit\n";
      return;
   }

   /*QString lineNo = "\n\n";
   
   for(int i = firstLineNo; i <= lastLineNo; i++) {
      lineNo += QString("%1\n").arg(i);
   }*/
   
   QTextCursor cur = firstVisible;
   LineListIterator li = m_labels.begin();
   QRect bounds;
   rect = active->cursorRect(firstVisible);
   
   if (firstLineNo >= lastLineNo/* || lastLineNo < 50*/) {
      lastLineNo = firstLineNo + 118; // hack, yippee :)
      rect.adjust(0, 2, 0, 2);
   }
   
   if (lastLineNo > max)
      lastLineNo = max;
   
   max = (max > lastLineNo ? max : lastLineNo);
   
   QLabel maxSized(QString("%1").arg(max));
   maxSized.setFont(*m_editorPane->m_font);
   int width = maxSized.sizeHint().width();// + m_breakPoint->width();
   if (width < m_breakPoint->width() + 2)
      width = m_breakPoint->width() + 2;
   //cerr << max << endl;
   
   int i;
   for(i = 0; i < lastLineNo - firstLineNo + 1; i++) {
      int lineNo = firstLineNo + i;//active->lineNumber(cur);
      
      (*li)->setLineNo(lineNo);//setText(QString("%1").arg(lineNo));
      
      QTabBar *tabBar = m_editorPane->tabBar();
      int offset = tabBar->contentsRect().height() + 2;
      
      QPoint p = rect.adjusted(0, offset, 0, 0).topLeft();
           //if ((*li)->sizeHint().width() > width)
      //   cerr << (*li)->sizeHint().width() << " > " << width << endl;
      
      int customWidth;
      if ((customWidth = (*li)->sizeHint().width()) > width)
         width = customWidth;
      
      QRect r(0, p.y(), width, rect.height());
      (*li)->setGeometry(r);
      
      bounds = bounds.united(r);
      if (i == 0)
         bottom += p.y() + rect.height();
      else if (p.y() > bottom) {
         //cerr << "Breaking; i = ";
         break;
      }
      
      (*li)->show();
      li++;
      
      int oldPos = cur.position();
      cur.movePosition(QTextCursor::Down);
      
      if (cur.position() == oldPos) {
         rect.adjust(0, rect.height(), 0, rect.height());

         if (lastLineNo - firstLineNo + 1 >= i + 1) {
            lastLineNo = 100 + firstLineNo;
            
            if (lastLineNo > noLines)
               lastLineNo = noLines;
      //      cerr << oldPos << endl;
         }
      } else rect = active->cursorRect(cur);
   }
   
   while(li != m_labels.end()) {
      (*li)->hide();
      li++;
   }
   
   //cerr << i << " \t Updating: (" << firstLineNo << ", " << lastLineNo << ")\n";
   //cerr << lineNo.toStdString() << endl;
   
   m_rect = bounds.adjusted(4, 0, (lastLineNo - firstLineNo >= 100 ? 0 : 10), 0);
   
   updateGeometry();
}

void LineNoDisplay::addToBounds(const QRect &r) {
   m_rect = m_rect.united(r);
   
   updateGeometry();
}

QSize LineNoDisplay::minimumSize() const {
   return m_rect.size();
}

QSize LineNoDisplay::sizeHint() const {
   return m_rect.size();
}

