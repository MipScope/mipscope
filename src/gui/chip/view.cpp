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
   file: view.cpp
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/6/2007

   Modified version of Qt 'chip' demo.
\* ---------------------------------------------- */

#include "view.h"
#include "chip.h"
#include <QGraphicsSceneMouseEvent>
#include "../MemoryView.H"
#include "../Utilities.H"
#include <assert.h>

#include <QtGui>

#define QT_NO_OPENGL
#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#endif

#include <math.h>

View::View(const QString &name, MemoryView *parent)
   : QFrame(parent), m_active(NULL), m_contextChip(NULL), 
   m_parent(parent), m_contextMenu(NULL)
{
   setFrameStyle(Sunken | StyledPanel);
   
   graphicsView = new QGraphicsView;
   graphicsView->setRenderHint(QPainter::Antialiasing, true);
   graphicsView->setRenderHint(QPainter::TextAntialiasing, true);
   graphicsView->setDragMode(QGraphicsView::RubberBandDrag);//NoDrag
   graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
   
//   graphicsView->setBackgroundBrush(QPixmap(":/stackBackground.jpg"));

   //int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
   QSize iconSize(16, 16);//size, size);

   QToolButton *zoomInIcon = new QToolButton;
   zoomInIcon->setAutoRepeat(true);
   zoomInIcon->setAutoRepeatInterval(16);
   zoomInIcon->setAutoRepeatDelay(0);
   zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
   zoomInIcon->setIconSize(iconSize);
   QToolButton *zoomOutIcon = new QToolButton;
   zoomOutIcon->setAutoRepeat(true);
   zoomOutIcon->setAutoRepeatInterval(16);
   zoomOutIcon->setAutoRepeatDelay(0);
   zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
   zoomOutIcon->setIconSize(iconSize);
   zoomSlider = new QSlider;
   zoomSlider->setMinimum(80);
   zoomSlider->setMaximum(300);
   zoomSlider->setValue(162);
   zoomSlider->setTickPosition(QSlider::TicksRight);
   zoomSlider->setStyle(new QCleanlooksStyle());

   // Zoom slider layout
   QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
   zoomSliderLayout->addWidget(zoomInIcon);
   zoomSliderLayout->addWidget(zoomSlider);
   zoomSliderLayout->addWidget(zoomOutIcon);

/*   QToolButton *rotateLeftIcon = new QToolButton;
   rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
   rotateLeftIcon->setIconSize(iconSize);
   QToolButton *rotateRightIcon = new QToolButton;
   rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
   rotateRightIcon->setIconSize(iconSize);
   rotateSlider = new QSlider;
   rotateSlider->setOrientation(Qt::Horizontal);
   rotateSlider->setMinimum(-360);
   rotateSlider->setMaximum(360);
   rotateSlider->setValue(0);
   rotateSlider->setTickPosition(QSlider::TicksBelow);

   // Rotate slider layout
   QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
   rotateSliderLayout->addWidget(rotateLeftIcon);
   rotateSliderLayout->addWidget(rotateSlider);
   rotateSliderLayout->addWidget(rotateRightIcon);*/

   /*resetButton = new QToolButton;
   resetButton->setText(tr("0"));
   resetButton->setEnabled(false);*/

   // Label layout
   QHBoxLayout *labelLayout = new QHBoxLayout;
//   label = new QLabel(name);
   antialiasButton = new QToolButton;
   antialiasButton->setText(tr("Antialiasing"));
   antialiasButton->setCheckable(true);
   antialiasButton->setChecked(true);
#ifndef QT_NO_OPENGL
   openGlButton = new QToolButton;
   openGlButton->setText(tr("OpenGL"));
   openGlButton->setCheckable(true);
   openGlButton->setEnabled(QGLFormat::hasOpenGL());
   if (openGlButton->isEnabled()) {
      openGlButton->setChecked(true);
      graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
   }
#endif
   printButton = new QToolButton;
   printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));
   printButton->setToolTip(tr("Print Memory"));

//   labelLayout->addWidget(label);
   labelLayout->addWidget(antialiasButton);
#ifndef QT_NO_OPENGL
   labelLayout->addWidget(openGlButton);
#endif
   labelLayout->addStretch();
   labelLayout->addWidget(printButton);

   QGridLayout *topLayout = new QGridLayout;
   topLayout->addLayout(labelLayout, 0, 0);
   topLayout->addWidget(graphicsView, 1, 0);
   topLayout->addLayout(zoomSliderLayout, 1, 1);
   topLayout->setMargin(3);
   
//   topLayout->addLayout(rotateSliderLayout, 2, 0);
//   topLayout->addWidget(resetButton, 2, 1);
   setLayout(topLayout);

//   connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
   connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
//   connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
//   connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
//   connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
   connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
#ifndef QT_NO_OPENGL
   connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
#endif
   //connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
   //connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
   connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
   connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
   connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
   
   m_selectionTime.start();
   //setContextMenuPolicy(Qt::CustomContextMenu);
   setupContextMenu();
   setupMatrix();
}

QGraphicsView *View::view() const
{
   return graphicsView;
}

void View::setScene(QGraphicsScene *scene) {
   if (scene == NULL)
      return;

   m_timer.stop();
   bool wasFocusing = (m_active != NULL);
   QPointF location = (wasFocusing ? m_active->mapToScene(m_active->boundingRect().center()) : QPointF());
   m_active = NULL;
   m_contextChip = NULL;
   graphicsView->setScene(scene);
   connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

   // attempt to keep zooming in on the same chip, even through scene changes
   if (wasFocusing) {
      QGraphicsItem *item = scene->itemAt(location);
      //cerr << "WAS_FOCUSING: " << item << endl;
      
      if (item != NULL) {
         m_active = dynamic_cast<Chip*>(item);

         if (m_active != NULL)
            m_timer.start(20, this);
      }
   }
}

/*void View::resetView()
{
   m_timer.stop();
   zoomSlider->setValue(250);
   rotateSlider->setValue(0);
   setupMatrix();
   graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

   resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
   resetButton->setEnabled(true);
}*/

void View::setupMatrix()
{
   qreal scale = ::pow(2.0, (zoomSlider->value() - 250) / 50.0);

   QMatrix matrix;
   matrix.scale(scale, scale);
//   matrix.rotate(rotateSlider->value());   // no rotation -- kinda pointless; just takes up extra room

   graphicsView->setMatrix(matrix);
   //setResetButtonEnabled();
}

void View::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
   graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void View::toggleAntialiasing()
{
   graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
   graphicsView->setRenderHint(QPainter::TextAntialiasing, antialiasButton->isChecked());
}

void View::print()
{
   QPrinter printer;
   QPrintDialog dialog(&printer, this);
   if (dialog.exec() == QDialog::Accepted) {
      QPainter painter(&printer);
      graphicsView->render(&painter);
   }
}

#include <iostream>
using namespace std;
#define TARGET_ZOOM     (300)

void View::zoomInOn(Chip *chip, QGraphicsSceneMouseEvent *event) {
   m_active = chip;
   graphicsView->centerOn(chip);
   
   if (event->button() & Qt::LeftButton) {
      m_timer.start(20, this);
   } else {
      //cerr << "else\n";
      m_timer.stop();
      setZoom(TARGET_ZOOM);
   }
}

void View::zoomIn()
{
   zoomSlider->setValue(zoomSlider->value() + 1);
}

void View::zoomOut()
{
//   cerr << zoomSlider->value() << endl;
   zoomSlider->setValue(zoomSlider->value() - 1);
}

void View::setZoom(int value) {
   zoomSlider->setValue(value);
}

/*void View::rotateLeft()
{
   rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
   rotateSlider->setValue(rotateSlider->value() + 10);
}*/

void View::timerEvent(QTimerEvent *event) {
   int target = TARGET_ZOOM;
   int val = zoomSlider->value();
   
   graphicsView->centerOn(m_active);
   if (val < target - 1)
      zoomSlider->setValue(val + 2);
   else if (val > target + 1)
      zoomSlider->setValue(val - 2);
   else {
      m_timer.stop();//killTimer(event->timerId());
      m_active = NULL;
   }
}

void View::showContextMenu(QGraphicsSceneContextMenuEvent *event) {
   QGraphicsItem *item = graphicsView->itemAt(graphicsView->mapFromScene(event->scenePos()));

   //cerr << m_selectionTime.elapsed() << ", items: " << m_oldSelectedItems.size() << "  (item = " << item << endl;
   bool shortTime = (m_selectionTime.elapsed() <= 50);

   if (item == NULL && m_selectedItems.isEmpty() && (!shortTime || m_oldSelectedItems.isEmpty())) {
      event->ignore();
      return;
   }

   if (shortTime) {
      if (item == NULL)
         item = m_oldSelectedItems.front();

      foreach(QGraphicsItem *item, m_oldSelectedItems)
         item->setSelected(true);
   }
   
   event->accept();
   showContextMenu(event->screenPos(), (Chip*) item);
}

void View::showContextMenu(const QPoint &pos, Chip *active) {
   m_contextChip = active;
   int no = 0;
   
//   m_selectedItems = graphicsView->scene()->selectedItems();
   no = m_selectedItems.size();
/*   foreach(QGraphicsItem *item, graphicsView->scene()->selectedItems()) {
//      Chip *chip = (Chip*)item;
      
      ++no;
   }*/
   
   if (no <= 1) {
      if (m_parent->hasWatchpoint(active))
         m_toggleWatchpointAction->setText("Disable watchpoint");
      else m_toggleWatchpointAction->setText("Enable watchpoint");
   } else {
      if (m_contextChip == NULL)
         m_contextChip = (Chip*)m_selectedItems.front();
      m_toggleWatchpointAction->setText("Toggle watchpoints");
   }

   m_contextMenu->popup(pos);
}

void View::setupContextMenu() {
   QMenu *menu = new QMenu(this);
   QAction *gotoAction = new QAction(QIcon(ICONS"/editGotoDeclaration.png"), "Goto .data Declaration", this);
   m_toggleWatchpointAction = new QAction(QIcon(ICONS"/editMemoryWatchpoint.png"), "Enable watchpoint", this);
   
   connect(gotoAction, SIGNAL(triggered()), this, SLOT(gotoDeclarationAction()));
   connect(m_toggleWatchpointAction, SIGNAL(triggered()), this, SLOT(toggleWatchpointAction()));
   
   menu->addAction(gotoAction);
   menu->addAction(m_toggleWatchpointAction);
   m_contextMenu = menu;
}

void View::toggleWatchpointAction() {
   if (m_contextChip == NULL)
      return;
   
   bool found = false;
   foreach(QGraphicsItem *item, m_selectedItems) {
      Chip *chip = (Chip*)item;
      
      m_parent->toggleWatchpoint(chip);
      if (chip == m_contextChip)
         found = true;
   }

   if (!found)
      m_parent->toggleWatchpoint(m_contextChip);
}

void View::gotoDeclarationAction() {
   if (m_contextChip == NULL)
      return;

   m_parent->gotoDeclaration(m_contextChip);
}

void View::selectionChanged() {
   m_oldSelectedItems = m_selectedItems;
   m_selectedItems = graphicsView->scene()->selectedItems();
   m_selectionTime.restart();

   //cerr << m_selectedItems.size() << endl;
}

/*void View::contextMenuEvent(QContextMenuEvent *event) {
   cerr << m_selectedItems.size() << endl;
}*/

