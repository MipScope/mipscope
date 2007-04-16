/* ---------------------------------------------- *\
   file: LineNoPane.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007

   Note:  this file is very hacky..
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
   connect(m_editorPane, SIGNAL(activeEditorChanged(TextEditor*)), this, SLOT(activeEditorChanged(TextEditor*)));
   
   updateLineNumbers();
}


LineNo::LineNo(EditorPane *editorPane, LineNoDisplay *parent) : QLabel(parent), m_editorPane(editorPane)
{
   setFont(*m_editorPane->m_font);
   
   setMargin(0);
   setWordWrap(false);
   hide();
}

void LineNo::mousePressEvent(QMouseEvent *e) { e->ignore(); }

void LineNo::mouseReleaseEvent(QMouseEvent *e) {
   if (rect().contains(e->pos())) {
      
      cerr << text().toStdString() << " was pressed\n";
   }
}

void LineNo::setLineNo(int val) {
   m_lineNo = val;
   
   if (val < 10)
      setText(QString("%1  ").arg(m_lineNo));
   else setText(QString("%1").arg(m_lineNo));
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

void LineNoDisplay::activeEditorChanged(TextEditor *active) {
   active = NULL;
   updateLineNumbers();
}

void LineNoDisplay::updateLineNumbers() {
   const TextEditor *active = m_editorPane->m_activeEditor;
   QRect rect = active->visibleRegion().boundingRect();
   rect.adjust(2, 0, 0, -2);
   
   QTextCursor firstVisible = active->cursorForPosition(rect.topLeft());
   const QTextCursor &lastVisible = active->cursorForPosition(rect.bottomLeft());
   
   int firstLineNo = active->lineNumber(firstVisible);
   int lastLineNo  = active->lineNumber(lastVisible);
   
   /*QString lineNo = "\n\n";
   
   for(int i = firstLineNo; i <= lastLineNo; i++) {
      lineNo += QString("%1\n").arg(i);
   }*/
   
   QTextCursor cur = firstVisible;
   LineListIterator li = m_labels.begin();
   QRect bounds;
   rect = active->cursorRect(firstVisible);
   
   if (firstLineNo == lastLineNo) {
      lastLineNo = firstLineNo + 219; // hack, yippee :)
      rect.adjust(0, 2, 0, 2);
   }
   
   int max = active->noLines();
   QLabel maxSized(QString("%1").arg(max > lastLineNo ? max : lastLineNo));
   maxSized.setFont(*m_editorPane->m_font);
   int width = maxSized.sizeHint().width();
   
   for(int i = 0; i < lastLineNo - firstLineNo + 1; i++) {
      int lineNo = firstLineNo + i;//active->lineNumber(cur);
      
      (*li)->setLineNo(lineNo);//setText(QString("%1").arg(lineNo));
      
      QTabBar *tabBar = m_editorPane->tabBar();
      int offset = tabBar->contentsRect().height() + 2;
      
      QPoint p = rect.adjusted(0, offset, 0, 0).topLeft();
      QRect r(0, p.y(), width/*(*li)->sizeHint().width()*/, rect.height());
      (*li)->setGeometry(r);

      bounds = bounds.united(r);
      (*li)->show();
      
      li++;
      
      int oldPos = cur.position();
      cur.movePosition(QTextCursor::Down);
      
      if (cur.position() == oldPos) {
         rect.adjust(0, rect.height(), 0, rect.height());
         if (lastLineNo - firstLineNo + 1 >= i + 2) {
            //lastLineNo = 200 + firstLineNo;
            cerr << oldPos << endl;
         }
      } else rect = active->cursorRect(cur);
   }
   
   while(li != m_labels.end()) {
      (*li)->hide();
      li++;
   }
   //cerr << "(" << firstLineNo << ", " << lastLineNo << ")\n";
   //cerr << lineNo.toStdString() << endl;
   
   m_rect = bounds.adjusted(4, 0, (lastLineNo - firstLineNo >= 200 ? 0 : 10), 0);
   
   updateGeometry();
}

QSize LineNoDisplay::minimumSize() const {
   return m_rect.size();
}

QSize LineNoDisplay::sizeHint() const {
   return m_rect.size();
}

