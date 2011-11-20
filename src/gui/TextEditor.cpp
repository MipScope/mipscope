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
   file: TextEditor.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007

      The main source-code editor; displays PC 
   and syntactic/semantic errors during debugging.
\* ---------------------------------------------- */
#include "TextEditor.H"
#include "SyntaxHighlighter.H"
#include "EditorPane.H"
#include "Utilities.H"
#include "Gui.H"
#include "RegisterView.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Statement.H"
#include "../simulator/Parser.H"
#include "../simulator/Identifier.H"
#include "Program.H"
#include <QtGui>
#include <QtCore>

unsigned int TextEditor::m_untitledNo = 0;

TextEditor::TextEditor(EditorPane *parent, QFont *font, QFile *file, TextEditor *prev)
   : QTextEdit(), m_parent(parent), m_prev(prev), m_next(NULL), m_file(NULL), 
   m_loaded(false), m_modified(false), m_undoAvailable(false), 
   m_redoAvailable(false), m_syntaxHighligher(NULL), 
   m_program(new Program(parent->m_parent, parent, this)), m_pc(NULL), 
   m_hasControlSelection(false), m_registerTip(NULL)
{
   setupEditor(font);
   
   if (file != NULL)
      openFile(file);
   else openFile(NULL); // open a template file
   
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
   setTabStopWidth(Options::tabWidth() * QFontMetrics(*font).width(' '));
   
   // strip formatting when pasting rich-text
   setAcceptRichText(false);
   setLineWrapMode(QTextEdit::NoWrap);
  
   m_syntaxTip = new SyntaxTip(this);
   m_registerTip = new RegisterTip(this);

   QShortcut *jumpToPC = new QShortcut(QKeySequence(tr("CTRL+J")), this, SLOT(focusPC()), SLOT(focusPC()), Qt::WidgetShortcut);
   
   
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
   connect(this, SIGNAL(updateModifiability(bool)), m_parent, SLOT(modifiabilityChanged(bool)));
   connect(m_parent, SIGNAL(fontChanged(const QFont&)), this, SLOT(fontChanged(const QFont&)));
   connect(Options::m_options, SIGNAL(errorHighlightingEnabledChanged(bool)), this, SLOT(errorHighlightingEnabledChanged(bool)));
   
   //modifiabilityChanged(m_parent->isModifiable());
   m_syntaxHighligher = new SyntaxHighlighter(this);
   setMouseTracking(true);
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
   if (key == Qt::Key_Space && !textCursor().hasSelection() && Options::syntaxPopupsEnabled()) {
      QTextCursor c = textCursor();
      c.movePosition(QTextCursor::StartOfLine/*PreviousWord*/, QTextCursor::KeepAnchor);
      QString selectedText = c.selectedText().trimmed();
//      bool match = m_syntaxHighligher->matches(selectedText);
      bool match1 = (selectedText != "" && instructionMap.contains(selectedText)), match2 = false;
      
//      cerr << selectedText.toStdString() << ", " << match1 << ", " << match2 << endl;
//      cerr << instructionMap.contains(selectedText) << ", " << (void*)instructionMap[selectedText] << endl;
      
      if (!match1) {
         int index;
         selectedText = selectedText.right((index = selectedText.lastIndexOf(" ")) >= 0 ? selectedText.length() - index - 2 : selectedText.length() - 1);
//         cerr << "TEXT2: " << selectedText.toStdString() << ", " << directiveMap.contains(selectedText) << endl;

         if (directiveMap.contains(selectedText))
            match2 = true;
      }
      
      if (match1 || match2) {
         // move cursor to start of matched word
         c = textCursor();
         c.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
         const QRect &rect = cursorRect(c);
//         const int top = rect.top();
         const QPoint pos(rect.center().x() + 2, rect.bottom() + 1);
               //top + ((rect.bottom() - top) >> 3));
         
         const Statement *statement = (match1 ? (Statement*)instructionMap[selectedText] : (Statement*)directiveMap[selectedText]);
         if (statement != NULL) {
            const QString text = QString("<p style='white-space:pre'><b>%1</b> %2</style>").arg(selectedText, QString(statement->getSyntax()));

            QString statusText = statement->getDescription();
            //"Adds two signed integers, storing the result in $dest.";

            //cerr << text.toStdString() << endl;
            //cerr << statusText.toStdString() << endl;

            if (m_registerTip->isVisible() && m_registerTip->text() == text)
               m_registerTip->hide();
            
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
      if (!text.trimmed().isEmpty()) {
         if (text.trimmed().at(0) == QChar('#')) { // previous line is a comment
            int commentIndex = text.indexOf(QChar('#'));
            if (commentIndex >= 0) {
               const QString &whitespace = text.left(commentIndex + 1) + QString(" ");
               c.setPosition(pos);
               c.insertText(whitespace);
            }
         } else if (Options::autoIndentationEnabled()) { // auto-indent
            int nonWhiteSpaceInd = text.indexOf(QRegExp("[^ \t]"));
            if (nonWhiteSpaceInd == -1)
               nonWhiteSpaceInd = text.length();

            if (nonWhiteSpaceInd >= 0) {
               const QString &whitespace = text.left(nonWhiteSpaceInd);

               c.setPosition(pos);
               c.insertText(whitespace);
            }
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
      c.setPosition(start);
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
      
//      cerr << "COMMENT;done checking: " << inComment << ", end=" << end << ", start=" << start << endl;

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
            pos = c.position();
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

void TextEditor::pcChanged(ParseNode *pc, bool justRolledBack) {
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
      
      if (!justRolledBack && b != NULL) {
         QTextCursor c = textCursor();

         c.setPosition(b->position());
         setTextCursor(c);
         ensureCursorVisible();
      }
   }
   
   m_parent->updateLineNumbers(0);
   // the 2 can be changed according to preferences later
   // just be sure to also change the fading factors in paintEvent.
   m_program->getLastXInstructions(Options::noPreviousXInstructions(), m_lastInstructions);
   viewport()->update();
}

void TextEditor::focusPC() {
   if (m_program->getStatus() == PAUSED && m_pc != NULL) {
      QTextBlock *b = m_pc->getTextBlock();
      
      QTextCursor c = textCursor();
      c.setPosition(b->position());
      setTextCursor(c);
      ensureCursorVisible();
   }
}

void TextEditor::programStatusChanged(int status) {
//   cerr << "TextEditor::programStatusChanged(" << status << ")\n";
//   cerr << "\t" << QThread::currentThreadId() << "\n";
   
   if (status == PAUSED)
      pcChanged(m_program->getPC(), false);
   else pcChanged(NULL, false);
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
         
         if (!parseNode->isValid())
            continue;
         
         const QTextBlock *block = parseNode->getTextBlock();
         if (block != NULL && block->isValid() && parseNode != m_pc && cursorPos != block->position())
            highlightLine(p, parseNode, highlightColor, width, height);
      }
   }
   
   p.end();
   //setTabStopWidth(Options::tabWidth() * QFontMetrics(f).width(' '));
   
   QTextEdit::paintEvent(e);
}

void TextEditor::resizeEvent(QResizeEvent *e) {
   QTextEdit::resizeEvent(e);
   
   if (m_parent != NULL)
      m_parent->updateLineNumbers(0);
}

void TextEditor::highlightLine(QPainter &p, ParseNode *parseNode, const QColor &color, const int width, const int height) {
   const QTextBlock *block;
   if (parseNode == NULL || !parseNode->isValid() || ((block = parseNode->getTextBlock()) == NULL) || !block->isValid())
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
   bool templateFile = false;
   m_selections.clear();
   m_hasControlSelection = false;
   
   if (file == NULL) { // open a template file
      m_myUntitledNo = m_untitledNo++;

      if (!QFile::exists(Options::getTemplatePath()))
         return false;
      
      QString fileName = Options::getTemplatePath();
      //if (m_untitledNo > 0)
      //   fileName += QString::number(m_untitledNo++);
      
      file = new QFile(fileName);
      templateFile = true;
   }
   
   if (file->open(QIODevice::ReadOnly | QFile::Text)) {
//      setUpdatesEnabled(false);
      m_lineToPosMap.clear();
      
//      QTextDocument *m = new QTextDocument();
//      m->setPlainText(file->readAll());
//      setDocument(m);
      setPlainText(file->readAll());
      file->close();
      
      if (!templateFile) {
         m_file = file;
         m_loaded = true;
      } else fillInTemplate();
      
      resetTabText();
      
      qApp->processEvents();

      // hack to get this sheise to display right..
      QTimer::singleShot(250, m_parent, SLOT(contentChangedProxy()));
      QTimer::singleShot(750, m_parent, SLOT(contentChangedProxy()));
      QTimer::singleShot(1300, m_parent, SLOT(contentChangedProxy()));
      //modifiabilityChanged(m_parent->isModifiable());
      
//      setUpdatesEnabled(true);
   } // else display error
   
//   m_program->loadProgram();
   QTimer::singleShot(1500, m_program, SLOT(loadProgram()));
   return true;
}

// attempt to auto-insert the date into the built-in template
//   TODO:  fill-in login and name on unix; OS-specific or cs31-specific?
void TextEditor::fillInTemplate() {
   QRegExp r("\\bDATE:\\s+");

   QTextCursor cursor = document()->find(r);
   
   //cerr << cursor.isNull() << endl;
   if (!cursor.isNull()) {
      cursor.setPosition(cursor.selectionEnd());
      cursor.insertText(QDate::currentDate().toString());
      document()->setModified(false); // no user-made changes to be saved
   }
}

// returns actual filename of open file
QString TextEditor::fileName() const {
   if (m_file == NULL) {
      QString name = UNTITLED;

      if (m_myUntitledNo > 1)
         name += QString(" %1").arg(m_myUntitledNo);

      return name;
   }

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
   const QIcon &icon = (isModifiable() ? (m_modified ? QIcon(IMAGES"/fileUnsaved.png") : QIcon(IMAGES"/fileSaved.png")) : QIcon(IMAGES"/fileLocked.png"));

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
            STATUS_DELAY * 2 );
      
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
      STATUS_BAR->showMessage( QString( "File %1 saved" ).arg(filename), STATUS_DELAY + 1000 );

   return QMessageBox::Yes;
}

QMessageBox::StandardButton TextEditor::saveAs() {
   const QString &filename = QFileDialog::getSaveFileName(this, tr("Save As"), 
      /*m_lastDirectoryOpened*/QString(), FILE_FILTER);
   
   if (filename == "")
      return QMessageBox::Cancel; // user selected cancel

   m_parent->m_parent->addRecentFile(filename);
   m_file = new QFile(filename);
   return save(true); // force save even if document is unmodified
}

void TextEditor::mousePressEvent(QMouseEvent *e) {
   bool handled = false;
   
   // CTRL + click --> goto declaration of word under mouse
   if (e->modifiers() & Qt::ControlModifier) {
      QTextCursor c = cursorForPosition(e->pos());
      
      c.select(QTextCursor::WordUnderCursor);
      handled = gotoDeclaration(c);
   }

   if (!handled)
      QTextEdit::mousePressEvent(e);
}

void TextEditor::mouseMoveEvent(QMouseEvent *e) {
   bool wasShowing = m_hasControlSelection;
   bool shown = false;
   
   if (e->modifiers() & Qt::ControlModifier) {
      QTextCursor c = cursorForPosition(e->pos());
      
      c.select(QTextCursor::WordUnderCursor);
      const QTextCursor &cursor = findDeclaration(c);

      if (!cursor.isNull()) {
         m_hasControlSelection = true;
         QList<ExtraSelection> selections = QList<ExtraSelection>(m_selections);
         
         QTextCharFormat format(c.charFormat());
         format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
         format.setUnderlineColor(Qt::darkBlue);
         const struct QTextEdit::ExtraSelection newSelection = {
            QTextCursor(c), 
            QTextCharFormat(format), 
         };
         
         selections.push_back(newSelection);
         if (!wasShowing)
            qApp->setOverrideCursor(Qt::PointingHandCursor);
         
         shown = true; 
         setExtraSelections(selections);
      }
   }
   
   if (!shown && wasShowing) {
      m_hasControlSelection = false;
      qApp->restoreOverrideCursor();
      setExtraSelections(m_selections);
   }
   
   bool showingRegister = false;
   
   int status = m_program->getStatus();
   const QPoint &pos = e->pos();
   QTextCursor c = cursorForPosition(pos);
   c.select(QTextCursor::WordUnderCursor);
   QRect r = cursorRect(c);
   
   QTextCursor temp(c);
   int position = temp.selectionStart();//, endSelection = temp.selectionEnd();
/*   if (c.hasSelection()) {
      cerr << "Sel: " << position << " vs " << endSelection << endl;
   };*/
   
   temp.movePosition(QTextCursor::End);
   int lastPosition = temp.position();
/*   temp.clearSelection();
   temp.setPosition(endSelection);
   int blockNo = temp.blockNumber();*/
   temp.setPosition(position);

//   bool isValidSelection = false;
//   if (blockNo == temp.blockNumber()) { // ensure selection spans one line
//      cerr << blockNo << " vs " << temp.blockNumber() << endl;
      
      do {
         r = r.united(cursorRect(temp));

         if (++position >= lastPosition)
            break;
         temp.setPosition(position);
      } while(position < temp.selectionEnd());


      r.setX(r.x() - 25); // ensure mouse is actually close to word under cursor
      r.setY(r.y() - 25);
      r.setWidth(r.width() + 30);
      r.setHeight(r.height() + 30);
//      isValidSelection = (r.contains(pos));
//   }
   
   if (r.contains(pos)) {//isValidSelection)
      QString text = c.selectedText();
      
      // see if the user's hovering over a register
      if (status == PAUSED) {
         int registerNo = attemptRegisterParse(text);

         //      cerr << "found registerNo: " << registerNo << endl;
         if (registerNo >= 0 && registerNo < register_count) {
            showingRegister = true;
            m_registerTip->show(m_parent->m_parent->getRegisterView()->getRegisterText(registerNo), pos);
         } else { // check if user's hovering over a label in .data section and display address
            LabelMap *labelMap = m_program->getLabelMap();
            
            if (labelMap->contains(text)) {
               const QString tip = QString("<p style='white-space:pre'>Address: 0x<b>%1</b></style>").arg(QString::number(labelMap->value(text)->getValue(), 16));

               showingRegister = true;
               m_registerTip->show(tip, pos);
            }
         }
      }
      
      // see if the user's hovering over a instruction or directive and display its syntax
      if (status == STOPPED) {// || (status == PAUSED && showingRegister == false)) {
         bool match1 = (text != "" && instructionMap.contains(text)), match2 = false;
         
//         cerr << directiveMap.contains(text) << ", '" << text.toStdString() << "'\n";

         if (match1 || (match2 = directiveMap.contains(text))) {
            // move cursor to start of matched word
//            c = textCursor();
//            c.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
//            const QRect &rect = cursorRect(c);
            //         const int top = rect.top();
//            const QPoint pos(rect.center().x() + 2, rect.bottom() + 1);
            //top + ((rect.bottom() - top) >> 3));
            
            const Statement *statement = (match1 ? (Statement*)instructionMap[text] : (Statement*)directiveMap[text]);
            if (statement != NULL) {
               const QString tip = QString("<p style='white-space:pre'><b>%1</b> %2</style>").arg(text, QString(statement->getSyntax()));
               QString statusText = statement->getDescription();
               if (m_syntaxTip->isVisible() && m_syntaxTip->text() == tip)
                  m_syntaxTip->hide();
               
               //if (STATUS_BAR != NULL)
                  //STATUS_BAR->showMessage(statusText.replace("&lt;", "<").replace("&gt;", ">"), STATUS_DELAY * 2);
               
               showingRegister = true;
               m_registerTip->show(tip, pos);
            }
         }
      }
   }
   
   if (!showingRegister && m_registerTip->isVisible())
      m_registerTip->hide();
   
   QTextEdit::mouseMoveEvent(e);
}

QTextCursor TextEditor::findDeclaration(const QTextCursor &c) {
   return findDeclaration(c.selectedText());
}

QTextCursor TextEditor::findDeclaration(const QString &identifier) {
   ParseNode *declaration = m_program->getDeclaration(identifier);
   QTextBlock *declarationBlock;
   
   if (declaration != NULL && (declarationBlock = declaration->getTextBlock()) != NULL) {
      QTextCursor cursor = textCursor();

      int pos = declarationBlock->position() + declarationBlock->text().indexOf(identifier);

      if (pos >= 0) {
         // move cursor to line containing declaration
         cursor.setPosition(pos);

         // highlight declaration text
         cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, identifier.length());
         
         return QTextCursor(cursor);
      }
   }
   
   return QTextCursor();
   //QTextDocument *doc = document();
   //return doc->find(identifier, 0, QTextDocument::FindCaseSensitively);
}

bool TextEditor::gotoDeclaration(const QTextCursor &c) {
   return gotoDeclaration(c.selectedText());
}

bool TextEditor::gotoDeclaration(const QString &identifier) {
   QTextCursor cursor = findDeclaration(identifier);
   
   if (!cursor.isNull()) {
      setTextCursor(cursor);
      ensureCursorVisible();
      
      return true;
   }

   // display error indicator if we couldn't locate identifier declaration
   if (STATUS_BAR != NULL)
      STATUS_BAR->showMessage(QString("Could not find declaration of '%1'.").arg(identifier), STATUS_DELAY);
   
   return false;
}

void TextEditor::gotoNode(ParseNode *node) {
   if (node == NULL)
      return;

   AddressIdentifier *addr = node->getLabel();
   if (addr != NULL)
      gotoDeclaration(addr->getID());
   else gotoLine(lineNumber(*(node->getTextBlock())));
}

/*
   ParseNode *declaration = m_program->getDeclaration(identifier);
   
   if (declaration != NULL && (declarationBlock = declaration->getTextBlock()) != NULL) {
      QTextCursor cursor = textCursor();
      
      int pos = declarationBlock->position() + declarationBlock->text().indexOf(identifier);
      
      if (pos >= 0) {
         // move cursor to line containing declaration
         cursor.setPosition(pos);

         // highlight declaration text
         cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, identifier.length());
         setTextCursor(cursor);
         
         ensureCursorVisible();
         shown = true;
      }
   }
   

   return shown;
}*/

bool TextEditor::isModified() const {
   return document()->isModified();
}

void TextEditor::modifiabilityChanged(bool isModifiable) {
   setModifiable(isModifiable);
}

void TextEditor::setModifiable(bool isModifiable) {
   setReadOnly(!isModifiable);
   resetTabText(m_modified);

   updateModifiability(isModifiable);
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

void SyntaxTip::show(const QString &text, QString &statusText, const QPoint &pos, const QTextCursor &c) {
   m_block = new QTextBlock(c.block());

//   m_cursor = new QTextCursor(c);
//   m_cursor->movePosition(QTextCursor::StartOfLine);
//   m_startPos = m_cursor->position();
   
   setText(text);

   // status bar cannot handle rich-text, so convert left/right brackets beforehand
   STATUS_BAR->showMessage(statusText.replace("&lt;", "<").replace("&gt;", ">"));
   
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
	
	//bug fix: previous method caused invisible text bug
	QTextCursor c = QTextCursor(b);

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

void TextEditor::changeTextCursor(const QTextCursor &cursor) {
   if (!cursor.isNull() && cursor.position() >= 0) {
      setTextCursor(cursor);
      ensureCursorVisible();
   }
}

QTextDocument *TextEditor::getTextDocument() const {
   return document();
}

/*void TextEditor::highlightLine(const QTextCursor &c, const QColor &color) {
      //const QTextCharFormat &f) {
   QTextCharFormat format(currentCharFormat());
   format.setBackground(color);
   
   const struct ExtraSelection newSelection = {
      QTextCursor(c), 
      format, //QTextCharFormat(f), 
   };
   
   m_highlighted.push_back(newSelection);
   setExtraSelections(m_highlighted);
}*/

void TextEditor::fontChanged(const QFont &f) {
   QTextCharFormat format;
   format.setFont(f);
   QTextCursor c = textCursor();
   c.select(QTextCursor::Document);
   c.setCharFormat(format);
   setCurrentFont(f);

   setTabStopWidth(Options::tabWidth() * QFontMetrics(f).width(' '));
}

Program *TextEditor::getProgram() const {
   return m_program;
}

void TextEditor::errorHighlightingEnabledChanged(bool enabled) {
   if (!enabled)
      updateSyntaxErrors(NULL);
   else updateSyntaxErrors(m_program->getSyntaxErrors());
}

void TextEditor::updateSyntaxErrors(SyntaxErrors *errors) {
   if (errors == NULL || !Options::errorHighlightingEnabled()) { // program is free of errors
      if (!m_selections.isEmpty()) {
         m_selections.clear();
         setExtraSelections(m_selections);
      }

      return;
   }
   
//   cerr << "setting Errors: " << errors->size() << endl;
   
   m_selections.clear();
   QTextCursor c = textCursor();
   QTextCharFormat format(currentCharFormat());
   format.setUnderlineStyle(QTextCharFormat::SingleUnderline);//SpellCheckUnderline);//WaveUnderline);
   format.setUnderlineColor(Qt::red);
   
/*   // remove any invalid text blocks (probably were deleted)
   foreach(const QTextBlock &b, m_errors.keys()) {
      if (!b.isValid())
         m_errors.remove(b);
   }:*/
   
   foreach(ParseError e, *errors) {
      const QTextBlock *block = e.getTextBlock();
      
      if (block != NULL && block->isValid()) {
         int subPos = e.getPosition();
         
         if (subPos >= 0) {
            c.setPosition(block->position() + subPos);
            c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, e.getLength());
            
            //c.setCharFormat(format);
         } else {
            c.setPosition(block->position());
            c.select(QTextCursor::LineUnderCursor);
            
            //c.setBlockCharFormat(format);
         }
         
//         format.setToolTip(QString("<b>Error</b>:  %1").arg(e));
         const struct QTextEdit::ExtraSelection newSelection = {
            QTextCursor(c), 
            QTextCharFormat(format), 
         };
         
         m_selections.push_back(newSelection);
         
         /*const QTextBlock &newBlock = c.block();
         if (m_errors.contains(newBlock))
            m_errors.remove(newBlock);
         
         m_errors.insert(newBlock, newSelection);*/
      }
   }
   
   setExtraSelections(m_selections);//m_errors.values());
}


void TextEditor::testMouseMove(QMouseEvent *e, const QPoint &pos) {
//   cerr << "TODO testMouseMove\n";
}

void TextEditor::leaveEvent(QEvent *e) {
   if (m_registerTip != NULL && m_registerTip->isVisible())
      m_registerTip->hide();
}


// -----------
// RegisterTip
// -----------

RegisterTip::RegisterTip(TextEditor *parent) 
   : QLabel(parent), m_parent(parent), 
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

void RegisterTip::mousePressEvent(QMouseEvent *e) {
   this->hide();
}

void RegisterTip::hide() {
   if (!isVisible())
      return;
   
   if (m_state == S_Fading || m_state == S_Hidden)
      return;
   
   m_state = S_Fading;
   m_alpha = 255;
   m_timer->start(EXTENDED_FADE_INTERVAL);
}

void RegisterTip::show(const QString &text, const QPoint &pos)
{
   setText(text);
   
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
   //connect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));
}

void RegisterTip::mouseMoveEvent(QMouseEvent *e) {
   e->ignore();
   m_parent->testMouseMove(e, mapTo(m_parent, e->pos()));
}

// @overridden
void RegisterTip::paintEvent(QPaintEvent *e) {
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

int TextEditor::attemptRegisterParse(QString &text) {
   if (!Parser::simplify(text))
      return -1;
   
   int regIndex = text.indexOf('$');
   int len = text.length();

   if (regIndex < 0)
      return -1;

   if (regIndex + 1 >= len)
      return -1;

   QString reg;
   QRegExp endOfReg("\\$[a-z0-9]+\\b");
   int endIndex = text.indexOf(endOfReg, regIndex);

   if (endIndex + endOfReg.matchedLength() > regIndex + 3)
      return -1;

   if (regIndex + 2 >= len || (endIndex > 0 && endIndex + endOfReg.matchedLength() == regIndex + 2 && endIndex == regIndex))
      reg = Parser::substring(text, regIndex + 0, 2);
   else reg = Parser::substring(text, regIndex + 0, 3);

/*   if (VERBOSE) cerr << _tab << "Len: " << len << ", regIndex = " << regIndex << ", endIndex = " << endIndex << ", matched = " << endOfReg.matchedLength() << endl;
   if (VERBOSE) cerr << _tab << "Parsing register: '" << text.toStdString() << "'  reg: " << reg.toStdString() << endl;*/

   int registerNo = -1;
   // determine which $r register
   if (reg.at(1).isDigit()) {
      reg = Parser::substring(text, 1, reg.length() - 1);  //2, reg.length() - 2);  (old, for $r3 type syntax -- which doesn't exist)

      bool okay = false;
      registerNo = reg.toInt(&okay, 10);
      if (!okay || registerNo < zero || registerNo >= pc)
         return -1;
   } else if (reg == "$fp") { // special alias - not in the registerAliases table
      registerNo = 30;
   } else { // determine between aliases
      bool found = false;

      for(int i = 0; i < pc ; i++) {
         QRegExp r(QString("\\$") + QString(registerAliases[i]));

         if (r.exactMatch(reg)) {
            found = true;
            registerNo = i;
            break;
         }
      }

      if (!found)
         return -1;
   }

   // at this point, should know register number
//   if (VERBOSE) cerr << _tab << "Register: " << registerNo << " \t $" << registerAliases[registerNo] << "\n";

   return registerNo;
}

