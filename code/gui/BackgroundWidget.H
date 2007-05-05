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

