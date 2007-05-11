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
   file: BackgroundWidget.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#ifndef __BACKGROUND_WIDGET_H__
#define __BACKGROUND_WIDGET_H__
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include "Utilities.H"

class QPixmap;
class QImage;
class QPaintEvent;

class BackgroundWidget : public QWidget {

   Q_OBJECT
   
   public:
      BackgroundWidget(const QString &path, QWidget *parent = NULL);
      //virtual ~BackgroundWidget();

   protected:
      QPixmap m_pixMap;
      QImage m_background;
      
      // @overridden
      virtual void paintEvent(QPaintEvent *e);
};

#endif // __BACKGROUND_WIDGET_H__

