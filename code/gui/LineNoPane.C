/* ---------------------------------------------- *\
   file: LineNoPane.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007

   Note:  this file is very, very hacky..
\* ---------------------------------------------- */
#include "LineNoPane.H"
#include "BorderLayout.H"
#include "EditorPane.H"
#include "Gui.H"
#include "Utilities.H"
#include <QtGui>

// Right-click context-menu for tabs
// ---------------------------------
LineNoPane::LineNoPane(Gui *gui, EditorPane *editorPane)
   : QWidget(), m_gui(gui), m_editorPane(editorPane)
{
   BorderLayout *layout = new BorderLayout();
   m_label = new LineNoDisplay(m_editorPane);
   
   layout->addWidget(editorPane, BorderLayout::Center);
   layout->addWidget(m_label, BorderLayout::West);
   
   setLayout(layout);
}

LineNoDisplay::LineNoDisplay(EditorPane *editorPane) 
   : QWidget(), m_editorPane(editorPane), 
   m_active(m_editorPane->m_activeEditor)
{  
   for(int i = 0; i < 220; i++) {
      LineNo *cur = new LineNo(m_editorPane, this);
      
      m_labels.push_back(cur);
      //addWidget(cur);
   }
   
   /*setFrameShadow(QFrame::Raised);
   setFrameShape(QFrame::StyledPanel);*/
   setFocusPolicy(Qt::NoFocus);
   m_editorPane->m_lineNoDisplay = this;
   
   connect(m_editorPane, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   connect(m_editorPane, SIGNAL(editorScrolled(TextEditor*, int)), this, SLOT(editorScrolled(TextEditor*,int)));
   connect(m_editorPane, SIGNAL(activeEditorChanged(TextEditor*)), this, SLOT(contentChanged(TextEditor*)));
   connect(m_editorPane, SIGNAL(contentChanged(TextEditor*)), this, SLOT(contentChanged(TextEditor*)));
   
   updateLineNumbers();
}

QPixmap *m_breakPoint;

LineNo::LineNo(EditorPane *editorPane, LineNoDisplay *parent) : QLabel(parent), m_editorPane(editorPane), m_lineNo(-1), m_last(NULL)
{
   setFont(*m_editorPane->m_font);
   if (m_breakPoint == NULL)
      m_breakPoint = new QPixmap(IMAGES"/breakPoint.png");
   
   setMargin(0);
   setWordWrap(false);
   hide();
}

void LineNo::mousePressEvent(QMouseEvent *e) { e->ignore(); }

void LineNo::mouseReleaseEvent(QMouseEvent *e) {
   if (rect().contains(e->pos()) && (e->button() == Qt::LeftButton)) {
//      cerr << text().toStdString() << " was pressed\n";
//      QPixmap p = mergeSideBySide(*m_breakPoint, text());
      
      QTextBlock *b = m_last = m_editorPane->m_activeEditor->getBlockForLine(m_lineNo, m_last);
      if (b != NULL) {
         // Toggle display of breakpoint
         if (b->userState() == B_BREAKPOINT) {
            b->setUserState(B_NORMAL);
         } else b->setUserState(B_BREAKPOINT);
         //m_editorPane->m_activeEditor->highlightLine(
      }
      
      resetDisplay(b);
   }
}

void LineNo::setLineNo(int val) {
   m_lineNo = val;
   
   resetDisplay();
}

void LineNo::resetDisplay(QTextBlock *b) {
   bool possibleBP = !(b == NULL && (b = m_last = m_editorPane->m_activeEditor->getBlockForLine(m_lineNo, m_last)) == NULL);
   
   if (possibleBP && b->userState() == B_BREAKPOINT)
      setPixmap(*m_breakPoint);

   else if (m_lineNo < 10)
      setText(QString("%1  ").arg(m_lineNo));
   else {
      int max = m_editorPane->m_activeEditor->noLines();
      
      setText(QString((max > 99 ? "%1  " : "%1")).arg(m_lineNo));
   }
}

QPixmap LineNo::mergeSideBySide(const QPixmap& pix, const QString &txt)
{
   QPainter p;
//   cerr << m_editorPane->m_font << endl;
   
   int strWidth = p.fontMetrics().width( txt );
   int strHeight = p.fontMetrics().height();
   
   int pixWidth = pix.width();
   int pixHeight = pix.height();
   int maxHeight = (strHeight > pixHeight ? strHeight : pixHeight);
   
   QPixmap res(strWidth + 3 + pixWidth, maxHeight);
//   res.fill(Qt::white);
   res.fill(QColor(255, 255, 255, 0));
   
   p.begin(&res);
   p.setFont(font());
   p.drawPixmap(pixWidth + 3, 0, pix);
   p.drawText(QRect( 0, 0, strWidth, strHeight), 0, txt);
   p.end();
   
   return res;
}

void LineNoDisplay::fontChanged(const QFont &font) {
   for(LineListIterator li = m_labels.begin(); li != m_labels.end(); li++) {
      (*li)->setFont(font);
   }
}

void LineNoDisplay::editorScrolled(TextEditor *active, int val) {
   active = NULL;
   val = 0;
   updateLineNumbers();
}

// called whenever active editor is switched or 
// active editor's content is modified
void LineNoDisplay::contentChanged(TextEditor *active) {
   active = NULL;
   updateLineNumbers();
}

void LineNoDisplay::updateLineNumbers() {
   const TextEditor *active = m_editorPane->m_activeEditor;
   QRect rect = active->visibleRegion().boundingRect();
   int bottom = rect.bottom();
   rect.adjust(2, 0, 0, -2);
   
   QTextCursor firstVisible = active->cursorForPosition(rect.topLeft());
   const QTextCursor &lastVisible = active->cursorForPosition(rect.bottomLeft());
   
   int firstLineNo = active->lineNumber(firstVisible);
   int lastLineNo  = active->lineNumber(lastVisible);
   
   int noLines = active->noLines(), max = noLines;
   if (firstLineNo == lastLineNo && firstLineNo == 0 && noLines > 1) {
      //cerr << "earlyexit\n";
      return;
   }

   /*QString lineNo = "\n\n";
   
   for(int i = firstLineNo; i <= lastLineNo; i++) {
      lineNo += QString("%1\n").arg(i);
   }*/
   
   QTextCursor cur = firstVisible;
   LineListIterator li = m_labels.begin();
   QRect bounds;
   rect = active->cursorRect(firstVisible);
   
   if (firstLineNo >= lastLineNo/* || lastLineNo < 50*/) {
      lastLineNo = firstLineNo + 118; // hack, yippee :)
      rect.adjust(0, 2, 0, 2);
   }
   
   if (lastLineNo > max)
      lastLineNo = max;
   
   max = (max > lastLineNo ? max : lastLineNo);
   
   QLabel maxSized(QString("%1").arg(max));
   maxSized.setFont(*m_editorPane->m_font);
   int width = maxSized.sizeHint().width();// + m_breakPoint->width();
   if (width < m_breakPoint->width() + 2)
      width = m_breakPoint->width() + 2;
   //cerr << max << endl;
   
   int i;
   for(i = 0; i < lastLineNo - firstLineNo + 1; i++) {
      int lineNo = firstLineNo + i;//active->lineNumber(cur);
      
      (*li)->setLineNo(lineNo);//setText(QString("%1").arg(lineNo));
      
      QTabBar *tabBar = m_editorPane->tabBar();
      int offset = tabBar->contentsRect().height() + 2;
      
      QPoint p = rect.adjusted(0, offset, 0, 0).topLeft();
           //if ((*li)->sizeHint().width() > width)
      //   cerr << (*li)->sizeHint().width() << " > " << width << endl;
      
      QRect r(0, p.y(), width/*(*li)->sizeHint().width()*/, rect.height());
      (*li)->setGeometry(r);
      
      bounds = bounds.united(r);
      if (i == 0)
         bottom += p.y() + rect.height();
      else if (p.y() > bottom) {
         //cerr << "Breaking; i = ";
         break;
      }
      
      (*li)->show();
      li++;
      
      int oldPos = cur.position();
      cur.movePosition(QTextCursor::Down);
      
      if (cur.position() == oldPos) {
         rect.adjust(0, rect.height(), 0, rect.height());

         if (lastLineNo - firstLineNo + 1 >= i + 1) {
            lastLineNo = 100 + firstLineNo;
            
            if (lastLineNo > noLines)
               lastLineNo = noLines;
      //      cerr << oldPos << endl;
         }
      } else rect = active->cursorRect(cur);
   }
   
   while(li != m_labels.end()) {
      (*li)->hide();
      li++;
   }
   
   //cerr << i << " \t Updating: (" << firstLineNo << ", " << lastLineNo << ")\n";
   //cerr << lineNo.toStdString() << endl;
   
   m_rect = bounds.adjusted(4, 0, (lastLineNo - firstLineNo >= 100 ? 0 : 10), 0);
   
   updateGeometry();
}

QSize LineNoDisplay::minimumSize() const {
   return m_rect.size();
}

QSize LineNoDisplay::sizeHint() const {
   return m_rect.size();
}

