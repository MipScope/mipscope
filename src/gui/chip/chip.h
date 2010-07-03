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
   file: chip.h
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/6/2007

   Modified version of Qt 'chip' demo.
\* ---------------------------------------------- */

#ifndef CHIP_H
#define CHIP_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

class ParseNode;
class View;
class QPixmap;

extern QPixmap *WATCHPOINT;

class Chip : public QGraphicsItem
{
   public:
      Chip(View *view, const QColor &color, int x, int y, unsigned int address, 
            unsigned int value, const QString &label, bool, ParseNode *setBy = NULL);

      QRectF boundingRect() const;
      QPainterPath shape() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
      unsigned int getAddress() const;
      void setWatchpointEnabled(bool);

   protected:
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
      void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
      
   private:
      int x, y;
      QColor color;
//      QList<QPointF> stuff;

      View *m_view;
      unsigned int m_address;
      unsigned int m_value;
      ParseNode *m_setBy;
      QString m_label;
      bool m_watchpoint;
};

#endif

