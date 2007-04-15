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
#include <QtGui>
#include <QtCore>

TextEditor::TextEditor(EditorPane *parent, QFont *font, QFile *file, TextEditor *prev)
   : QTextEdit(), m_parent(parent), m_prev(prev), m_next(NULL), m_file(NULL), 
   m_loaded(false), m_modified(false), m_undoAvailable(false), 
   m_redoAvailable(false), m_syntaxHighligher(NULL)
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
   
   // strip formatting when pasting rich-text
   setAcceptRichText(false);
   setLineWrapMode(QTextEdit::NoWrap);
  
   m_syntaxTip = new SyntaxTip(this);

   setContextMenuPolicy(Qt::NoContextMenu); // TODO: add custom context menu?
   connect(this, SIGNAL(textChanged()), this, SLOT(codeChanged()));
   
   // signals for enabling/disabling editing functionality
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));

   connect(this, SIGNAL(undoAvailable(bool)), m_parent, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(redoAvailable(bool)), m_parent, SLOT(redoAvailabilityModified(bool)));
   connect(this, SIGNAL(copyAvailable(bool)), m_parent, SLOT(copyAvailabilityModified(bool)));
   connect(m_parent, SIGNAL(isModifiable(bool)), this, SLOT(modifiabilityChanged(bool)));
   connect(m_parent, SIGNAL(fontChanged(const QFont&)), this, SLOT(setCurrentFont(const QFont&)));
   
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
   bool match = false;
   
   // check if previously typed word was an instruction, immediately after the 
   // user completes it by typing the space bar (for syntax-help)
   if (key == Qt::Key_Space && !textCursor().hasSelection()) {
      QTextCursor c = textCursor();
      c.movePosition(QTextCursor::StartOfLine/*PreviousWord*/, QTextCursor::KeepAnchor);
      const QString &selectedText = c.selectedText();
      
      match = //m_parent->m_highlighter->matches(selectedText);
         m_syntaxHighligher->matches(selectedText);
      
      if (match) {
         // move cursor to start of matched word
         c = textCursor();
         c.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
         const QRect &rect = cursorRect(c);
//         const int top = rect.top();
         const QPoint pos(rect.center().x() + 2, rect.bottom() + 1);
               //top + ((rect.bottom() - top) >> 3));
         
         const QString text = "<p style='white-space:pre'><b>add</b> $dest, $src1, $src2";
         const QString statusText = "Adds two signed integers, storing the result in $dest.";

         m_syntaxTip->show(text, statusText, pos, c);
//         QToolTip::showText(mapToGlobal(pos), text, this);
      }
   } else if (key == Qt::Key_Return) {
//      QToolTip::hideText();
      m_syntaxTip->hide();
   } else if (key == Qt::Key_Escape) {
      m_syntaxTip->hide();
   } /*else if (key == Qt::Key_Tab) {
      if (m_lastTab->elapsed() > 200) {
         m_lastTab->restart();
         
         QTextCursor c = textCursor();
         insertPlainText("   ");
      }
      
      e->accept();
      return;
   }*/
   
//   cerr << textCursor().block().text().toStdString() << endl;
   
   cerr << currentLineNumber() << endl;
   
//   if (!match)
   QTextEdit::keyReleaseEvent(e);
}

bool TextEditor::openFile(QFile *file) {
   if (file->open(QIODevice::ReadOnly | QFile::Text)) {
         setPlainText(file->readAll());
         file->close();
         
         m_file = file;
         m_loaded = true;
         resetTabText();
   } // else display error
   
   return true;
}

// returns actual filename of open file
QString TextEditor::fileName() const {
   if (m_file == NULL)
      return QString();

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
   m_parent->setTabIcon(index, (m_modified ? QIcon(IMAGES"/fileUnsaved.png") : QIcon(IMAGES"/fileSaved.png")));
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
   setReadOnly(!isModifiable);
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

SyntaxTip::SyntaxTip(TextEditor *parent) : QLabel(parent), m_parent(parent) {
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

   disconnect(m_parent->document(), SIGNAL(cursorPositionChanged(const QTextCursor&)), 
         this, SLOT(cursorPositionChanged(const QTextCursor&)));
   disconnect(m_parent, SIGNAL(cursorPositionChanged()), this, SLOT(testCursorPos()));
}

void SyntaxTip::cursorPositionChanged(const QTextCursor &cursor) {
   hide();
}

void SyntaxTip::testCursorPos() {
   QTextCursor c = m_parent->textCursor();
   int pos = c.position();
   c.setPosition(m_startPos);
   c.movePosition(QTextCursor::Down);
   
   if (pos < m_startPos || pos >= c.position())
      hide();
}

void SyntaxTip::show(const QString &text, const QString &statusText, const QPoint &pos, const QTextCursor &c) {
   m_cursor = new QTextCursor(c);
   m_cursor->movePosition(QTextCursor::StartOfLine);
   m_startPos = m_cursor->position();
   
   setText(text);
   STATUS_BAR->showMessage(statusText);
   
  const QSize &hint = sizeHint();
   setGeometry(pos.x(), pos.y(), hint.width(), hint.height());
   
   connect(m_parent->document(), SIGNAL(cursorPositionChanged(const QTextCursor&)), 
         this, SLOT(cursorPositionChanged(const QTextCursor&)));
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


