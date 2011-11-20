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
   file: MemoryView.C
   auth: Travis Fischer
   acct: tfischer
   date: 5/4/2007
\* ---------------------------------------------- */
#include "MemoryView.H"
#include "Utilities.H"
#include "Gui.H"
#include "Program.H"
#include "../simulator/State.H"
#include "../simulator/Debugger.H"
#include "../simulator/typedefs.H"
#include <math.h>

#include "chip/view.h"
#include "chip/chip.h"
#include "chip/scene.h"

#include <QtGui>
//#include <QtOpenGL/QtOpenGL>
//using namespace QGL;

// Comment or uncomment to switch between 2d, bird's-eye view and a texture-mapped sphere
//    I like the 2d version better, but the sphere was still a cool idea.
#define USE_BIRDS_EYE

#define SIZE    (128)

class TempWidget : public QWidget {
   friend class MemoryView;
   
   public:
      TempWidget(MemoryView *parent) : QWidget(parent), m_parent(parent), 
         m_pixMap(128, 128)
      {
         setAutoFillBackground(false);
         setAttribute(Qt::WA_NoSystemBackground);
      }
      
      void render(QImage *image) {
         if (image->isNull()) return;  
         m_pixMap = QPixmap::fromImage(*image);
         update();
      }
      
   protected:
      MemoryView *m_parent;
      QPixmap m_pixMap;
//      QImage m_background;

      // @overridden
      void paintEvent(QPaintEvent *e) {
         QPainter p(this);
         QRect r(QPoint(0, 0), m_pixMap.size());
         //const QRect &bounds = rect();
         //bool resizeH = (bounds.width() > r.width());
         //bool resizeV = (bounds.height() > r.height());

         // ensure background pic always takes up whole frame, scaling if necessary
/*         if (resizeH || resizeV) {
            m_pixMap = QPixmap::fromImage(m_background.scaled(resizeH ? bounds.width() : r.width(), resizeV ? bounds.height() : r.height()));

            r.setSize(m_pixMap.size());
         }*/

         r.moveCenter(rect().center());
         p.drawPixmap(r, m_pixMap);
      }
};


bool MemoryView::isSupported() {
//   cerr << "hasOpenGL: " << QGLFormat::hasOpenGL() << ", hasPBuffers: " << QGLPixelBuffer::hasOpenGLPbuffers() << endl;
   
   return true;//(QGLFormat::hasOpenGL());// && QGLPixelBuffer::hasOpenGLPbuffers());
}

MemoryView::MemoryView(Gui *gui) 
   : QDockWidget(tr("Memory View"), gui), m_gui(gui), 
   /*m_glMemoryPane(NULL),*/ m_view(NULL), m_scene(NULL)
{
   QDockWidget::setObjectName(tr("Memory View"));
   
   Q_INIT_RESOURCE(memoryImages);
#ifdef USE_BIRDS_EYE
   m_view = new View(tr("Memory View"), this);
   setWidget(m_view);
   populateDefault();
#else
   m_glMemoryPane = new GLMemoryPane(this, m_gui);
   setWidget(m_glMemoryPane);
#endif
   
   // For testing purposes
   //m_tempWidget = new TempWidget(this);
   //setWidget(m_tempWidget);
}

MemoryView::~MemoryView() {
   //safeDelete(m_glMemoryPane);
   safeDelete(m_view);
}

void MemoryView::gotoDeclaration(Chip *chip) {
   if (chip == NULL)
      return;
   
   unsigned int address = chip->getAddress();
   m_gui->gotoDeclaration(address);
}

#ifndef USE_BIRDS_EYE
unsigned char m_data[128 * 128 * 4];
#endif

void MemoryView::updateDisplay(Program *program) {
//#ifdef USE_BIRDS_EYE
   
   populateScene(program);
   
//#else
#if 0
   
   MemoryUseMap *memoryUseMap = program->getMemoryUseMap();
//   unsigned int blockSize = (unsigned int)ceilf(sqrt(noAddresses));
   unsigned int dataSize  = program->getDataSize();
   unsigned int noAddresses = dataSize >> 2;//memoryUseMap->size();
   unsigned int maxData = DATA_BASE_ADDRESS + dataSize;
//   unsigned int noStackAddresses  = noAddresses - (dataSize >> 2);
//   unsigned int noDataAddresses = noAddresses - noStackAddresses;
   
   TIMESTAMP current = program->getCurrentTimestamp();
   unsigned int earliest = 0;//current - 100; // earliest timestamp to consider
   
//   cerr << "UPDATEDISPLAY:  noAddr: " << noAddresses << /*", noData: " << noDataAddresses << 
//      ", noStack: " << noStackAddresses << */ ", dataSize: " << dataSize << "\n";
   
//   memset(m_buffer, 0, sizeof(float) * 128 * 128);
//   memset(m_norms, 0, sizeof(float) * 128 * 128);
   
   //QList<unsigned int> toRemove;
   unsigned int largestList = 1;
   
   QHash<unsigned int, unsigned int> addressesToAccesses;
   unsigned int lastValidAddr = DATA_BASE_ADDRESS;
 
   for(unsigned int address = DATA_BASE_ADDRESS; address < maxData; address += 4) {
      if (!memoryUseMap->contains(address)) {
         addressesToAccesses.insert(address, 0);
         continue;
      }
      QList<TIMESTAMP> *list = memoryUseMap->value(address);
      
      if (list == NULL || address > maxData)
         continue;
      unsigned int accesseses = list->size();
      foreach(TIMESTAMP stamp, *list) {
        if (stamp < earliest)
           accesseses--;
        else break;
      }

      if (accesseses > 0) {
         addressesToAccesses.insert(address, accesseses);

         if (accesseses > largestList)
            largestList = accesseses;
         if (address > lastValidAddr)
            lastValidAddr = address;
      }
   }

   float mostAccesses = largestList;

//   cerr << "noA: " << noAddresses << ", mostAccessed: " << mostAccesses << endl;
   
//   noAddresses = (lastValidAddr - DATA_BASE_ADDRESS) >> 2;
   float *values = (float*)malloc(sizeof(float) * noAddresses);
   memset(values, 0, sizeof(float) * noAddresses);
   int *occurrences = (int*)malloc(sizeof(int) * noAddresses);
   memset(occurrences, 0, sizeof(int) * noAddresses);
    
   for(unsigned int address = DATA_BASE_ADDRESS; address < maxData; address += 4) {
      if (!addressesToAccesses.contains(address))
         continue;
   //foreach(unsigned int address, addressesToAccesses.keys()) {
      unsigned int size = addressesToAccesses[address];

      float val = ((float)size) / mostAccesses;
      unsigned int index;

      // data section
      index = (unsigned int)((address - DATA_BASE_ADDRESS) >> 2);
      
      if (index >= noAddresses)
         index = noAddresses - 1;
      
      occurrences[index]++;
      values[index] = val;
   }

   for(unsigned int i = 0; i < noAddresses; i++) {
      if (occurrences[i] != 0)
         values[i] /= occurrences[i];
   } // take average
   
   const int width = 128, height = width;
   QImage *image = new QImage(width, height, QImage::Format_RGB32);
   QImage activityGradient(IMAGES"/activityGradient.png");
   
   for(unsigned int i = 0; i < (128 * 128); i++) {
      float sum = 0;
      
      for(int a = -1; a <= 1; a++) {
         if (i < 128 && a < 0)
            continue;
         for(int b = -1; b <= 1; b++) {
            unsigned long long index = i + ((signed)a * 128 + b);
            
            if (index < (128 * 128)) {
               unsigned int key = (unsigned int)((index * (unsigned long long)noAddresses) >> 14);
               assert(key < noAddresses);
               
               sum += values[key];
            }
         }
      }
      
      sum /= 7;
      /*
      for(int a = -1; a <= 1; a++) {
         for(int b = -1; b <= 1; b++) {
            if (i < 128 && a < 0)
               continue;
            
            unsigned int index = i + 128 * a + b;
            
            if (index > 0 && index < (128 * 128)) {
               unsigned int normalized = (unsigned int) round(((index) * (float)noAddresses) / (128 * 16));
               
               val += values[normalized];
            }
         }
      }*/
      
      //val /= 9;
      
      unsigned char scale = 255;
      if (sum < 1)
         scale = (unsigned char)(255 * sum);
      
      /*m_data[i << 2] = scale;
      m_data[(i << 2) + 1] = scale;
      m_data[(i << 2) + 2] = scale;
      m_data[(i << 2) + 3] = 0;*/
      
      QColor color = QColor(activityGradient.pixel(scale, 0));


//      unsigned int rgb = scale | (scale << 8) | (scale << 16) | (0xff << 24);
      image->setPixel(i & 127, i / 128, color.rgb());//qRgb(scale, scale, scale));
   }
   
   //cerr << "last: " << last << ", (noAddr=" << noAddresses << ")\n";//endl;

   //memset(image.bits(), 0, image.numBytes() * sizeof(unsigned char));
   
   //m_tempWidget->render(image);
   //m_glMemoryPane->render(image);
   
   delete image;
   free(values);
   free(occurrences);
#endif
}

QHash<unsigned int, bool> *MemoryView::getWatchpoints() {
   return &m_watchpoints;
}

void MemoryView::reset() {
//   m_watchpoints.clear();

   //populateDefault();
#ifndef USE_BIRDS_EYE
   m_glMemoryPane->reset();
#endif
}

QSize MemoryView::minimumSizeHint() const {
   return QSize(10, 10);
}

QSize MemoryView::sizeHint() const {
   return QSize(100, 200);
}

// ------------
// GLMemoryPane
// ------------
#if 0

GLMemoryPane::GLMemoryPane(MemoryView *parent, Gui *gui)
: QGLWidget(parent), m_parent(parent), m_gui(gui)
{
   object = 0;
   xRot = 0;
   yRot = 0;
   zRot = 0;

   setAttribute(Qt::WA_NoSystemBackground);
}

GLMemoryPane::~GLMemoryPane() {
   // TODO:  clean up OpenGL resources

   makeCurrent();
   glDeleteLists(object, 1);
   deleteTexture(m_defaultTexture);
   //safeDelete();
}

QSize GLMemoryPane::minimumSizeHint() const {
   return QSize(50, 50);
}

QSize GLMemoryPane::sizeHint() const {
   return QSize(300, 300);
}

void GLMemoryPane::setXRotation(int angle) {
   normalizeAngle(&angle);

   if (angle != xRot) {
      xRot = angle;
      emit xRotationChanged(angle);
      updateGL();
   }
}

void GLMemoryPane::setYRotation(int angle) {
   normalizeAngle(&angle);

   if (angle != yRot) {
      yRot = angle;
      emit yRotationChanged(angle);
      updateGL();
   }
}

void GLMemoryPane::setZRotation(int angle) {
   normalizeAngle(&angle);

   if (angle != zRot) {
      zRot = angle;
      emit zRotationChanged(angle);
      updateGL();
   }
}

void GLMemoryPane::initializeGL() {
   qglClearColor(Qt::black);
   object = createSphere();
   
   m_defaultTexture = bindTexture(QPixmap(QString(":/test.png")), GL_TEXTURE_2D);
   m_currentTexture = m_defaultTexture;

   glShadeModel(GL_FLAT);
#ifndef Q_OS_WIN32
   glEnable(GL_MULTISAMPLE);
#endif
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);
}

void GLMemoryPane::reset() {
   if (m_currentTexture != m_defaultTexture) {
      m_currentTexture = m_defaultTexture;
      updateGL();
   }
}

void GLMemoryPane::render(QImage *image) {
   if (image->isNull()) return;   
   
   bool hasOld = (m_currentTexture != m_defaultTexture);
   GLuint old = m_currentTexture;
   m_currentTexture = bindTexture(QPixmap::fromImage(*image), GL_TEXTURE_2D);

   if (hasOld)
      deleteTexture(old);
   
   updateGL();
}

void GLMemoryPane::paintGL() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslated(0.0, 0.0, -10.0);
   glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
   glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
   glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
   
   glBindTexture(GL_TEXTURE_2D, m_currentTexture);
   glCallList(object);
}

void GLMemoryPane::resizeGL(int width, int height) {
   int side = qMin(width, height);
   glViewport((width - side) / 2, (height - side) / 2, side, side);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
   glMatrixMode(GL_MODELVIEW);
}

void GLMemoryPane::mousePressEvent(QMouseEvent *event) {
   lastPos = event->pos();
}

void GLMemoryPane::mouseMoveEvent(QMouseEvent *event) {
   int dx = event->x() - lastPos.x();
   int dy = event->y() - lastPos.y();
   
   if (event->buttons() & Qt::LeftButton) {
      setXRotation(xRot + 8 * dy);
      setYRotation(yRot + 8 * dx);
   } else if (event->buttons() & Qt::RightButton) {
      setXRotation(xRot + 8 * dy);
      setZRotation(zRot + 8 * dx);
   }

   lastPos = event->pos();
}

GLuint GLMemoryPane::createSphere() {
   GLuint list = glGenLists(1);
   glNewList(list, GL_COMPILE);
   

   
   //glBindTexture(GL_TEXTURE_2D, m_defaultTexture);
   //qglColor(Qt::red);
   Sphere sphere(50, 50);
   sphere.draw();


   
   glEndList();
   return list;
}

void GLMemoryPane::quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
      GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4) {
   qglColor(Qt::red);

   glVertex3d(x1, y1, -0.05);
   glVertex3d(x2, y2, -0.05);
   glVertex3d(x3, y3, -0.05);
   glVertex3d(x4, y4, -0.05);

   glVertex3d(x4, y4, +0.05);
   glVertex3d(x3, y3, +0.05);
   glVertex3d(x2, y2, +0.05);
   glVertex3d(x1, y1, +0.05);
}

void GLMemoryPane::extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
   qglColor(Qt::green);

   glVertex3d(x1, y1, +0.05);
   glVertex3d(x2, y2, +0.05);
   glVertex3d(x2, y2, -0.05);
   glVertex3d(x1, y1, -0.05);
}

void GLMemoryPane::normalizeAngle(int *angle) {
   while (*angle < 0)
      *angle += 360 * 16;
   while (*angle > 360 * 16)
      *angle -= 360 * 16;
}

#endif


void MemoryView::populateScene(Program *program) {
   MemoryUseMap *memoryUseMap = program->getMemoryUseMap();
//   unsigned int noAddresses   = memoryUseMap->size();
   unsigned int dataSize  = program->getDataSize();
   unsigned int noAddresses = dataSize >> 2;
   unsigned int maxData = DATA_BASE_ADDRESS + dataSize;
   
   //TIMESTAMP current = program->getCurrentTimestamp();
   unsigned int earliest = 0;//current - 100; // earliest timestamp to consider
   unsigned int largestList = 1;

   QHash<unsigned int, unsigned int> addressesToAccesses;
   unsigned int lastValidAddr = DATA_BASE_ADDRESS;
   unsigned int averageAccesses = 0;
   
   for(unsigned int address = DATA_BASE_ADDRESS; address < maxData; address += 4) {
//   foreach(unsigned int address, memoryUseMap->keys()) {
      //assert(memoryUseMap->contains(address));
      if (!memoryUseMap->contains(address)) {
         addressesToAccesses.insert(address, 0);
         continue;
      }
      QList<TIMESTAMP> *list = memoryUseMap->value(address);
      
      if (list == NULL || address > maxData)
         continue;
      unsigned int accesseses = list->size();
      foreach(TIMESTAMP stamp, *list) {
        if (stamp < earliest)
           accesseses--;
        else break;
      }
      
      if (accesseses > 0) {
         addressesToAccesses.insert(address, accesseses);

         if (accesseses > largestList)
            largestList = accesseses;
         if (address > lastValidAddr)
            lastValidAddr = address;

         ++averageAccesses;
      }
   }
   
   float outlierFactor = 1;
   // try to throw out outliers
   if (addressesToAccesses.size() > 0) {
      float average = ((float)averageAccesses) / ((float)addressesToAccesses.size());

      if (average * 100 < largestList && largestList > 0 && averageAccesses > 0) {
//         cerr << "outliers: " << average << ", " << averageAccesses << " vs " << largestList << endl;

         largestList  /= 2;
         outlierFactor = 2.5f;
      }
   }
   float mostAccesses  = largestList;

//   cerr << "noA: " << noAddresses << ", mostAccessed: " << mostAccesses << endl;
   
//   noAddresses = (lastValidAddr - DATA_BASE_ADDRESS) >> 2;
   float *values = (float*)malloc(sizeof(float) * noAddresses);
   memset(values, 0, sizeof(float) * noAddresses);
   int *occurrences = (int*)malloc(sizeof(int) * noAddresses);
   memset(occurrences, 0, sizeof(int) * noAddresses);
    
   for(unsigned int address = DATA_BASE_ADDRESS; address < maxData; address += 4) {
      if (!addressesToAccesses.contains(address))
         continue;
   //foreach(unsigned int address, addressesToAccesses.keys()) {
      unsigned int size = addressesToAccesses[address];

      float val = ((float)size) / mostAccesses;
      unsigned int index;
      
      if (size < largestList && outlierFactor > 1)
         val *= outlierFactor;
      
      // data section
      index = (unsigned int)((address - DATA_BASE_ADDRESS) >> 2);
      
      if (index >= noAddresses)
         index = noAddresses - 1;
      
      occurrences[index]++;
      values[index] = val;
   }

   for(unsigned int i = 0; i < noAddresses; i++) {
      if (occurrences[i] != 0)
         values[i] /= occurrences[i];
   } // take average
   
   createChips(values, noAddresses, program);
   free(occurrences);
}

// create a nice gradient-effect for the default
void MemoryView::populateDefault() {
   unsigned int height = 25;
   unsigned int noAddresses = height << 3;
   float *values = (float*)malloc(sizeof(float) * noAddresses);
   memset(values, 0, sizeof(float) * noAddresses);
   float max = (noAddresses >> 1) + ((noAddresses & 7) << 2);
   
   for(unsigned int i = 0; i < noAddresses; i++)
      values[i] = ((i >> 1) + ((i & 7) << 2)) / max;

   createChips(values, noAddresses, NULL);
}

void MemoryView::createChips(float *values, unsigned int noAddresses, Program *program) {
   State *state = (program == NULL ? NULL : program->getState());
   QGraphicsScene *oldScene = m_scene;
   m_scene = new Scene(m_view);//QGraphicsScene();
   
   const QImage activityGradient(IMAGES"/activityGradient.png");
   int maxY = 500;//(noAddresses >> 3) * 60;
   //int maxY = (noAddresses >> 1) * 5;
   const unsigned int factor = (state == NULL ? 255 : 300);
//   cerr << "noAddresses: " << noAddresses << ", maxY = " << maxY << endl;

   // ============
   // Create Chips
   // ============
   for(unsigned int addr = 0; addr < noAddresses; addr++) {
      int x = -784  + (addr & 7) * (1568 / 16);
      int y = -maxY + (addr >> 3) * (maxY / 8);
      
      unsigned int address = DATA_BASE_ADDRESS + (addr << 2);
      unsigned int value = (state == NULL ? 0 : state->getMemoryWordUnobserved(address));
      unsigned char scale = 255;
      
      if (factor * values[addr] < 255)
         scale = (unsigned char)(factor * values[addr]);
      
      QColor color = QColor(activityGradient.pixel(scale, 0));
      
      QString label;
      if (program != NULL)
         program->getLabelForAddress(address, label);
      
      QGraphicsItem *item = new Chip(m_view, QColor(color), x, y, address, value, label, hasWatchpoint(address), NULL);
      item->setPos(QPointF(x, y));
      m_scene->addItem(item);
   }
   
   m_view->setScene(m_scene);
   if (oldScene != NULL)
      delete oldScene;
   
   free(values);
}

void MemoryView::toggleWatchpoint(Chip *chip) {
   unsigned int addr = chip->getAddress();
   bool enabled = !hasWatchpoint(addr);

   //cerr << "setting wpnt: " << addr << " to " << enabled << endl;

   m_watchpoints[addr] = enabled;
   chip->setWatchpointEnabled(enabled);
}

bool MemoryView::hasWatchpoint(Chip *chip) const {
   unsigned int addr = chip->getAddress();

   return hasWatchpoint(addr);
}

bool MemoryView::hasWatchpoint(unsigned int addr) const {
   if (!m_watchpoints.contains(addr))
      return false;

   return m_watchpoints[addr];
}

//    QImage image(":/test.png");

    // Populate m_scene
/*    int xx = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i += 110) {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += 70) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            QGraphicsItem *item = new Chip(color, xx, yy, 0, i, NULL);
            item->setPos(QPointF(i, j));
            m_scene->addItem(item);

            ++nitems;
        }
    }*/

/*
   GLdouble x1 = +0.06;
   GLdouble y1 = -0.14;
   GLdouble x2 = +0.14;
   GLdouble y2 = -0.06;
   GLdouble x3 = +0.08;
   GLdouble y3 = +0.00;
   GLdouble x4 = +0.30;
   GLdouble y4 = +0.22;

   quad(x1, y1, x2, y2, y2, x2, y1, x1);
   quad(x3, y3, x4, y4, y4, x4, y3, x3);

   extrude(x1, y1, x2, y2);
   extrude(x2, y2, y2, x2);
   extrude(y2, x2, y1, x1);
   extrude(y1, x1, x1, y1);
   extrude(x3, y3, x4, y4);
   extrude(x4, y4, y4, x4);
   extrude(y4, x4, y3, x3);

   const double Pi = 3.14159265358979323846;
   const int NumSectors = 200;

   for (int i = 0; i < NumSectors; ++i) {
      double angle1 = (i * 2 * Pi) / NumSectors;
      GLdouble x5 = 0.30 * sin(angle1);
      GLdouble y5 = 0.30 * cos(angle1);
      GLdouble x6 = 0.20 * sin(angle1);
      GLdouble y6 = 0.20 * cos(angle1);

      double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
      GLdouble x7 = 0.20 * sin(angle2);
      GLdouble y7 = 0.20 * cos(angle2);
      GLdouble x8 = 0.30 * sin(angle2);
      GLdouble y8 = 0.30 * cos(angle2);

      quad(x5, y5, x6, y6, x7, y7, x8, y8);

      extrude(x6, y6, x7, y7);
      extrude(x8, y8, x5, y5);
   }*/
      /*do {
         cur = list->front();
         if (cur < earliest) {
            list->removeFirst();
            if (list->isEmpty()) {
               //toRemove.push_back(address);
               break;
            }
         } else {
            unsigned int size = list->size();
            
            if (size > largestList)
               largestList = size;
            
            break;
         }
      } while (true);*/

 /*
   foreach(unsigned int address, memoryUseMap->keys()) {
      QList<TIMESTAMP> *list = memoryUseMap->value(address);
      
      if (list == NULL || list->isEmpty() || address > maxData)
         continue;

      unsigned int size = list->size();
      float val = size / mostAccesses;
      unsigned int index;
      
      if (address > maxData) { // probably in stack
#if 0
         index = noDataAddresses + (unsigned int)round(((float)(STACK_BASE_ADDRESS - address)) / (float)(noStackAddresses * 4));
         if (index >= noAddresses)
            index = noAddresses - 1;
#endif
         continue;
      } else { // data section
         index = (unsigned int)((address - DATA_BASE_ADDRESS) >> 4);// ) / (float)(noDataAddresses * 4);
         if (index > noAddresses)
            index = noAddresses;
            
      }
      
      cerr << "Address: " << address << "( " << (address > maxData) << " ) -> " << index << ", (maxData = " << maxData << ")" << endl;
      
      m_buffer[index] += val;
      m_norms[index] += 1;
      
      bool left  = ((index & 127)  > 0);
      bool right = ((index & 127)  < 127);
      float cornerContrib = 0.3f * val;
      float sideContrib   = 0.5f * val;
      
      if (left) {
         m_buffer[index - 1] += sideContrib;
         m_norms[index - 1] += 0.5f;
      }
      
      if (right) {
         m_buffer[index + 1] += sideContrib;
         m_norms[index + 1] += 0.5f;
      }
      
      if (index >= 128) {
         m_buffer[index - 128] += sideContrib;
         m_norms[index - 128] += 0.5f;
         
         if (left) {
            m_buffer[index - 129] += cornerContrib;
            m_norms[index - 129] += 0.3f;
         }
         
         if (right) {
            m_buffer[index - 127] += cornerContrib;
            m_norms[index - 127] += 0.3f;
         }
      }

      if (index < (128 * 127)) {
         m_buffer[index + 128] += sideContrib;
         m_norms[index + 128] += 0.5f;
         
         if (left) {
            m_buffer[index + 127] += cornerContrib;
            m_norms[index + 127] += 0.3f;
         }
         
         if (right) {
            m_buffer[index + 129] += cornerContrib;
            m_norms[index + 129] += 0.3f;
         }
      }
   }
   
   for(int i = 0; i < (128 * 128); i++) {
      unsigned char val = 0;
      
      if (m_norms[i] != 0)
         val = (unsigned char)((255 * m_buffer[i]) / m_buffer[i]);
      
      m_data[i] = val;
//      p.setPixel(i & 127, i / 128, val);
   }*/
  
//   unsigned int mostAccesses = removeEarlyAccesses(memoryUse, earliestTimeToConsider);
//   memset(p.bits(), 255, p.numBytes() * sizeof(unsigned char));

/*
// Returns largest list after removing early accesses
unsigned int MemoryUseMap::removeEarlyAccesses(MemoryUseMap *memoryUseMap, TIMESTAMP earliest) const {
      
   return largestList;
}*/



