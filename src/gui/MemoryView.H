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
   file: MemoryView.H
   auth: Travis Fischer and Tim O'Donnell
   acct: tfischer, tim
   date: 5/4/2007
\* ---------------------------------------------- */
#ifndef _MEMORY_VIEW_H_
#define _MEMORY_VIEW_H_

#include <QDockWidget>
//#include <QtOpenGL/QtOpenGL>
#include <QHash>

class QGraphicsScene;
class QGraphicsView;
class View;

class State;
class Gui;
class GLMemoryPane;
class Program;
class TempWidget;
class Chip;

class MemoryView : public QDockWidget {
   
   Q_OBJECT
   
   public:
      // Returns true if the system running MipScope supports all of the required OpenGL features
      static bool isSupported();
      
      MemoryView(Gui *gui);
      virtual ~MemoryView();
      
      void reset();
      void updateDisplay(Program *program);
      
      void gotoDeclaration(Chip *chip);

      void toggleWatchpoint(Chip *chip);
      bool hasWatchpoint(Chip *chip) const;
      bool hasWatchpoint(unsigned int addr) const;

      QHash<unsigned int, bool> *getWatchpoints();

      // Returns largest list after removing early accesses
      //unsigned int MemoryUseMap::removeEarlyAccesses(MemoryUseMap *memoryUseMap, TIMESTAMP earliest) const;

   protected:
      Gui *m_gui;
      GLMemoryPane *m_glMemoryPane;
      TempWidget *m_tempWidget;
      
      QSize minimumSizeHint() const;
      QSize sizeHint() const;
      View *m_view;

      void populateScene(Program*);
      void populateDefault();
      void createChips(float *values, unsigned int dataSize, Program *program = NULL);
      QGraphicsScene *m_scene;
      
      // holds whether or not each memory address contains a watchpoint
      QHash<unsigned int, bool> m_watchpoints;
};

#if 0
class GLMemoryPane : public QGLWidget {
   
   Q_OBJECT
   
   public:
      GLMemoryPane(MemoryView *parent, Gui *gui);
      virtual ~GLMemoryPane();
      
      void reset();
      void render(QImage *image);
      QSize minimumSizeHint() const;
      QSize sizeHint() const;
   
   public slots:
      void setXRotation(int angle);
      void setYRotation(int angle);
      void setZRotation(int angle);

   signals:
      void xRotationChanged(int angle);
      void yRotationChanged(int angle);
      void zRotationChanged(int angle);
      
   protected:
      void initializeGL();
      void paintGL();
      void resizeGL(int width, int height);
      void mousePressEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);

   private:
      GLuint createSphere();
      void quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
            GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4);
      void extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
      void normalizeAngle(int *angle);

      GLuint object;
      int xRot;
      int yRot;
      int zRot;
      QPoint lastPos;
      GLuint m_defaultTexture;
      GLuint m_currentTexture;
      
   protected:
      MemoryView *m_parent;
      Gui *m_gui;

};
#endif

#endif // _MEMORY_VIEW_H_

