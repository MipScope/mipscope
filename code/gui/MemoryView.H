/* ---------------------------------------------- *\
   file: MemoryView.H
   auth: Travis Fischer and Tim O'Donnell
   acct: tfischer, tim
   date: 5/4/2007
\* ---------------------------------------------- */
#ifndef _MEMORY_VIEW_H_
#define _MEMORY_VIEW_H_

#include <QDockWidget>
#include <QGLWidget>

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
      // Returns largest list after removing early accesses
      //unsigned int MemoryUseMap::removeEarlyAccesses(MemoryUseMap *memoryUseMap, TIMESTAMP earliest) const;

   protected:
      Gui *m_gui;
      GLMemoryPane *m_glMemoryPane;
      TempWidget *m_tempWidget;
      
      
      View *m_view;
      void populateScene(Program*);
      void populateDefault();
      void createChips(float *values, unsigned int heapSize, Program *program = NULL);
      QGraphicsScene *m_scene;
};

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

#endif // _MEMORY_VIEW_H_

