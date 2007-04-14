/* ---------------------------------------------- *\
   file: Gui.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "Gui.H"
#include "EditorPane.H"
#include "Utilities.H"
#include <QtGui>
#include <string.h>

Gui::Gui(int argc, char **argv) : QMainWindow(), m_fileSaveAction(NULL), 
   m_fileSaveAllAction(NULL), m_editorPane(new EditorPane(this))
{
   QApplication::setStyle(new QPlastiqueStyle());
   
   if (argc > 1) {
      int i = 1;
      
      // load multiple files via the command-line
      while(i < argc) {
         if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-file")) {
            QString fileName(argv[++i]);

            if (!QFile::exists(fileName))
               cerr << "Error loading file '" << argv[i] << "'\n";
            else m_editorPane->openFile(fileName);
         }

         ++i;
      }
   }
   
   setupGui();
   show();
}

Gui::~Gui(void) { }

void Gui::setupGui() {
   setWindowTitle(tr("Spim sucks"));
   setWindowIcon(QIcon(IMAGES"/logo.png"));
   setIconSize(QSize(ICON_SIZE, ICON_SIZE));
   
   setupActions();
   setCentralWidget(m_editorPane);
   setupStatusBar();

   resize(640, 640);
}

void Gui::setupActions() {
   // TODO: add more (standard) menu options
   setupFileActions();
   setupEditActions();
   setupOptionsMenu();

   {
      QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
      helpMenu->addAction(tr("About &"PROJECT_NAME), this, SLOT(aboutMipScope()));
      helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
   }
}

QAction *Gui::addAction(QToolBar *tb, QMenu *menu, QAction *a, const QObject *receiver, const char *member, const QKeySequence &key, bool isEnabled) {
   if (!key.isEmpty())
      a->setShortcut(key);
   
   a->setEnabled(isEnabled);
   connect(a, SIGNAL(triggered()), receiver, member);

   if (tb != NULL)
      tb->addAction(a);
   if (menu != NULL)
      menu->addAction(a);

   return a;
}

void Gui::setupFileActions() {
   QToolBar *tb = new QToolBar(this);
   tb->setWindowTitle(tr("File Actions"));
   addToolBar(tb);
   
   m_fileMenu = menuBar()->addMenu(tr("&File"));
   QMenu *menu = m_fileMenu;
   
   addAction(tb, menu, new QAction(QIcon(ICONS"/fileNew.png"), tr("&New"), this), m_editorPane, SLOT(newBlankTabAction()), QKeySequence(QKeySequence::New));
   addAction(tb, menu, new QAction(QIcon(ICONS"/fileOpen.png"), tr("&Open"), this), this, SLOT(fileOpenAction()), QKeySequence::Open);
   
   menu->addSeparator();

   m_fileSaveAction = addAction(tb, menu, getSaveAction(), m_editorPane, SLOT(saveAction()), QKeySequence(QKeySequence::Save), false);

   addAction(tb, menu, new QAction(QIcon(ICONS"/fileSaveAs.png"), tr("Save &As.."), this), m_editorPane, SLOT(saveAsAction()));
   m_fileSaveAllAction = addAction(tb, menu, getSaveAllAction(), this, SLOT(fileSaveAllAction()));
   
   menu->addSeparator();

   addAction(tb, menu, new QAction(QIcon(ICONS"/filePrint.png"), tr("&Print.."), this), this, SLOT(filePrintAction()), QKeySequence(QKeySequence::Print));
   
   menu->addSeparator();

   addAction(NULL, menu, new QAction(tr("&Exit"), this), this, SLOT(fileExitAction()), QKeySequence(QKeySequence::Close));
   
   connect(m_editorPane, SIGNAL(isModified(bool)), m_fileSaveAction, SLOT(setEnabled(bool)));
}

void Gui::setupEditActions() {
   QToolBar *tb = new QToolBar(this);
   tb->setWindowTitle(tr("Edit Actions"));
   addToolBar(tb);
   
   m_editMenu = menuBar()->addMenu(tr("&Edit"));
   QMenu *menu = m_editMenu;
   
   m_editUndoAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editUndo.png"), tr("&Undo"), this), m_editorPane, SLOT(undo()), QKeySequence(QKeySequence::Undo), false);
   m_editRedoAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editRedo.png"), tr("&Redo"), this), m_editorPane, SLOT(redo()), QKeySequence(QKeySequence::Redo), false);
   
   menu->addSeparator();

   m_editCutAction   = addAction(tb, menu, new QAction(QIcon(ICONS"/editCut.png"), tr("Cu&t"), this), m_editorPane, SLOT(cut()), QKeySequence(QKeySequence::Cut), false);
   m_editCopyAction  = addAction(tb, menu, new QAction(QIcon(ICONS"/editCopy.png"), tr("&Copy"), this), m_editorPane, SLOT(copy()), QKeySequence(QKeySequence::Copy), false);
   m_editPasteAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editPaste.png"), tr("&Paste"), this), m_editorPane, SLOT(paste()), QKeySequence(QKeySequence::Paste), !QApplication::clipboard()->text().isEmpty());
   
   // Setup automatic enabling/disabling of different menu-items
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editUndoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editRedoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editCutAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editPasteAction, SLOT(setEnabled(bool)));
   
   connect(m_editorPane, SIGNAL(undoAvailabile(bool)), m_editUndoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(redoAvailabile(bool)), m_editRedoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(copyAvailabile(bool)), m_editCutAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(copyAvailabile(bool)), m_editCopyAction, SLOT(setEnabled(bool)));
   connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardModified()));
}

void Gui::setupOptionsMenu() {
   QMenu *menu = menuBar()->addMenu(tr("&Options"));
   
   menu->addAction(new QAction(tr("&TODO"), this));
}

QStatusBar *STATUS_BAR;
void Gui::setupStatusBar() {
   m_statusBar = statusBar();
   STATUS_BAR = m_statusBar;

   m_statusBar->showMessage(tr("Welcome to "PROJECT_NAME), STATUS_DELAY);
}

// -----------------------
// MenuBar-related Actions
// -----------------------

void Gui::fileOpenAction() {
   const QString &fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 
      m_lastDirectoryOpened, FILE_FILTER);

   if (fileName.isEmpty())
      return;

   m_editorPane->openFile(fileName);
}

void Gui::fileExitAction() {
   if (m_editorPane->closeAllTabs())
      qApp->quit();
}

void Gui::fileSaveAllAction() {
   m_editorPane->saveAllFiles(true); // do not prompt user
}

void Gui::filePrintAction() {
   QPrinter printer;

   QPrintDialog dlg(&printer, this);
   if (m_editorPane->m_activeEditor->textCursor().hasSelection())
      dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
   
   if (dlg.exec() != QDialog::Accepted)
      return;

   m_editorPane->m_activeEditor->print(&printer);
}

//@overridden
void Gui::closeEvent(QCloseEvent *event) {
   if (m_editorPane->closeAllTabs())
      event->accept();
   else event->ignore();
}

const char *ABOUT_TEXT = 
"<html>"
   "<span style=\"font-size:17pt;font-weight:bold\">"PROJECT_NAME"</span>"
   "<p>"PROJECT_NAME" is a full-featured <c>MIPS</c> development environment aimed at students who are learning assembly for the first time.</p>"
   "<p>In addition to common features found in traditional IDEs, "PROJECT_NAME" includes a visual, reversible debugger which allows the user to edit code during a program run and see the aggregate results <b>without re-running their program</b>.</p>"
   "<br>"
   "<br>"
   "Pre-Pre-Pre Alpha version<br>"
   "April 2007"//__DATE__
   "<br>"
   "<br>"
   "Written by:"
   "<ul>"
      "<li><a href=\"mailto:fisch0920@gmail.com\">Travis Fischer</a> (tfischer)</li>"
      "<li><a href=\"mailto:timodonnell@gmail.com\">Tim O'Donnell</a> (tim)</li>"
   "</ul>"
"</html>";

void Gui::aboutMipScope() {
   // TODO:  Make this better..
   QMessageBox::about(this, tr("About "PROJECT_NAME), tr(ABOUT_TEXT));
}

QAction *Gui::getSaveAction() {
   if (m_fileSaveAction == NULL)
      m_fileSaveAction = new QAction(QIcon(ICONS"/fileSave.png"), tr("&Save"), this);
   
   return m_fileSaveAction;
}

QAction *Gui::getSaveAllAction() {
   if (m_fileSaveAllAction == NULL)
      m_fileSaveAllAction = new QAction(QIcon(ICONS"/fileSaveAll.png"), tr("Save A&ll"), this);
   
   return m_fileSaveAllAction;
}

void Gui::clipboardModified() {
   bool enabled = (m_editorPane->isModifiable() && 
         !QApplication::clipboard()->text().isEmpty());

   m_editPasteAction->setEnabled(enabled);
}

