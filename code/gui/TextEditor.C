/* ---------------------------------------------- *\
   file: TextEditor.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "TextEditor.H"
#include "SyntaxHighlighter.H"
#include "EditorPane.H"
#include "Utilities.H"
#include "Gui.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Statement.H"
#include "Program.H"
#include <QtGui>
#include <QtCore>

TextEditor::TextEditor(EditorPane *parent, QFont *font, QFile *file, TextEditor *prev)
   : QTextEdit(), m_parent(parent), m_prev(prev), m_next(NULL), m_file(NULL), 
   m_loaded(false), m_modified(false), m_undoAvailable(false), 
   m_redoAvailable(false), m_syntaxHighligher(NULL), m_program(new Program(parent->m_parent, parent, this)), m_pc(NULL)
{
   setupEditor(font);
   
   if (file != NULL)
      openFile(file);
   
   if (m_prev != NULL) {
      m_next = m_prev->m_next;
      m_prev->m_next = this;

      if (m_next != NULL)
         m_next->m_prev = this;
   }
}

TextEditor::~TextEditor(void) { }

void TextEditor::setupEditor(QFont *font) {
   setFont(*font);
   setTabStopWidth(4 * QFontMetrics(*font).width(' '));
   
   // strip formatting when pasting rich-text
   setAcceptRichText(false);
   setLineWrapMode(QTextEdit::NoWrap);
  
   m_syntaxTip = new SyntaxTip(this);
   
//   m_commentLine = new QShortcut(QKeySequence(tr("CTRL+D")), this, SLOT(toggleComment()), SLOT(toggleComment()), Qt::WidgetShortcut);
   
   
   //setContextMenuPolicy(Qt::CustomContextMenu);
   setContextMenuPolicy(Qt::NoContextMenu);
   connect(this, SIGNAL(textChanged()), this, SLOT(codeChanged()));
   connect(this, SIGNAL(textChanged()), m_parent, SLOT(contentChangedProxy()));

   // TODO:  temp?  optimize!
   connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorPosition()));
   
   // signals for enabling/disabling editing functionality
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));

   connect(this, SIGNAL(undoAvailable(bool)), m_parent, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(redoAvailable(bool)), m_parent, SLOT(redoAvailabilityModified(bool)));
   connect(this, SIGNAL(copyAvailable(bool)), m_parent, SLOT(copyAvailabilityModified(bool)));
   connect(verticalScrollBar(), SIGNAL(valueChanged(int)), m_parent, SLOT(updateLineNumbers(int)));
   connect(verticalScrollBar(), SIGNAL(valueChanged(int)), m_syntaxTip, SLOT(editorScrolled(int)));
//   connect(m_parent, SIGNAL(isModifiable(bool)), this, SLOT(modifiabilityChanged(bool)));
   connect(m_parent, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   
   //modifiabilityChanged(m_parent->isModifiable());
   m_syntaxHighligher = new SyntaxHighlighter(this);
}

// Slot, notified every time text is changed in the parent TextEdit
void TextEditor::codeChanged() {
   if (!isModified())
      resetTabText(false);
   else if (!m_modified)
      resetTabText(true);
   
   m_parent->isModified(this->isModified());
}

// @overridden to deal w/ showing syntax help after typing an instruction
void TextEditor::keyReleaseEvent(QKeyEvent *e) {
   const int key = e->key();
   
   // check if previously typed word was an instruction, immediately after the 
   // user completes it by typing the space bar (for syntax-help)
   if (key == Qt::Key_Space && !textCursor().hasSelection()) {
      QTextCursor c = textCursor();
      c.movePosition(QTextCursor::StartOfLine/*PreviousWord*/, QTextCursor::KeepAnchor);
      const QString &selectedText = c.selectedText().trimmed();
//      bool match = m_syntaxHighligher->matches(selectedText);
      bool match1 = (selectedText != "" && instructionMap.contains(selectedText)), match2 = false;
      
//      cerr << selectedText.toStdString() << ", " << match1 << ", " << match2 << endl;
//      cerr << instructionMap.contains(selectedText) << ", " << (void*)instructionMap[selectedText] << endl;

      if (match1 || (match2 = directiveMap.contains(selectedText))) {
         // move cursor to start of matched word
         c = textCursor();
         c.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
         const QRect &rect = cursorRect(c);
//         const int top = rect.top();
         const QPoint pos(rect.center().x() + 2, rect.bottom() + 1);
               //top + ((rect.bottom() - top) >> 3));
         
         const Statement *statement = (match1 ? (Statement*)instructionMap[selectedText] : (Statement*)directiveMap[selectedText]);
         if (statement != NULL) {
            const QString text = QString("<p style='white-space:pre'><b>%1</b> %2").arg(selectedText, QString(statement->getSyntax()));

            const QString statusText = statement->getDescription();
            //"Adds two signed integers, storing the result in $dest.";

            //cerr << text.toStdString() << endl;
            //cerr << statusText.toStdString() << endl;

            m_syntaxTip->show(text, statusText, pos, c);

//         highlightLine(c, Qt::red);
//         QToolTip::showText(mapToGlobal(pos), text, this);
         }
      }
   } else if (key == Qt::Key_Return) {
//      QToolTip::hideText();
      m_syntaxTip->hide();
      
      QTextCursor c = textCursor();
      int pos = c.position();
      c.movePosition(QTextCursor::Up);
      c.select(QTextCursor::LineUnderCursor);
      const QString text = c.selectedText();
      
      // auto-insert comment
      if (text.trimmed().at(0) == QChar('#')) { // previous line is a comment
         int commentIndex = text.indexOf(QChar('#'));
         if (commentIndex >= 0) {
            const QString &whitespace = text.left(commentIndex + 1) + QString(" ");
            c.setPosition(pos);
            c.insertText(whitespace);
         }
      } else {
         int nonWhiteSpaceInd = text.indexOf(QRegExp("[^ \t]"));
         if (nonWhiteSpaceInd == -1)
            nonWhiteSpaceInd = text.length();

         if (nonWhiteSpaceInd >= 0) {
            const QString &whitespace = text.left(nonWhiteSpaceInd);

            c.setPosition(pos);
            c.insertText(whitespace);
         }
      }
   } else if (key == Qt::Key_Escape) {
      m_syntaxTip->hide();
   }/* else if (m_syntaxTip->isVisible()) {
      m_syntaxTip->cursorPositionChanged(textCursor());
   } else if (key == Qt::Key_Tab) {
      QTextCursor c = textCursor();
      c.deletePreviousChar();
      c.insertText("   ");
      setTextCursor(c);

      e->accept();
      return;
      //insertPlainText("   ");
   }*/
   /*else if (key == Qt::Key_Tab) {
      if (m_lastTab->elapsed() > 200) {
         m_lastTab->restart();
         
         QTextCursor c = textCursor();
         insertPlainText("   ");
      }
      
      e->accept();
      return;
   }*/
   
//   cerr << textCursor().block().text().toStdString() << endl;
//   cerr << currentLineNumber() << endl;
   
   QTextEdit::keyReleaseEvent(e);
}

/*void TextEditor::contextMenuEvent(QContextMenuEvent *e) {
   m_parent->showContextMenu(e);
}*/

void TextEditor::toggleComment() {
//   cerr << "CTRL+D pressed\n";
   extern const char *CommentSyntax;

   QRegExp comment(QString("\\s?") + QString(CommentSyntax));
   QTextCursor c = textCursor();

   // block toggle comment
   if (c.hasSelection()) {
      int start = c.selectionStart(), end = c.selectionEnd(), prevPos, pos = start;
      QTextCursor c2 = textCursor();
      c2.setPosition(end);
      if (c2.atBlockStart())
         end += 2;
      
      bool inComment = true;
      c.setPosition(start);;
      c.movePosition(QTextCursor::StartOfLine);
      
      do {
         
         if (!comment.exactMatch(c.block().text())) { // line is not already commented
//            cerr << "Line no: " << lineNumber(c) << ";  " << c.position() << " vs " << end << endl;
            
            inComment = false;
            break;
         }
         
         c.movePosition(QTextCursor::NextBlock);
         c.movePosition(QTextCursor::StartOfLine);
         prevPos = pos;
         pos = c.position();
      } while(pos <= end && prevPos < pos);
      
//      cerr << "done checking: " << inComment << endl;

      c.beginEditBlock();
      pos = start;
      c.setPosition(pos);
      c.movePosition(QTextCursor::StartOfLine);
      if (inComment) { // all lines are in comment
         do {
            c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

            if (c.selectedText() == QString("#"))
               c.removeSelectedText();
            
            c.movePosition(QTextCursor::NextBlock);
            c.movePosition(QTextCursor::StartOfLine);
         } while(pos <= --end);

      } else { // not all lines are in comment
         int lastLine = noLines();
         
         do {
//            int pos = c.position();
//            c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            
            //if (c.selectedText().remove(QChar(' ')) != "\n")
            //   c.setPosition(pos);
            c.insertText("#");
            
            if (c.blockNumber() < lastLine) {
               c.movePosition(QTextCursor::NextBlock);
               c.movePosition(QTextCursor::StartOfLine);
            } else break;
            prevPos = pos;
            pos = c.position();
//            cerr << "\tLooping " << "pos " << c.position() << " end " << end << endl;
         } while(pos <= ++end && prevPos < pos);

//         cerr << "pos " << c.position() << " end " << end << endl;
      }

      c.endEditBlock();
   } else { // only toggling comment on one line
      c.movePosition(QTextCursor::StartOfLine);

      if (comment.exactMatch(c.block().text())) { // line is already commented
         c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

         if (c.selectedText() == QString("#"))
            c.removeSelectedText();
         //else cerr << "\n\nerror w/ regExp after CTRL+D\n\n";
      } else c.insertText("#");
   }
}

void TextEditor::clearLastInstructions() {
   m_lastInstructions.clear();
}

void TextEditor::pcChanged(ParseNode *pc) {
   ParseNode *old = m_pc;
//   cerr << "\tpcChanged id:" << QThread::currentThreadId() << "\n";
   
   if (old == pc)
      return;
   
   m_pc = pc;
//   if (m_program->getStatus() != PAUSED)
//      return;
   
   if (old != NULL) {
      QTextBlock *b = old->getTextBlock();
      int state = b->userState();
      if (state > 0)
         b->setUserState(state & ~B_CURRENT_PC);
   }

   if (m_pc != NULL) {
      QTextBlock *b = m_pc->getTextBlock();
//      cerr << "b: " << b->text().toStdString() << endl;
      int state = b->userState();
      if (state < 0)
         b->setUserState(B_CURRENT_PC);
      else b->setUserState(state | B_CURRENT_PC);
      
      QTextCursor c = textCursor();
      c.setPosition(b->position());
      setTextCursor(c);
      ensureCursorVisible();
   }
   
   m_parent->updateLineNumbers(0);
   // the 2 can be changed according to preferences later
   // just be sure to also change the fading factors in paintEvent.
   m_program->getLastXInstructions(2, m_lastInstructions);
   viewport()->update();
}

void TextEditor::programStatusChanged(int status) {
//   cerr << "TextEditor::programStatusChanged(" << status << ")\n";
//   cerr << "\t" << QThread::currentThreadId() << "\n";

   if (status == PAUSED)
      pcChanged(m_program->getPC());
   else pcChanged(NULL);
}

void TextEditor::updateCursorPosition() {
   viewport()->update();
}

void TextEditor::paintEvent(QPaintEvent *e) {
   int width = viewport()->width(), height = fontMetrics().height();
   QRect cRect(0, cursorRect().y(), viewport()->width(), height);
   const int cursorPos = textCursor().block().position();
   
   QPainter p(viewport());
   // good for highlighting line during paused execution:
   p.fillRect(cRect, QBrush(QColor(215, 227, 255)));
   
   if (m_pc != NULL && m_program->getStatus() == PAUSED) {
      QColor highlightColor = QColor(255, 240, 117); // yellow
      highlightLine(p, m_pc, highlightColor, width, height);
      
//      highlightColor = highlightColor.lighter(105);
//      cerr << "<<<Painting: size = " << m_lastInstructions.size() << endl;
      foreach(ParseNode *parseNode, m_lastInstructions) {
         highlightColor = highlightColor.lighter(120);
         if (highlightColor == Qt::white) {
            //cerr << "\t" << i << " painted!\n";
            break;
         }
         
         const QTextBlock *block = parseNode->getTextBlock();
         if (block != NULL && parseNode != m_pc && cursorPos != block->position())
            highlightLine(p, parseNode, highlightColor, width, height);
      }
   }
   
   p.end();
   //setTabStopWidth(4 * QFontMetrics(f).width(' '));
   
   QTextEdit::paintEvent(e);
}

void TextEditor::resizeEvent(QResizeEvent *e) {
   QTextEdit::resizeEvent(e);
   
   if (m_parent != NULL)
      m_parent->updateLineNumbers(0);
}

void TextEditor::highlightLine(QPainter &p, ParseNode *parseNode, const QColor &color, const int width, const int height) {
   const QTextBlock *block;
   if (parseNode == NULL || ((block = parseNode->getTextBlock()) == NULL))
      return;
   
   int pos = block->position();
//   cerr << "\tPos = " << pos << ", block=" << block->text().toStdString() << endl;
   if (pos >= 0) {
      QTextCursor c = textCursor();
      c.setPosition(pos);
      
      QRect r(0, cursorRect(c).y(), width, height);
      p.fillRect(r, QBrush(color));
   }
}

bool TextEditor::openFile(QFile *file) {
   if (file->open(QIODevice::ReadOnly | QFile::Text)) {
//      setUpdatesEnabled(false);
      m_lineToPosMap.clear();
      
//      QTextDocument *m = new QTextDocument();
//      m->setPlainText(file->readAll());
//      setDocument(m);
      setPlainText(file->readAll());
      file->close();

      m_file = file;
      m_loaded = true;
      resetTabText();
      
      qApp->processEvents();

      // hack to get this shit to display right.. fuck!
      QTimer::singleShot(250, m_parent, SLOT(contentChangedProxy()));
      QTimer::singleShot(750, m_parent, SLOT(contentChangedProxy()));
      QTimer::singleShot(1300, m_parent, SLOT(contentChangedProxy()));
      //modifiabilityChanged(m_parent->isModifiable());
      
//      setUpdatesEnabled(true);
   } // else display error
   
   return true;
}

// returns actual filename of open file
QString TextEditor::fileName() const {
   if (m_file == NULL)
      return QString(UNTITLED);

   QString path = m_file->fileName();
   
   int index = path.lastIndexOf('/');
   if (index < 0)
      return path;
   
   return path.right(path.length() - index - 1);
}

void TextEditor::resetTabText(bool modified) {
   const QString &name = fileName();
   m_modified = modified;
   int index = m_parent->indexOf(this);
   
   m_parent->setTabText(index, name);// + (m_modified ? "*" : ""));
   const QIcon &icon = (isModifiable() ? (m_modified ? QIcon(IMAGES"/fileUnsaved.png") : 
            QIcon(IMAGES"/fileSaved.png")) : QIcon(IMAGES"/fileLocked.png"));

   m_parent->setTabIcon(index, icon);
}

bool Accepted(QMessageBox::StandardButton result) {
   return (result == QMessageBox::Yes || result == QMessageBox::YesToAll || 
           result == QMessageBox::No || result == QMessageBox::NoToAll);
}

// returns true upon successful closure
QMessageBox::StandardButton TextEditor::close(bool promptForSave) {
   QMessageBox::StandardButton ret = QMessageBox::NoButton;
   if (m_parent->m_activeEditor != this) {
      m_parent->setActiveEditor(this);
      m_parent->update();
   }
   
   // Prompt the user to save any unsaved changes
   if (promptForSave && isModified() && !Accepted(ret = promptUnsavedChanges()))
      return ret;
   
   if (m_next != NULL)
      m_next->m_prev = m_prev;
   
   if (m_prev != NULL) {
      m_prev->m_next = m_next;
      m_parent->setActiveEditor(m_parent->m_activeEditor->m_prev);
   } else m_parent->setActiveEditor(m_parent->m_activeEditor->m_next);
   
   m_parent->removeTab(m_parent->indexOf(this));

   return ret;
}

// Returns true if, after executing, there are no pending changes
// (prompts user chose to save pending changes)
// returns false if file was not saved.
QMessageBox::StandardButton TextEditor::promptUnsavedChanges(QMessageBox::StandardButtons extraButtons) {
   if (isModified()) {
      if (m_parent->m_activeEditor != this) {
         m_parent->setActiveEditor(this);
         m_parent->update();
      }

      QString name = fileName();
      QString text;
      if (name == "")
         text = "This file has not been saved";
      else text = QString("The file '") + fileName() + QString("' has been modified");
      
      text += ".\n\nWould you like to save your changes?";
      
      //QMessageBox::StandardButton ret = 
      QMessageBox prompt(QMessageBox::Question, tr("Save changes?"), text, 
         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | 
         extraButtons, this);
      prompt.setDefaultButton(QMessageBox::Yes);
      
      //QMessageBox prompt(tr("Save changes?"), text, QMessageBox::Question, 
      //      QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel, this);
      //extraButtons = 0;
      
      QMessageBox::StandardButton ret = (QMessageBox::StandardButton) prompt.exec();
      
      if (ret == QMessageBox::Yes || ret == QMessageBox::YesToAll) {
         QMessageBox::StandardButton result = save();
         
         if (Accepted(result))
            return ret;
         
         return result;
      }
      
      return ret;
   }
   
   return QMessageBox::Yes;
}

// returns true if there are no pending changes after executing this method.
QMessageBox::StandardButton TextEditor::save(bool forceSave) {
   if (!forceSave && !isModified())
      return QMessageBox::Yes;

   if (m_file == NULL)
      return saveAs();
   
   const QString &filename = fileName();
   QString text = toPlainText();
   if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text)) {
      //cerr << "Could not write to file: '" << filename << "'" << endl;
      if (STATUS_BAR != NULL)
         STATUS_BAR->showMessage( QString("Could not write to %1").arg(fileName()),
            STATUS_DELAY );
      
      return QMessageBox::Abort;
   }

   QTextStream t(m_file);
   t << text;
   m_file->close();
   
   m_loaded = true;
   document()->setModified(false);
   m_parent->isModified(false);
   resetTabText(false);
   
   if (STATUS_BAR != NULL)
      STATUS_BAR->showMessage( QString( "File %1 saved" ).arg(filename), STATUS_DELAY );

   return QMessageBox::Yes;
}

QMessageBox::StandardButton TextEditor::saveAs() {
   const QString &filename = QFileDialog::getSaveFileName(this, tr("Save As"), 
      /*m_lastDirectoryOpened*/QString(), FILE_FILTER);
   
   if (filename == "")
      return QMessageBox::Cancel; // user selected cancel

   m_file = new QFile(filename);
   return save(true); // force save even if document is unmodified
}

bool TextEditor::isModified() const {
   return document()->isModified();
}

void TextEditor::modifiabilityChanged(bool isModifiable) {
   setModifiable(isModifiable);
}

void TextEditor::setModifiable(bool isModifiable) {
   setReadOnly(!isModifiable);
   resetTabText(m_modified);
}

bool TextEditor::isModifiable() const {
   return !isReadOnly();
}

bool TextEditor::copyIsAvailable() const {
   return (textCursor().hasSelection());
}

bool TextEditor::undoIsAvailable() const {
   return m_undoAvailable;
}

bool TextEditor::redoIsAvailable() const {
   return m_redoAvailable;
}

void TextEditor::undoAvailabilityModified(bool isAvailable) {
   m_undoAvailable = isAvailable;
}

void TextEditor::redoAvailabilityModified(bool isAvailable) {
   m_redoAvailable = isAvailable;
}

SyntaxTip::SyntaxTip(TextEditor *parent) : QLabel(parent), m_parent(parent), m_block(NULL) {
   QPalette pal = palette();
   QColor c(255, 255, 215); // light yellow
   //c.setAlpha(100);

   pal.setColor(QPalette::Window, c);
   pal.setColor(QPalette::Foreground, Qt::black);
   setPalette(pal);

   setWordWrap(false);
   setAutoFillBackground(true);
   setMargin(-3);

   //m_syntaxTip->setFrameShadow(QFrame::Raised);
   setFrameShape(QFrame::StyledPanel);
   setFocusPolicy(Qt::NoFocus);
   hide();
   // TODO:  check if change is okay/bad from original!!!
   
   /*QFont font(currentFont());
   font.setPointSize(12);
   m_syntaxTip->setFont(font);*/
}

void SyntaxTip::mousePressEvent(QMouseEvent *e) {
   this->hide();
   
   e->ignore();
}

void SyntaxTip::hide() {
   if (STATUS_BAR != NULL)
      STATUS_BAR->clearMessage();
   
   QLabel::hide();

   //disconnect(m_parent->document(), SIGNAL(cursorPositionChanged(const QTextCursor&)), this, SLOT(cursorPositionChanged(const QTextCursor&)));
   disconnect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));

   safeDelete(m_block);
}

void SyntaxTip::cursorPositionChanged(const QTextCursor &cursor) {
   hide();
}

void SyntaxTip::testCursorPos() {
   QTextCursor c = m_parent->textCursor();
   if (c.block() != *m_block)
      hide();
   c.movePosition(QTextCursor::StartOfLine);
   c.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
   QString selectedText = c.selectedText().trimmed();
   if (selectedText == "") {
      c.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
      selectedText = c.selectedText().trimmed();
   }
   
//   cerr << "sel: '" << selectedText.toStdString() << "'\n";
   if (!(instructionMap.contains(selectedText) || 
       directiveMap.contains(selectedText)))
      hide();
}

void SyntaxTip::editorScrolled(int val) {
   if (isVisible()) {
      if (!m_block->isValid()) {
         hide();
      } else {
         const QRect &geom = geometry();
         //m_startPos = m_block->position();
         QTextCursor c = m_parent->textCursor();

         c.setPosition(m_block->position());//m_startPos);
         const QRect &rect = m_parent->cursorRect(c);
         val = 0;

         setGeometry(geom.x(), rect.bottom() + 1, geom.width(), geom.height());
      }
   }
}

void SyntaxTip::show(const QString &text, const QString &statusText, const QPoint &pos, const QTextCursor &c) {
   m_block = new QTextBlock(c.block());

//   m_cursor = new QTextCursor(c);
//   m_cursor->movePosition(QTextCursor::StartOfLine);
//   m_startPos = m_cursor->position();
   
   setText(text);
   STATUS_BAR->showMessage(statusText);
   
   const QSize &hint = sizeHint();
   setGeometry(pos.x(), pos.y(), hint.width(), hint.height());
   
//   connect(m_parent->document(), SIGNAL(cursorPositionChanged(const QTextCursor&)), this, SLOT(cursorPositionChanged(const QTextCursor&)));
   connect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));

   QLabel::show();
}


// ------------------------------------------------------
// Utility Methods to make working with QTextEdit easier!
// ------------------------------------------------------

// returns the line number of the given text block
int TextEditor::lineNumber(const QTextBlock &b) const {
   // if you have a QTextCursor, easy to get blockNumber()
   // if you have a QTextCursor, easy to get corresponding block(), position()
   if (!b.isValid())
      return -1;

   QTextCursor c = textCursor();
   c.setPosition(b.position());

   return lineNumber(c);
}

// Retursn the line number of the given cursor
int TextEditor::lineNumber(const QTextCursor &c) const {
   return c.blockNumber();
}

// Returns the line no of the current cursor
int TextEditor::currentLineNumber() const {
   return lineNumber(textCursor());
}

// Returns the total number of lines
int TextEditor::noLines() const {
   return document()->blockCount() - 1;
/* QTextCursor sec = textCursor();
   sec.movePosition(QTextCursor::End);

   return sec.blockNumber();*/
   /*
   int pos;
   
   do {
      pos = sec.position();
      sec.movePosition(QTextCursor::Down);
   } while (pos != sec.position());
   
   return sec.blockNumber();*/
}

// Returns the QTextBlock pertaining to the given line number
// Returns NULL if the given line number is invalid
QTextBlock *TextEditor::getBlockForLine(int lineNo, QTextBlock *last) {
   QTextCursor c = textCursor();
//   cerr << c.position() << " entering getBlockForLine(" << lineNo << ", " << last << ");\n";

   if (last != NULL && last->isValid()) {
      c.movePosition(QTextCursor::End);
      int lastPosition = last->position();
      if (c.position() >= lastPosition) {
         c.setPosition(lastPosition);

         if (c.blockNumber() == lineNo) {
            //      cerr << "\texiting last\n";
            return last;
         }
      }
   }
   
//   cerr << c.blockNumber() << " vs " << lineNo << endl;
   LineToMapIterator i = m_lineToPosMap.find(lineNo);
   if (i != m_lineToPosMap.end()) {
      int prospectPos = i->second;

      c.setPosition(prospectPos);
      if (c.blockNumber() == lineNo) {
     //    cerr << "\texiting cached\n";
         return new QTextBlock(c.block());
      }
      if (m_lineToPosMap.size() > 0)
         m_lineToPosMap.clear();
      //else cerr << "changed\n";
   }
   
   c.movePosition(QTextCursor::Start);
   int before = c.position();
   c.movePosition(QTextCursor::End);
   int after = c.position(), blockNo;

   if (c.blockNumber() != lineNo) {
      int pos = -1, oldPos;

      do {
         c.setPosition((before + after) / 2);
         blockNo = c.blockNumber();
         oldPos = pos;
         pos = c.position();

         if (blockNo < lineNo)
            before = pos;
         else if (blockNo > lineNo)
            after = pos;
         //      cerr << c.blockNumber() << " vs " << lineNo << endl;
      } while(blockNo != lineNo && before != after && oldPos != pos);
   }
   
   m_lineToPosMap[lineNo] = c.position();
   //cerr << "\t" << c.position() << " normal exit from getBlockForLine(" << lineNo << ", " << last << ");\n";
   
   return new QTextBlock(c.block());
}

// Moves text cursor to given line, scrolling editor if necessary
void TextEditor::gotoLine(int lineNo) {
   int max = noLines();
//   cerr << "lineNo: " << lineNo << ", max=" << max << endl;

   if (lineNo > max) // cap to ensure lineNo is valid
      lineNo = max;
   else if (lineNo < 0)
      lineNo = 0;
   
   QTextCursor c = textCursor();
   QTextBlock *b = getBlockForLine(lineNo);
   int pos = b->position();
   if (pos < 0) pos = 0;
   c.setPosition(pos);
   delete b;
   setTextCursor(c);
   ensureCursorVisible();
}

void TextEditor::highlightLine(const QTextCursor &c, const QColor &color) {
      //const QTextCharFormat &f) {
   QTextCharFormat format(currentCharFormat());
   format.setBackground(color);
   
   const struct ExtraSelection newSelection = {
      QTextCursor(c), 
      format, //QTextCharFormat(f), 
   };
   
   m_highlighted.push_back(newSelection);
   setExtraSelections(m_highlighted);
}

void TextEditor::fontChanged(const QFont &f) {
   setCurrentFont(f);

   setTabStopWidth(4 * QFontMetrics(f).width(' '));
}

