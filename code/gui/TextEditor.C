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
#include <QFileDialog>
#include <QTextStream>
#include <QTextCursor>
#include <QKeyEvent>
#include <QFile>
#include <QFont>

TextEditor::TextEditor(EditorPane *parent, QFile *file, TextEditor *prev)
   : QTextEdit(), m_parent(parent), m_prev(prev), m_next(NULL), m_file(NULL), 
   m_loaded(false), m_syntaxHighligher(NULL)
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
   
   setContextMenuPolicy(Qt::NoContextMenu); // TODO:  temporary?
   connect(this, SIGNAL(textChanged()), this, SLOT(codeChanged()));
   
   m_syntaxHighligher = new SyntaxHighlighter(this);
}

// Slot, notified every time text is changed in the parent TextEdit
void TextEditor::codeChanged() {
//   cout << "codeChanged\n";
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
      
      match = m_syntaxHighligher->matches(selectedText);
   }
   
   if (match)
      cout << "space matched\n";
   else QTextEdit::keyReleaseEvent(e);
}

bool TextEditor::openFile(QFile *file) {
   if (file->open(QIODevice::ReadOnly | QFile::Text)) {
         setPlainText(file->readAll());
         file->close();

         m_loaded = true;
         m_file = file;
   } // else display error
   
   return true;
}

// returns actual filename of open file
QString TextEditor::fileName() {
   if (m_file == NULL)
      return QString();

   QString path = m_file->fileName();
   
   int index = path.lastIndexOf('/');
   if (index < 0)
      return path;

   return path.right(path.length() - index - 1);
}

void TextEditor::save() {
   if (!document()->isModified())
      return;

   if (m_file == NULL) {
      saveAs();
      return;
   }
   
   const QString &filename = fileName();
   QString text = toPlainText();
   if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text)) {
      //cerr << "Could not write to file: '" << filename << "'" << endl;
      if (STATUS_BAR != NULL)
         STATUS_BAR->showMessage( QString("Could not write to %1").arg(fileName()),
            2500 );
      
      return;
   }

   QTextStream t(m_file);
   t << text;
   m_file->close();
   
   m_loaded = true;
   document()->setModified(false);
   m_parent->setTabText(m_parent->indexOf(this), fileName());
   
   if (STATUS_BAR != NULL)
      STATUS_BAR->showMessage( QString( "File %1 saved" ).arg(filename), 2500 );
}

void TextEditor::saveAs() {
   const QString &filename = QFileDialog::getSaveFileName(this, tr("Save File As"), 
      /*m_lastDirectoryOpened*/QString(), FILE_FILTER);
   
   if (filename == "")
      return; // user selected cancel?

   m_file = new QFile(filename);
   save();
}

