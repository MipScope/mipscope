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
   file: RegisterView.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#include "RegisterView.H"
#include "Utilities.H"
#include "EditorPane.H"
#include "Gui.H"
#include "../simulator/ParseNode.H"
//#include "BackgroundWidget.H"
#include <QtGui>

const char *const registerAliases[] = {
   "r0", "at",       // $r0-1
   "v0", "v1",       // $r2-3
   "a0", "a1", "a2", "a3", // $r4-r7
   "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", // $r8-15
   "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", // $r16-23
   "t8", "t9",       // $r24-25
   "k0", "k1", "gp", // $r26-28
   "sp",             // $r29
   "s8",             // $r30
   "ra",             // $r31
   
   // Special
   "PC",             // 32
   "HI",             // 33
   "LO",             // 34
};

QPixmap *WATCHPOINT = NULL;

// Pseudo-Output terminal, capable of undoing/redoing output
// ---------------------------------------------------------
RegisterView::RegisterView(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("Register View"), gui), m_gui(gui), m_editorPane(editorPane)
{
   setObjectName(tr("Register View"));
   
   // Default display types (unsigned base 10, and register aliases)
//   m_displayType = D_HEX;
//   m_registerDisplayType = D_ALIAS;
   
   WATCHPOINT = new QPixmap(IMAGES"/watchPoint.png");
   watchPoint = WATCHPOINT;

   // create a new qtabbedwidget holding View/Options, respectively
//   m_tabWidget = new QTabWidget(this);
   m_registerPane = new RegisterPane(this);
//   m_optionsPane  = new RegisterOptionsPane(this);

//   m_tabWidget->addTab(m_registerPane, "View");
//   m_tabWidget->addTab(m_optionsPane, "Options");
   
   setWidget(m_registerPane);//tabWidget);
}

bool *RegisterView::getWatchpoints() {
   return m_registerPane->getWatchpoints();
}

bool *RegisterPane::getWatchpoints() {
   return m_watchPoints;
}

class CustomBackgroundWidget : public QWidget {
   public:
      CustomBackgroundWidget(QString path, RegisterPane *pane, QWidget *parent = NULL) : QWidget(parent), m_registerPane(pane) {
         setMouseTracking(true);

         //cerr << path.toStdString() << endl;

         m_background = QImage(path);
         if (!m_background.isNull()) m_pixMap = QPixmap::fromImage(m_background);

         /*         QPalette p = palette();
                    p.setBrush(QPalette::Window, *m_pixMap);
                    setPalette(p);*/
         setAutoFillBackground(false);
         setAttribute(Qt::WA_NoSystemBackground);
      }

   protected:
      QPixmap m_pixMap;
      QImage m_background;
      RegisterPane *m_registerPane;

      // @overridden
      void paintEvent(QPaintEvent *e) {
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

      virtual void mousePressEvent(QMouseEvent *);
      virtual void mouseReleaseEvent(QMouseEvent *);
      virtual void mouseMoveEvent(QMouseEvent *);
};

RegisterPane::RegisterPane(RegisterView *regView) : QWidget(), 
   m_parent(regView), m_widget(new CustomBackgroundWidget(QString(IMAGES"/registerBackground.jpg"), this)) 
{
   QGridLayout *l = new QGridLayout();

   int max = 32;// + 3;
   for(int i = max; i--;) {
      int col = (i >> 3) * 3, row = (i & 7);
      
      m_registerLabels[i] = new IDLabel(this, i);
      l->addWidget(m_registerLabels[i], row, col);
      l->addWidget(m_registerLabels[i]->getValueLabel(), row, col + 1);
   }
   
   for(int i = NO_REGISTERS - max; i--;) {
      int row = 10, col = i * 3, index = 32 + i;
      
      m_registerLabels[index] = new IDLabel(this, index);
      l->addWidget(m_registerLabels[index], row, col);
      l->addWidget(m_registerLabels[index]->getValueLabel(), row, col + 1);
   }
   
   for(int i = 3; i--;)
      l->setColumnMinimumWidth(2 + 3 * i, 14);

   l->setRowMinimumHeight(9, 11);
/*   QColor orange(255, 213, 141);
   QSize size(8, 8);
   for(int i = 10; i--;)
      for(int j = 3; j--;)
         l->addWidget(new CustomSpacer(orange, size, this), i, 2 + 3 * j);*/

   for(int i = 12; i--;)
      l->setColumnStretch(i, 1);
   for(int i = 11; i--;)
      l->setRowStretch(i, 1);
   
   l->setSpacing(0);
   m_widget->setLayout(l);
   
   /*QPalette p = m_widget->palette();
   p.setBrush(QPalette::Window, QPixmap(QString(IMAGES"/registerBackground.jpg")));
   m_widget->setPalette(p);*/
   
   l = new QGridLayout();
   l->setMargin(0);
   l->addWidget(m_widget, 0, 0);//, Qt::AlignCenter);
   setLayout(l);

   // clear all watchpoints and hide the extended-tip view
   memset(m_watchPoints, 0, sizeof(bool) * register_count);
   
   m_extended = new ExtendedView(this, m_widget);
   m_extended->hide();
}

/*
QSize RegisterPane::minimumSize() const {
   return m_rect.size();
}

QSize RegisterPane::sizeHint() const {
   return m_rect.size();
}*/

// Clears all registers
void RegisterView::reset() {
   m_registerPane->reset();
}

void RegisterView::updateDisplay(bool registerAliases) {
   //m_displayType = (registerAliases ? D_ALIAS : D_REGISTER);
   Q_UNUSED(registerAliases);
   updateDisplay();
}

void RegisterView::updateBase(int base) {
   Q_UNUSED(base);
   updateDisplay();
}

// Clears all registers
void RegisterPane::reset() {
   for(int i = NO_REGISTERS; i--;)
      m_registerLabels[i]->reset();
   
   // clear all watchpoints and hide the extended-tip view
//   memset(m_watchPoints, 0, sizeof(bool) * register_count);
   m_extended->hide();
}

void CustomBackgroundWidget::mousePressEvent(QMouseEvent *e) { e->ignore(); }
void CustomBackgroundWidget::mouseReleaseEvent(QMouseEvent *e) {
   m_registerPane->testMousePress(e, e->pos());
}

void CustomBackgroundWidget::mouseMoveEvent(QMouseEvent *e) {
   m_registerPane->testMouseMove(e, e->pos());
}

void RegisterPane::leaveEvent(QEvent *e) {
   m_extended->hide();
   e->accept();
}

void RegisterPane::testMouseMove(QMouseEvent *e, const QPoint &pos) {
   unsigned int bestDist = 999999999;
   RegisterLabel *best = NULL;
   
   for(int i = NO_REGISTERS; i--;) {
      QRect r = m_registerLabels[i]->geometry();
      if (r.contains(pos)) {
         m_registerLabels[i]->testMouseMove(e, pos);
         break;
      }
      const QPoint &center = r.center();
      unsigned int dist = pos.x() - center.x();
      dist *= dist;
      unsigned int distY = pos.y() - center.y();distY *= distY;
      dist += distY;
      if (dist < bestDist) {
         bestDist = dist;
         best = m_registerLabels[i];
      }
      
      ValueLabel *v = m_registerLabels[i]->getValueLabel();

      r = v->geometry();
      if (r.contains(pos)) {
         v->testMouseMove(e, pos);
         break;
      }

      const QPoint &center2 = r.center();
      dist = pos.x() - center2.x();
      dist *= dist;
      distY = pos.y() - center2.y();distY *= distY;
      dist += distY;
      if (dist < bestDist) {
         bestDist = dist;
         best = v;
      }
   }

   if (best != NULL)
      best->testMouseMove(e, pos);
}

void RegisterPane::testMousePress(QMouseEvent *e, const QPoint &pos) {
   for(int i = NO_REGISTERS; i--;) {
      if (m_registerLabels[i]->geometry().contains(pos)) {
         m_registerLabels[i]->clicked(e, pos);
         break;
      }
      ValueLabel *v = m_registerLabels[i]->getValueLabel();

      if (v->geometry().contains(pos)) {
         v->clicked(e, pos);
         break;
      }
   }
}

// IDLabels notify RegisterPane which notifies the active prog
// which notifies the active Debugger
void RegisterPane::watchPointModified(unsigned int reg, bool watchPoint) {
   m_watchPoints[reg] = watchPoint;

   m_parent->m_gui->watchPointModified(reg, watchPoint);
}

void RegisterLabel::testMouseMove(QMouseEvent *e, const QPoint &parentPos) {
//   m_lastPos = parentPos;

   //m_time->restart();

   e->accept();

   showExtended(parentPos);//, true);
   //checkForExtended();
//   QTimer::singleShot(REGISTER_HOVER_DELAY + 1, this, SLOT(checkForExtended()));
}

void RegisterLabel::clicked(QMouseEvent *e, const QPoint &parentPos) {
   handleClick(e);
}

//void RegisterLabel::mousePressed(QMouseEvent *e, const QPoint &parentPos) {
//   handleClick(e);
//}

// Updates display of modified register
void RegisterView::registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc) {
//   assert(reg < register_count);
   
   m_registerPane->m_registerLabels[reg]->setValue(value, pc);
   if (status == PAUSED)
      m_registerPane->m_registerLabels[reg]->updateDisplay();
}

void RegisterView::pcChanged(ParseNode *curPC, bool justRolledBack) {
   unsigned int addr = 0;

   if (curPC != NULL)
      addr = curPC->getAddress();

   registerChanged(pc, addr, PAUSED, curPC);
}

void RegisterView::updateDisplay(int state) {
   if (state == STOPPED)
      reset();
}

void RegisterView::updateDisplay() {
   for(int i = 0; i < register_count; i++)
      m_registerPane->m_registerLabels[i]->updateDisplay();
}

void RegisterPane::showExtended(const QPoint &pos, const QString &text, RegisterLabel *v) {
   m_extended->show(text, "", pos, v);
}

void RegisterPane::displayTypeChanged() {
   for(int i = NO_REGISTERS; i--;)
      m_registerLabels[i]->updateDisplay();
}

RegisterLabel::RegisterLabel(RegisterPane *regPane) 
   : QLabel(), m_parent(regPane)
{
   setMouseTracking(true);
   
   setFont(QFont("Courier", 11));
   
   /*QPalette pal = palette();
   QColor orange(255, 213, 141);
   
   pal.setColor(QPalette::Window, orange);
   pal.setColor(QPalette::Foreground, Qt::black);
   setPalette(pal);
   setAutoFillBackground(true);*/
   
   setWordWrap(false);
//   setFrameShape(QFrame::Panel);
//   setFocusPolicy(Qt::NoFocus);
   QWidget::setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

//void RegisterLabel::mousePressEvent(QMouseEvent *e) { e->ignore(); }
/*void RegisterLabel::mouseReleaseEvent(QMouseEvent *e) {
//   if (rect().contains(e->pos())) {
      handleClick(e);
      e->accept();
//   } else e->ignore();
}*/

/*void RegisterLabel::checkForExtended() {
//   int ms = m_time->elapsed();
//   cerr << m_isInside <<  " ms " << ms << endl;
   
   //if (!m_isInside) {
      //if (!m_parent->m_extended->isVisible())// || m_parent->m_extended->m_orig == this)
         //return;
   //}
   
   //if (ms >= REGISTER_HOVER_DELAY) {
      showExtended(m_lastPos, true);
      
      m_time->restart();
   //}
}*/

void RegisterLabel::mouseMoveEvent(QMouseEvent *e) {
   testMouseMove(e, mapToParent(e->pos()));
}

/*void RegisterLabel::enterEvent(QEvent *e) {
   m_time->restart();
   m_isInside = true;
   e->accept();
}

void RegisterLabel::leaveEvent(QEvent *e) {
   m_isInside = false;
   
   m_time->restart();
   e->accept();
}*/

IDLabel::IDLabel(RegisterPane *regPane, unsigned int id) 
   : RegisterLabel(regPane), m_register(id), m_value(0), 
   m_valueLabel(new ValueLabel(regPane, this)), 
   m_watchPoint(false), m_lastModified(NULL)
{
   //QFont f = font();
   QPalette pal = palette();
   // reddish
   QColor c(140, 72, 22);//(0, 79, 175);
   
//   pal.setColor(QPalette::Window, orange);
   pal.setColor(QPalette::Foreground, c);
   setPalette(pal);
//   setAutoFillBackground(true); 
   
   updateDisplay();
}

void IDLabel::reset() {
//   m_watchPoint   = false;
   m_lastModified = NULL;
   setValue(0);
}

void IDLabel::setValue(unsigned int value, ParseNode *pc) {
   m_value = value;
   m_lastModified = pc;
   //updateDisplay();
}

void IDLabel::updateDisplay() {
   QString text = getRegisterText(m_register, Options::registerAliases());
   if (m_register == 9 && !Options::registerAliases())//m_registerDisplayType != D_ALIAS)
      text += QString(": ");
   else text += QString(":");

   if (m_watchPoint)
      setPixmap(merge(watchPoint, text));
   else setText(text);
   
   m_valueLabel->setValue(m_value);
}

QString getRegisterText(int reg, bool displayType) {
   if (displayType || reg >= 32)
      return QString(registerAliases[reg]);
   
   return QString("r%1").arg(reg);
}

QPixmap IDLabel::merge(const QPixmap *pix, const QString &txt)
{
   QPainter p;
   QPixmap temp(1, 1);
   p.begin(&temp);
   p.setFont(font());
 
   int strWidth = p.fontMetrics().width( txt );
   int strHeight = p.fontMetrics().height();

   p.end();

   int pixWidth = pix->width(), pixExtra = (pixWidth << 1) / 3;
   int pixHeight = pix->height();
   int maxHeight = (strHeight > pixHeight ? strHeight + 2 : pixHeight);
   
   QPixmap res(strWidth + pixExtra, maxHeight);
//   res.fill(Qt::white);
   res.fill(QColor(253, 230, 153, 255));
   
   p.begin(&res);
   p.setFont(font());
   p.drawText(QRect( 0, 2, strWidth, strHeight), 0, txt);
   p.drawPixmap(strWidth - (pixWidth - pixExtra), 0, *pix);
   p.end();
   
   return res;
}

QPixmap *watchPoint = NULL;
void IDLabel::handleClick(QMouseEvent *e) {
   m_watchPoint = !m_watchPoint;
   
   // notify the chain which notifies the debugger of a change in watchpoint status
   m_parent->watchPointModified(m_register, m_watchPoint);

   e->accept();
   updateDisplay();

   // TODO: actually set watchpoint in debugger
}

unsigned int IDLabel::getValue() const {
   return m_value;
}

QString RegisterView::getRegisterText(int registerNo) const {
   return m_registerPane->getRegisterText(registerNo);
}

QString RegisterPane::getRegisterText(int registerNo) {
   unsigned int value = m_registerLabels[registerNo]->getValue();
   
   QString alias = ::getRegisterText(registerNo, true);
   QString regName = ((registerNo >= 32) ? 
      QString("<b>%1</b>").arg(alias) : 
      QString("<b>r%1</b> (%2)").arg(QString::number(registerNo), alias));
   
   QString mText = QString(
      "<span style=\"font-size: 12;\">%1</span>"
      "<pre>").arg(regName);

   if (value == 0)
      mText += QString("Value = 0");
   else {
      QString one = getNoInBase(value, 16), 
//              two = getNoInBase(value, 2), 
              three = getNoInBase((signed)value, -10),//D_SIGNED_DECIMAL), 
              four = getNoInBase(value, 10);
      
      mText += QString(
         "Hex:      %1<br>"
//         "Binary:   %2<br>"
         "Decimal:  %2")
         .arg(one, /*two, */three);
      if (three != four)
         mText += QString("<br>UDecimal: %1").arg(four);
   }
   
   // Display the line which is responsible for setting 
   // the value currently in this register.
   ParseNode *lastModified = m_registerLabels[registerNo]->m_lastModified;
   
   TextEditor *active = NULL;
   if (lastModified != NULL && lastModified->isValid() && lastModified->getTextBlock() != NULL && (active = m_parent->m_gui->getActiveProgram()) != NULL) {
      mText += QString("<br>"
                       "(Set by line <i>%1</i>)").arg(1 + active->lineNumber(*lastModified->getTextBlock()));
   }
   
   mText += QString("</pre>");

   return mText;
}

void IDLabel::showExtended(const QPoint &p) {//, bool alreadyAdjusted, RegisterLabel *v) {
/*   QPoint p2 = p;
   if (!alreadyAdjusted)
      p2 = mapToParent(p);

   if (v == NULL)
      v = this;*/

/*   if (m_parent->m_extended->m_orig == this || m_parent->m_extended->m_orig == v) {
      m_parent->m_extended->move(p2);
      return;
   }*/

   QString alias = getRegisterText(m_register, true);
   QString regName = ((m_register >= 32) ? 
      QString("<b>%1</b>").arg(alias) : 
      QString("<b>r%1</b> (%2)").arg(QString::number(m_register), alias));
   
   QString mText = QString(
      "<span style=\"font-size: 12;\">%1</span>"
      "<pre>").arg(regName);

   if (m_value == 0)
      mText += QString("Value = 0");
   else {
      QString one = getNoInBase(m_value, 16), 
//              two = getNoInBase(m_value, 2), 
              three = getNoInBase(m_value, -10), 
              four = getNoInBase(m_value, 10);
      
      mText += QString(
         "Hex:      %1<br>"
//         "Binary:   %2<br>"
         "Decimal:  %2")
         .arg(one, /*two, */three);
      if (three != four)
         mText += QString("<br>UDecimal: %1").arg(four);
   }
   
   // Display the line which is responsible for setting 
   // the value currently in this register.
   TextEditor *active = NULL;
   if (m_lastModified != NULL && m_lastModified->isValid() && m_lastModified->getTextBlock() != NULL && (active = m_parent->m_parent->m_gui->getActiveProgram()) != NULL) {
      mText += QString("<br>"
                       "(Set by line <i>%1</i>)").arg(1 + active->lineNumber(*m_lastModified->getTextBlock()));
   }
   
   mText += QString("</pre>");
   /*
      QString("");
      ("Last modifed by<br>line: <b>%1</b>").arg(QString("N/A"));*/
    //"<ul style=\"list-style: none;\">"
         /*"<li "STYLE_LEFT">Hex</li><li "STYLE_RIGHT">%2</li>"
         "<li "STYLE_LEFT">Binary</li><li "STYLE_RIGHT">%3</li>"
         "<li "STYLE_LEFT">Decimal</li><li "STYLE_RIGHT">%4</li>"
         "<li "STYLE_LEFT">Unsigned</li><li "STYLE_RIGHT">%5</li>"*/
      //"</ul>"
   
   m_parent->showExtended(p, mText, this);
}

ValueLabel *IDLabel::getValueLabel() {
   return m_valueLabel;
}

//ValueDisplay m_displayType;
ValueLabel::ValueLabel(RegisterPane *regPane, IDLabel *idLabel) 
   : RegisterLabel(regPane), m_idLabel(idLabel)
{
   setValue(0);
}

void ValueLabel::handleClick(QMouseEvent *e) {
   m_idLabel->handleClick(e);
}

void ValueLabel::showExtended(const QPoint &p) {
   m_idLabel->showExtended(p);//, true || alreadyAdjusted, this);
}

void ValueLabel::setValue(unsigned int newValue) {
   setText(getNoInBase(newValue, Options::dislayBase()));//m_displayType));
}

QString getNoInBase(unsigned int no, int base) {
//   const int bases[] = { 16, /*2, */10, 10 };
//   int base2 = bases[base];

   if (base < 0)
      return QString::number((signed int)no, -base);
   
   return QString::number(no, base);//2);
}


/*class CustomSpacer : public QWidget {
   
   public:
      CustomSpacer(const QColor &c, const QSize &size, QWidget *parent = NULL) : QWidget(parent), m_size(size)
      {
         QPalette pal = palette();

         pal.setColor(QPalette::Window, c);
         setPalette(pal);
         
         setAutoFillBackground(true);
         setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      }

      // @override
      virtual QSize sizeHint() const { return m_size; }
      virtual QSize minimumSize() const { return m_size; }
      virtual QSize maximumSize() const { return m_size; }
      
   protected:
      QSize m_size;
};*/

ExtendedView::ExtendedView(RegisterPane *regPane, QWidget *parent) 
   : QLabel(regPane), m_registerPane(regPane), m_parent(parent), 
   m_timer(new QTimer(this)), m_state(S_Hidden)
{
/*   QPalette pal = palette();
   QColor c(255, 255, 215); // light yellow
   //c.setAlpha(120);
   
   pal.setColor(QPalette::Window, c);
   pal.setColor(QPalette::Foreground, Qt::black);
   setPalette(pal);*/
   
   setWordWrap(false);
   setAutoFillBackground(false);
   setBackgroundRole(QPalette::Window);
   
   // timer to automatically repaint for fading effects
   connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
   
//   cerr << testAttribute(Qt::WA_NoMousePropagation) << endl;
//   cerr << testAttribute(Qt::WA_NoSystemBackground) << endl;
   //setMargin(-1);

   setMouseTracking(false);
   //setFrameShadow(QFrame::Raised);
   //setFrameShape(QFrame::StyledPanel);
   setFocusPolicy(Qt::NoFocus);
   QLabel::hide();
   
   QFont font("Courier New", 11);
   setFont(font);
}

void ExtendedView::mousePressEvent(QMouseEvent *e) {
//   this->hide();
   
   e->ignore();
   m_registerPane->testMousePress(e, mapTo(m_registerPane, e->pos()));
}

void ExtendedView::hide() {
   if (!isVisible())
      return;
   
   if (m_state == S_Fading || m_state == S_Hidden)
      return;
   
   m_state = S_Fading;
   m_alpha = 255;
   m_timer->start(EXTENDED_FADE_INTERVAL);

   if (STATUS_BAR != NULL)
      STATUS_BAR->clearMessage();
   
   m_orig = NULL;
   //QLabel::hide();
}

void ExtendedView::show(const QString &text, const QString &statusText, 
      const QPoint &pos, RegisterLabel *orig)
{
   m_orig = orig;
   setText(text);
   STATUS_BAR->showMessage(statusText);
   
   const QSize &hint = sizeHint();
   QRect r(pos.x() + 6, pos.y() + 4, hint.width(), hint.height());
   QRect container = m_parent->rect();
   
   // force label to stay within bounds of container
   if (r.right() > container.right()) 
      r.translate(container.right() - r.right(), 0);
   
   if (r.bottom() > container.bottom()) 
      r.translate(0, container.bottom() - r.bottom());
   
   setGeometry(r);
   if (!isVisible()) {
      m_state = S_Appearing;
      m_alpha = 0;
      QLabel::show();
      m_timer->start(EXTENDED_FADE_INTERVAL);
   } else if (m_state != S_Appearing) {
      m_state = S_Normal;
      update();
   }
//   if (isVisible())
//      update();
   //else QLabel::show();
   
   //connect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));
}

void ExtendedView::mouseMoveEvent(QMouseEvent *e) {
   e->ignore();
   m_registerPane->testMouseMove(e, mapTo(m_registerPane, e->pos()));
}

// @overridden
void ExtendedView::paintEvent(QPaintEvent *e) {
   QPainter p(this);
   QRect r = rect();
   
   int alpha = 255;
   int fading = -(m_state == S_Fading) + (m_state == S_Appearing);
   if (fading) {
      // 255 * interval / duration
      m_alpha += fading * (255 * EXTENDED_FADE_INTERVAL) / EXTENDED_FADE_DURATION;
      // cap interval
      alpha = (m_alpha < 0 ? 0 : (m_alpha > 255 ? 255 : m_alpha));
   }

   QColor c(255, 255, 225, alpha); // light yellow
   QColor c2(255, 255, 150, alpha); // light yellow
   QColor black = Qt::black;
   black.setAlpha(alpha);
   
   QLinearGradient grad(0, 0, r.width(), r.height());
   grad.setColorAt(0.0, c);
   grad.setColorAt(1, c2);
   
   p.setPen(QPen(QBrush(black), 2));
   
   p.fillRect(r, grad);
   p.drawRect(r);//, 10, 10);
   p.end();
   
   if (fading < 0 && alpha <= 0) { // S_Fading
      m_timer->stop();
      m_state = S_Hidden;
      QLabel::hide();
      return;
   } else if (fading > 0 && alpha >= 255) { // S_Appearing
      m_timer->stop();
      m_state = S_Normal;
   }
   
   QLabel::paintEvent(e);
}

//IDDisplay m_registerDisplayType;
/*RegisterOptionsPane::RegisterOptionsPane(RegisterView *parent) 
   : QWidget(), m_parent(parent)
{
   QWidget *p = new QWidget();
   QGridLayout *l = new QGridLayout();
   QLabel *label;
   QFont f(font());
   f.setPointSizeF(11.2f);
   setFont(f);

   l->addWidget((label = new QLabel("Register Display:")), 0, 0, Qt::AlignLeft);
   label->setToolTip("Change the way register names are displayed.");
   QComboBox *b = new QComboBox();
   b->addItem(QString("Numbers ($r0, $r1..)"));
   b->addItem(QString("Aliases ($v0, $t0..)"));
   b->setCurrentIndex(m_registerDisplayType);
   connect(b, SIGNAL(currentIndexChanged(int)), this, SLOT(registerDisplayTypeChanged(int)));
   
   l->addWidget(b, 1, 0, Qt::AlignRight);
   
   l->addWidget((label = new QLabel("Default Base:")), 2, 0, Qt::AlignLeft);
   label->setToolTip("Sets the base in which register values will be displayed.");
   b = new QComboBox();
   b->addItem(QString("Hex"));
//   b->addItem(QString("Binary"));
   b->addItem(QString("Decimal (signed)"));
   b->addItem(QString("Decimal (unsigned)"));
   b->setCurrentIndex(m_displayType);
   connect(b, SIGNAL(currentIndexChanged(int)), this, SLOT(defaultBaseChanged(int)));
   
   l->addWidget(b, 3, 0, Qt::AlignRight);
   for(int i = 4; i--;)
      l->setRowStretch(i, 1);
   
   p->setLayout(l);
//   p->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   
   l = new QGridLayout();
   l->addWidget(p, 0, 0, Qt::AlignTop);
   l->setMargin(0);
   setLayout(l);
}

void RegisterOptionsPane::registerDisplayTypeChanged(int val) {
   m_registerDisplayType = (IDDisplay)val;

   m_parent->m_registerPane->displayTypeChanged();
}

void RegisterOptionsPane::defaultBaseChanged(int val) {
   m_displayType = (ValueDisplay)val;

   m_parent->m_registerPane->displayTypeChanged();
}*/

