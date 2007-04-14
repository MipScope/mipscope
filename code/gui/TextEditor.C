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

TextEditor::TextEditor(EditorPane *parent, QFile *file, TextEditor *prev)
   : QTextEdit(), m_parent(parent), m_prev(prev), m_next(NULL), m_file(NULL), 
   m_loaded(false), m_modified(false), m_undoAvailable(false), 
   m_redoAvailable(false), m_syntaxHighligher(NULL)
{
   setupEditor();
   
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

void TextEditor::setupEditor() {
   QFont font;
   font.setFamily("Courier");
   font.setFixedPitch(true);
   font.setPointSize(11);

   setFont(font);
   
   // strip formatting when pasting rich-text
   setAcceptRichText(false);
   setLineWrapMode(QTextEdit::NoWrap);
   
   setContextMenuPolicy(Qt::NoContextMenu); // TODO: add custom context menu?
   connect(this, SIGNAL(textChanged()), this, SLOT(codeChanged()));
   
   // signals for enabling/disabling editing functionality
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailabilityModified(bool)));

   connect(this, SIGNAL(undoAvailable(bool)), m_parent, SLOT(undoAvailabilityModified(bool)));
   connect(this, SIGNAL(redoAvailable(bool)), m_parent, SLOT(redoAvailabilityModified(bool)));
   connect(this, SIGNAL(copyAvailable(bool)), m_parent, SLOT(copyAvailabilityModified(bool)));
   connect(m_parent, SIGNAL(isModifiable(bool)), this, SLOT(modifiabilityChanged(bool)));
   
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
   bool match = false;
   
   // check if previously typed word was an instruction, immediately after the 
   // user completes it by typing the space bar (for syntax-help)
   if (e->key() == Qt::Key_Space) {
      QTextCursor c = textCursor();
      c.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
      const QString &selectedText = c.selectedText();
      
      match = //m_parent->m_highlighter->matches(selectedText);
         m_syntaxHighligher->matches(selectedText);
      
      if (match) {
         //QTextCursor c = textCursor();
         //int pos = c.position();
         const QRect &rect = cursorRect(c);
         const int top = rect.top();
         const QPoint pos(rect.center().x(), top + ((rect.bottom() - top) >> 3));
         
         QToolTip::showText(mapToGlobal(pos), "<p style='white-space:pre'><b>add</b> $dest, $src1, $src2", this);
         STATUS_BAR->showMessage(QString("Adds two signed integers, storing the result in $dest."));
      }
   } else if (e->key() == Qt::Key_Return) {
      QToolTip::hideText();
      STATUS_BAR->clearMessage();
   }
   
   if (!match)
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
   
   m_parent->setTabText(m_parent->indexOf(this), name + (m_modified ? "*" : ""));
}

bool Accepted(unsigned int result) {
   return (result == QMessageBox::Yes || result == QMessageBox::No || result == QMessageBox::NoAll || result == QMessageBox::YesAll);
}

// returns true upon successful closure
unsigned int TextEditor::close(bool promptForSave) {
   unsigned int ret = 0;
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
unsigned int TextEditor::promptUnsavedChanges(unsigned int extraButtons) {
   if (isModified()) {
      if (m_parent->m_activeEditor != this) {
         m_parent->setActiveEditor(this);
         m_parent->update();
      }

/*      QMessageBox prompt(QMessageBox::Warning, tr("Save changes?"), 
            QString("The file '") + fileName() + QString(" has been modified.\n\n"
               "Would you like to save your changes?"), 
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | 
            extraButtons, this);*/
      QString name = fileName();
      QString text;
      if (name == "")
         text = "This file has not been saved";
      else text = QString("The file '") + fileName() + QString("' has been modified");
      
      text += ".\n\nWould you like to save your changes?";
      QMessageBox prompt(tr("Save changes?"), text, QMessageBox::Warning, 
            QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel, this);
      extraButtons = 0;
//      if (extraButtons != 0)
//         prompt.add
      
      unsigned int ret = prompt.exec();
      
      if (ret == QMessageBox::Yes || ret == QMessageBox::YesAll) {
         unsigned int result = save();

         if (Accepted(result))
            return ret;
         
         return result;
      }
      
      return ret;
   }
   
   return QMessageBox::Yes;
}

// returns true if there are no pending changes after executing this method.
unsigned int TextEditor::save(bool forceSave) {
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

unsigned int TextEditor::saveAs() {
   const QString &filename = QFileDialog::getSaveFileName(this, tr("Save As"), 
      /*m_lastDirectoryOpened*/QString(), FILE_FILTER);
   
   if (filename == "")
      return QMessageBox::Cancel; // user selected cancel?
   
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

