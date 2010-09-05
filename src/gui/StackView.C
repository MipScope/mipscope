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
   file: StackView.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#include "StackView.H"
#include "EditorPane.H"
#include "Gui.H"
#include "../simulator/State.H"
#include "../simulator/ParseNode.H"
#include <QPainter>
#include <QList>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>

// TODO:
//    MemoryView
// 

// View of the Stack for the currently running program
// ---------------------------------------------------
StackView::StackView(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("Stack View"), gui), m_gui(gui), m_editorPane(editorPane), 
   m_dirty(true),           // whether or not display is up-to-date
   m_sp(STACK_BASE_ADDRESS) // current value of $sp
{
   QDockWidget::setObjectName(tr("Stack View"));
   m_display = new CustomTextEdit(this, QString(IMAGES"/stackBackground.jpg"));
   m_display->setFont(*m_editorPane->font());
   reset();

   setWidget(m_display);

   QDockWidget::connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
}

void StackView::reset() {
   m_display->setPlainText("");
   m_sp = STACK_BASE_ADDRESS;
   m_modifiedMap.clear();
}

void StackView::registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc) {
   if (reg != sp)
      return;
   
   m_dirty = true;
   m_sp = value;
   status = 0;
   // TODO:  record pc which last changed $sp
}

void StackView::memoryChanged(unsigned int address, unsigned int value, ParseNode *pc) {
   if (address >= m_sp + 4) {
      m_dirty = true;
      value = 0;
   }
   
   m_modifiedMap[address] = pc;
}

void StackView::updateDisplay(State *s, int state) {
   if (m_dirty && state != RUNNING) {
      s->getStack(m_stack);
      
      
      int cur_index = 0, size = m_stack.size();
      
      const unsigned int base = 16;  // hex
      const QString &longestAddr = QString::number(STACK_BASE_ADDRESS - 4, base);
      const int maxAddrLength = longestAddr.length();
      
      m_display->setUpdatesEnabled(false);
      m_display->setPlainText("");
      QString text;
      
      unsigned int address = STACK_BASE_ADDRESS - 4;
      foreach(int value, m_stack) {
         QString addr = QString::number(address, base);
         for(int max = addr.length(), i = maxAddrLength; i > max; i--)
            addr += " ";
         
         const QString &valString = QString::number(value, base);
         text = QString(": %1\n").arg(valString);
         
         QTextCursor c = m_display->textCursor();
         c.movePosition(QTextCursor::End);
         //int oldEnd = c.position();
         QTextCharFormat format(m_display->currentCharFormat());

         if (cur_index++ > 200) {
            if (VERBOSE)
               cerr << "STACK_SIZE: " << m_stack.size() << endl;
            c.insertText(QString("... STACK IS TOO LARGE TO DISPLAY ...\n"
                                 "You may be in an infinite loop. Stack size: %1 words").arg(QString::number(size)));
               /*QMessageBox::warning(this, tr("Stack warning"), 
                     QString("Stack is too large to display.\n"
                        "You may be in an infinite loop. Stack size: %1 words").arg(QString::number(size)));*/
            break;
         }


         // TODO:  Have tip tell which line last modified it!
         QString tip = QString("Hex: %1: %2<br>"
               "Dec: %3: %4").arg(addr, valString, 
                  QString::number(address, 10), 
                  QString::number(value, 10));
         if (address == m_sp) {
            format.setFontWeight(100);
            tip += "<br>(<b>$sp</b>)";
         }
         if (m_modifiedMap.contains(address)) {
            ParseNode *last = m_modifiedMap[address];
            TextEditor *active = NULL;

            if (last != NULL && last->isValid() && last->getTextBlock() != NULL && (active = m_gui->getActiveProgram()) != NULL) {
               tip += QString("<br>"
                     "(Set by line <i>%1</i>)").arg(1 + active->lineNumber(*last->getTextBlock()));
            }
         }
         format.setToolTip(tip);

         c.insertText(addr, format);
         if (address == m_sp)
            format.setFontWeight(50);
         c.insertText(text, format);

         address -= 4;
      }

      m_display->setUpdatesEnabled(true);
      //      m_display->setPlainText(text);
   }
}

void StackView::fontChanged(const QFont &newFont) {
   m_display->setFont(newFont);
}

   CustomTextEdit::CustomTextEdit(StackView *parent, const QString &background)
: QTextEdit(parent), m_stackView(parent)
{
   m_background = QImage(background);
   if (!m_background.isNull()) m_pixMap = QPixmap::fromImage(m_background);

   setReadOnly(true);
   setAcceptRichText(false);
   setLineWrapMode(QTextEdit::NoWrap);

   setAutoFillBackground(false);
   setAttribute(Qt::WA_NoSystemBackground);
}

// @overridden
void CustomTextEdit::paintEvent(QPaintEvent *e) {
   if (!m_background.isNull()) {
      QPainter p(viewport());
      QRect r(QPoint(0, 0), m_pixMap.size());
      const QRect &bounds = rect();
      bool resizeH = (bounds.width() > r.width());
      bool resizeV = (bounds.height() > r.height());

      // ensure background pic always takes up whole frame, scaling if necessary
      if (resizeH || resizeV) {
         m_pixMap = QPixmap::fromImage(m_background.scaled(resizeH ? bounds.width() : r.width(), resizeV ? bounds.height() : r.height()));

         r.setSize(m_pixMap.size());
      }

      r.moveCenter(rect().center());
      p.drawPixmap(r, m_pixMap);
      p.end();
   }
   QTextEdit::paintEvent(e);
}

/*c.setPosition(oldEnd);
  c.select(QTextCursor::LineUnderCursor);
//         c.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

QTextCharFormat format(m_display->currentCharFormat());
format.setToolTip(QString("Hex: %1: %2<br>"
"Dec: %3: %4").arg(addr, valString, 
QString::number(address, 10), 
QString::number(value, 10)));
format.setUnderlineStyle(QTextCharFormat::SingleUnderline);

const struct QTextEdit::ExtraSelection newSelection = {
QTextCursor(c), 
format, //QTextCharFormat(f), 
};

highlights.push_back(newSelection);


QList<QTextEdit::ExtraSelection> highlights;
m_display->setExtraSelections(highlights);
*/

