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
#include <QtGui>

// Pseudo-Output terminal, capable of undoing/redoing output
// ---------------------------------------------------------
RegisterView::RegisterView(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("Register View"), gui), m_gui(gui), m_editorPane(editorPane)
{
   setObjectName(tr("Register View"));
   
   // create a new qtabbedwidget holding View/Options, respectively
   m_tabWidget = new QTabWidget(this);
   m_registerPane = new RegisterPane(this);
   m_optionsPane  = new RegisterOptionsPane(this);
   
   m_displayType = D_UNSIGNED_DECIMAL;
   m_registerDisplayType = D_ALIAS;

   m_tabWidget->addTab(m_registerPane, "View");
   m_tabWidget->addTab(m_optionsPane, "Options");
   
   setWidget(m_tabWidget);
}

RegisterPane::RegisterPane(RegisterView *regView) : QWidget(), 
   m_parent(regView), m_extended(new ExtendedView(this))
{
   QWidget *m_widget = new QWidget(this);
   QGridLayout *l = new QGridLayout();

   int max = 32 + 4;
   for(int i = max; i--;) {
      m_registerLabels[i] = new IDLabel(this, i);
      int col = (i / 9) * 3, row = (i % 9);
      
      l->addWidget(m_registerLabels[i], row, col);
      l->addWidget(m_registerLabels[i]->getValueLabel(), row, col + 1);
   }
   for(int i = 3; i--;)
      l->setColumnMinimumWidth(2 + 3 * i, 14);

/*   QColor orange(255, 213, 141);
   QSize size(8, 8);
   for(int i = 10; i--;)
      for(int j = 3; j--;)
         l->addWidget(new CustomSpacer(orange, size, this), i, 2 + 3 * j);*/

   for(int i = 12; i--;)
      l->setColumnStretch(i, 1);
   for(int i = 10; i--;)
      l->setRowStretch(i, 1);
   
   l->setSpacing(0);
   l->setMargin(0);
   m_widget->setLayout(l);
   
   l = new QGridLayout();
   l->addWidget(m_widget, 0, 0);//, Qt::AlignCenter);
   setLayout(l);

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

// Clears all registers
void RegisterPane::reset() {
   for(int i = 32 + 4; i--;)
      m_registerLabels[i]->reset();
}

void RegisterPane::leaveEvent(QEvent *e) {
   m_extended->hide();
   e->accept();
}

void RegisterPane::testMouseMove(QMouseEvent *e, const QPoint &pos) {
   for(int i = 32 + 4; i--;) {
      if (m_registerLabels[i]->geometry().contains(pos)) {
         m_registerLabels[i]->testMouseMove(e, pos);
         break;
      }
   }
}

void RegisterLabel::testMouseMove(QMouseEvent *e, const QPoint &parentPos) {
   m_lastPos = parentPos;

   m_time->restart();

   e->accept();
   QTimer::singleShot(REGISTER_HOVER_DELAY + 5, this, SLOT(checkForExtended()));
}

// Updates display of modified register
void RegisterView::registerChanged(int reg, int value) {
   assert(reg < 32 + 4);

   m_registerPane->m_registerLabels[reg]->setValue(value);
}

void RegisterPane::showExtended(const QPoint &pos, const QString &text) {
   m_extended->show(text, "", pos);
}

void RegisterPane::displayTypeChanged() {
   for(int i = 32 + 4; i--;)
      m_registerLabels[i]->updateDisplay();
}

RegisterLabel::RegisterLabel(RegisterPane *regPane) 
   : QLabel(), m_parent(regPane), m_isInside(false), 
   m_time(new QTime())
{
   setMouseTracking(true);
   
   // TODO: setup formatting/font
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

void RegisterLabel::mousePressEvent(QMouseEvent *e) { e->ignore(); }
void RegisterLabel::mouseReleaseEvent(QMouseEvent *e) {
   if (rect().contains(e->pos())) {
      handleClick(e);
      e->accept();
   } else e->ignore();
}

void RegisterLabel::checkForExtended() {
   int ms = m_time->elapsed();
//   cerr << m_isInside <<  " ms " << ms << endl;
   
   if (!m_isInside) {
      if (!m_parent->m_extended->isVisible())
         return;
   }
   
   if (ms >= REGISTER_HOVER_DELAY) {
      showExtended(m_lastPos, true);
      
      m_time->restart();
   }
}

void RegisterLabel::mouseMoveEvent(QMouseEvent *e) {
   testMouseMove(e, mapTo(m_parent, e->pos()));
}

void RegisterLabel::enterEvent(QEvent *e) {
   m_time->restart();
   m_isInside = true;
   e->accept();
}

void RegisterLabel::leaveEvent(QEvent *e) {
   m_isInside = false;
   
   m_time->restart();
   e->accept();
}

IDLabel::IDLabel(RegisterPane *regPane, unsigned int id) 
   : RegisterLabel(regPane), m_register(id), m_value(0), 
   m_valueLabel(new ValueLabel(regPane, this)), 
   m_watchPoint(false)
{
   //QFont f = font();
   QPalette pal = palette();
   QColor c(0, 79, 175);
   
//   pal.setColor(QPalette::Window, orange);
   pal.setColor(QPalette::Foreground, c);
   setPalette(pal);
//   setAutoFillBackground(true); 
   
   updateDisplay();
}

void IDLabel::reset() {
   setValue(0);
}

void IDLabel::setValue(int value) {
   m_value = value;
   
   updateDisplay();
}

void IDLabel::updateDisplay() {
   // TODO: if m_registerDisplayType is D_ALIAS, lookup alias in table
   
   if (m_register == 9)
      setText(QString("r%1: ").arg(m_register));
   else setText(QString("r%1:").arg(m_register));
   
   m_valueLabel->setValue(m_value);
}

void IDLabel::handleClick(QMouseEvent *e) {
   m_watchPoint = !m_watchPoint;
   
   e->accept();
   // TODO:  actually update icon oder etwas
   
   


}

void IDLabel::showExtended(const QPoint &p, bool alreadyAdjusted) {
//#define STYLE_LEFT   "style=\"clear: both;float: left; width: 40%;\""
//#define STYLE_RIGHT  "style=\"float: left;width: 60%;padding-left: 2px;" 
   //
   //"border: 1px dotted #99BB57;\""
   QString mText = QString(
      "<span style=\"font-size: 12;\"><b>$r%1</b></span>"
      "<pre>").arg(m_register);

   if (m_value == 0)
      mText += QString("Value = 0");
   else {
      QString one = getNoInBase(m_value, D_HEX), 
              two = getNoInBase(m_value, D_BINARY), 
              three = getNoInBase(m_value, D_SIGNED_DECIMAL), 
              four = getNoInBase(m_value, D_UNSIGNED_DECIMAL);
      
      mText += QString(
         "Hex:      %2<br>"
         "Binary:   %3<br>"
         "Decimal:  %4<br>"
         "UDecimal: %5")
         .arg(m_register).arg(one, two, three, four);
   }

   mText += QString("</pre>");
   /*
      QString("");
      ("Last modifed by<br>line: <b>%6</b>").arg(QString("N/A"));*/
    //"<ul style=\"list-style: none;\">"
         /*"<li "STYLE_LEFT">Hex</li><li "STYLE_RIGHT">%2</li>"
         "<li "STYLE_LEFT">Binary</li><li "STYLE_RIGHT">%3</li>"
         "<li "STYLE_LEFT">Decimal</li><li "STYLE_RIGHT">%4</li>"
         "<li "STYLE_LEFT">Unsigned</li><li "STYLE_RIGHT">%5</li>"*/
      //"</ul>"
   
   QPoint p2 = p;
   if (!alreadyAdjusted)
      p2 = mapTo(m_parent, p);

   m_parent->showExtended(p2, mText);
}

ValueLabel *IDLabel::getValueLabel() {
   return m_valueLabel;
}

ValueDisplay m_displayType;
ValueLabel::ValueLabel(RegisterPane *regPane, IDLabel *idLabel) 
   : RegisterLabel(regPane), m_idLabel(idLabel)
{
   setValue(0);
}

void ValueLabel::handleClick(QMouseEvent *e) {
   m_idLabel->handleClick(e);
}

void ValueLabel::showExtended(const QPoint &p, bool alreadyAdjusted) {
   m_idLabel->showExtended(mapTo(m_parent, p), true || alreadyAdjusted);
}

void ValueLabel::setValue(unsigned int newValue) {
   setText(getNoInBase(newValue, m_displayType));
}

QString getNoInBase(unsigned int no, int base) {
   const int bases[] = { 16, 2, 10, 10 };
   int base2 = bases[base];
   
   if (m_displayType == D_SIGNED_DECIMAL)
      return QString::number((signed int)no, base2);
   
   return QString::number(no, base2);
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

ExtendedView::ExtendedView(RegisterPane *parent) : QLabel(parent), m_parent(parent)
{
   QPalette pal = palette();
   QColor c(255, 255, 215); // light yellow
   //c.setAlpha(120);
   
   pal.setColor(QPalette::Window, c);
   pal.setColor(QPalette::Foreground, Qt::black);
   setPalette(pal);
   
   setWordWrap(false);
   setAutoFillBackground(true);
   //setMargin(-1);

   setMouseTracking(false);
   setFrameShadow(QFrame::Raised);
   setFrameShape(QFrame::StyledPanel);
   setFocusPolicy(Qt::NoFocus);
   QLabel::hide();
   
   QFont font("Courier New", 11);
   setFont(font);

}

void ExtendedView::mousePressEvent(QMouseEvent *e) {
   this->hide();
   
   e->accept();
}

void ExtendedView::hide() {
   if (!isVisible())
      return;

   if (STATUS_BAR != NULL)
      STATUS_BAR->clearMessage();
   
   QLabel::hide();
}

void ExtendedView::show(const QString &text, const QString &statusText, const QPoint &pos) {
/*   if (isVisible())
      QLabel::hide();*/

   setText(text);
   STATUS_BAR->showMessage(statusText);
   
   const QSize &hint = sizeHint();
   QRect r(pos.x(), pos.y(), hint.width(), hint.height());
   QRect container = m_parent->rect();
   
   // force label to stay within bounds of container
   if (r.right() > container.right()) 
      r.translate(container.right() - r.right(), 0);
   
   if (r.bottom() > container.bottom()) 
      r.translate(0, container.bottom() - r.bottom());
   
   setGeometry(r);

   QLabel::show();
   //connect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));
}

void ExtendedView::mouseMoveEvent(QMouseEvent *e) {
   e->ignore();
   m_parent->testMouseMove(e, mapTo(m_parent, e->pos()));
}

IDDisplay m_registerDisplayType;
RegisterOptionsPane::RegisterOptionsPane(RegisterView *parent) 
   : QWidget(), m_parent(parent)
{
   QWidget *p = new QWidget();
   QGridLayout *l = new QGridLayout();
   
   l->addWidget(new QLabel("Register Display:"), 0, 0, Qt::AlignLeft);
   QComboBox *b = new QComboBox();
   b->addItem(QString("Numbers ($r0, $r1..)"));
   b->addItem(QString("Aliases ($v0, $t0..)"));
   b->setCurrentIndex(m_registerDisplayType);
   connect(b, SIGNAL(currentIndexChanged(int)), this, SLOT(registerDisplayTypeChanged(int)));
   
   l->addWidget(b, 1, 0, Qt::AlignRight);
   
   l->addWidget(new QLabel("Default Base:"), 2, 0, Qt::AlignLeft);
   b = new QComboBox();
   b->addItem(QString("Hex"));
   b->addItem(QString("Binary"));
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

   m_parent->displayTypeChanged();
   // TODO: update all labels
}

void RegisterOptionsPane::defaultBaseChanged(int val) {
   m_displayType = (ValueDisplay)val;
   
   m_parent->displayTypeChanged();
   // TODO: update all labels
}

