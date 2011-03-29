/****************************************************************************
 **
 ** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the demonstration applications of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/
/* ---------------------------------------------- *\
   file: chip.cpp
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/6/2007

   Modified version of Qt 'chip' demo.
\* ---------------------------------------------- */

#include "chip.h"
#include "../../simulator/ParseNode.H"
#include "view.h"

#include <QtGui>

Chip::Chip(View *view, const QColor &color, int x, int y, 
      unsigned int address, unsigned int value, 
      const QString &label, bool watchpoint, ParseNode *setBy)
: m_view(view), m_address(address), m_value(value), 
   m_setBy(setBy), m_label(label), m_watchpoint(watchpoint)
{
   this->x = x;
   this->y = y;
   this->color = color;
   setZValue((x + y) % 2);
   QString toolTip = QString("Address: 0x%1<br>Value: 0x%2").arg(QString::number(m_address, 16), QString::number(m_value, 16));
   
   if (!m_label.isEmpty())
      toolTip += QString("<br>Within label <u>%1</u>").arg(m_label);

   if (m_setBy != NULL)
      toolTip += QString("<br><br>(Set by '%1')").arg(m_setBy->getTextBlock()->text());
   
   setToolTip(toolTip);
   
   setFlags(ItemIsSelectable);// | ItemIsMovable);
   setAcceptsHoverEvents(true);
}

QRectF Chip::boundingRect() const
{
   return QRectF(0, 0, 100, 60); // 110, 70
}

QPainterPath Chip::shape() const
{
   QPainterPath path;
   path.addRect(14, 14, 82, 42);
   return path;
}

void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   Q_UNUSED(widget);
   
   QColor c2 = (m_watchpoint ? QColor(206, 21, 2) : color);
   QColor fillColor = (option->state & QStyle::State_Selected) ? c2.dark(180)/*QColor(250, 251, 187)*/ : c2;

   if (option->state & QStyle::State_MouseOver)
      fillColor = fillColor.light(125);

   if (option->levelOfDetail < 0.2) {
      if (option->levelOfDetail < 0.125) {
         painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
         return;
      }

      painter->setPen(QPen(Qt::black, 0));
      painter->setBrush(fillColor);
      painter->drawRect(13, 13, 97, 57);
      return;
   }

   QPen oldPen = painter->pen();
   QPen pen = oldPen;
   int width = 0;
   if (option->state & QStyle::State_Selected)
      width += 2;

   pen.setWidth(width);
   painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

   painter->drawRect(QRect(14, 14, 79, 39));
   if (option->levelOfDetail >= 0.2f) {
      painter->setPen(QPen(Qt::gray, 1));
      painter->drawLine(15, 54, 94, 54);
      painter->drawLine(94, 53, 94, 15);
      painter->setPen(QPen(Qt::black, 0));
   }

   // Draw text
   if (option->levelOfDetail >= 0.2f) {
      QFont font("Times", 28);
      font.setStyleStrategy(QFont::ForceOutline);
      painter->setFont(font);
      painter->setRenderHint(QPainter::TextAntialiasing, true); // false
      painter->save();
      painter->scale(0.2, 0.2);
      
      painter->drawText(85, 100, QString("Address: 0x%1").arg(QString::number(m_address, 16)));
      painter->drawText(85, 132, QString("Value: 0x%1").arg(QString::number(m_value, 16)));

      if (!m_label.isEmpty())
         painter->drawText(85, 164, QString("Within label '%1'").arg(m_label));
      else if (m_setBy != NULL)
         painter->drawText(85, 164, QString("(Set by '%1')").arg(m_setBy->getTextBlock()->text()));
     
      painter->restore();
   }

#if 0
   // Draw lines
   QVarLengthArray<QLineF, 36> lines;
   if (option->levelOfDetail >= 0.2) {
      for (int i = 0; i <= 10; i += (option->levelOfDetail > 0.2 ? 1 : 2)) {
         lines.append(QLineF(18 + 7 * i, 13, 18 + 7 * i, 5));
         lines.append(QLineF(18 + 7 * i, 54, 18 + 7 * i, 62));
      }
      for (int i = 0; i <= 6; i += (option->levelOfDetail > 0.2 ? 1 : 2)) {
         lines.append(QLineF(5, 18 + i * 5, 13, 18 + i * 5));
         lines.append(QLineF(94, 18 + i * 5, 102, 18 + i * 5));
      }
   }
   if (option->levelOfDetail >= 0.2) {

#define LBASE  (15)
#define VBASE  (3)

      const QLineF lineData[] = {
         QLineF(LBASE+25, VBASE+35, LBASE+35, VBASE+35),
         QLineF(LBASE+35, VBASE+30, LBASE+35, VBASE+40),
         QLineF(LBASE+35, VBASE+30, LBASE+45, VBASE+35),
         QLineF(LBASE+35, VBASE+40, LBASE+45, VBASE+35),
         QLineF(LBASE+45, VBASE+30, LBASE+45, VBASE+40),
         QLineF(LBASE+45, VBASE+35, LBASE+55, VBASE+35)
      };
      lines.append(lineData, 6);
   }
   painter->drawLines(lines.data(), lines.size());
#endif

   if (m_watchpoint)
      painter->drawPixmap(18, 40, *WATCHPOINT);
   
/* // Draw red ink
   if (stuff.size() > 1) {
      painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->setBrush(Qt::NoBrush);
      QPainterPath path;
      path.moveTo(stuff.first());
      for (int i = 1; i < stuff.size(); ++i)
         path.lineTo(stuff.at(i));
      painter->drawPath(path);
   }*/
}

void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mousePressEvent(event);
   if (event->button() & Qt::LeftButton) {
//      m_view->view()->centerOn(this);
      
      m_view->zoomInOn(this, event);
//      m_view->setZoom(320); // zoom in on this chip :)
   }

   update();
}

void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->modifiers() & Qt::ShiftModifier) {
//      stuff << event->pos();
      update();
      return;
   }
   QGraphicsItem::mouseMoveEvent(event);
}

void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mouseReleaseEvent(event);
   update();
}

void Chip::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
   m_view->showContextMenu(event->screenPos(), this);
}

unsigned int Chip::getAddress() const {
   return m_address;
}

void Chip::setWatchpointEnabled(bool enabled) {
   m_watchpoint = enabled;
   update();
}

