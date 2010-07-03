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
   file: view.h
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/6/2007

   Modified version of Qt 'chip' demo.
\* ---------------------------------------------- */

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QBasicTimer>
#include <QList>
#include <QTime>

class QContextMenuEvent;
class QMenu;
class QGraphicsView;
class QLabel;
class QSlider;
class QToolButton;
class QTimerEvent;
class Chip;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QPoint;
class MemoryView;
class QGraphicsScene;
class QAction;
class QGraphicsItem;

class View : public QFrame
{
   Q_OBJECT

   public:
      View(const QString &name, MemoryView *parent);

      QGraphicsView *view() const;
      void setScene(QGraphicsScene *scene);
      void setZoom(int value);
      void zoomInOn(Chip *chip, QGraphicsSceneMouseEvent *event);

      void showContextMenu(const QPoint &pos, Chip *active);
      void showContextMenu(QGraphicsSceneContextMenuEvent *event);
      
   protected:
      void timerEvent(QTimerEvent *event);
      void setupContextMenu();
    //  void contextMenuEvent(QContextMenuEvent *event);
      
   private slots:
//      void resetView();
//      void setResetButtonEnabled();
      void setupMatrix();
      void toggleOpenGL();
      void toggleAntialiasing();
      void print();

      void zoomIn();
      void zoomOut();
//      void rotateLeft();
//      void rotateRight();

      void gotoDeclarationAction();
      void toggleWatchpointAction();
      void selectionChanged();

   private:
      QGraphicsView *graphicsView;
//      QLabel *label;
      QToolButton *openGlButton;
      QToolButton *antialiasButton;
      QToolButton *printButton;
//      QToolButton *resetButton;
      QSlider *zoomSlider;
 //     QSlider *rotateSlider;
      QAction *m_toggleWatchpointAction;

      Chip *m_active, *m_contextChip;
      QBasicTimer m_timer;
      MemoryView *m_parent;
      QMenu *m_contextMenu;
//      QList<QGraphicsItem*> m_oldSelectedItems;
      QList<QGraphicsItem*> m_selectedItems, m_oldSelectedItems;
      QTime m_selectionTime;
};

#endif
