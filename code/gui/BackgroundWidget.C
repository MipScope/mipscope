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
   m_pixMap = QPixmap::fromImage(m_background);
   setAutoFillBackground(false);
   setAttribute(Qt::WA_NoSystemBackground);
}

//BackgroundWidget::~BackgroundWidget() { }

// @overridden
void BackgroundWidget::paintEvent(QPaintEvent *e) {
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

