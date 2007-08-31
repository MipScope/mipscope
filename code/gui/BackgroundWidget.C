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
   file: BackgroundWidget.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#include "BackgroundWidget.H"
#include <QtGui>

// Utility class for displaying custom background images on QWidgets
// -----------------------------------------------------------------
BackgroundWidget::BackgroundWidget(const QString &path, QWidget *parent) : QWidget(parent) {
   setMouseTracking(true);
   m_background = QImage(path);
   if (!m_background.isNull()) m_pixMap = QPixmap::fromImage(m_background);
   setAutoFillBackground(false);
   setAttribute(Qt::WA_NoSystemBackground);
}

//BackgroundWidget::~BackgroundWidget() { }

// @overridden
void BackgroundWidget::paintEvent(QPaintEvent *e) {
   if (m_background.isNull()) return;  
   
   QPainter p(this);
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
}

