/* ---------------------------------------------- *\
   file: Gui.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "Gui.H"
#include "EditorPane.H"
#include "LineNoPane.H"
#include "OutputConsole.H"
#include "RegisterView.H"
#include <QtGui>

Gui::Gui(int argc, char **argv) : QMainWindow(), m_fileSaveAction(NULL), 
   m_fileSaveAllAction(NULL), m_editorPane(new EditorPane(this)), 
   m_lineNoPane(new LineNoPane(this, m_editorPane)), m_mode(M_NORMAL)
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
   
   QPalette palette(qApp->palette());
   palette.setColor(QPalette::Highlight, QColor(180, 201, 233));
   qApp->setPalette(palette);
   
   setupActions();
   setCentralWidget(m_lineNoPane);
   setupStatusBar();
   resize(800, 720);
   
/*   QRect geom = m_output->geometry();
   geom.setSize(m_output->minimumSize());
   m_output->setGeometry(geom);*/

   loadSettings();
}

void Gui::setupActions() {
   // TODO: add more (standard) menu options
   setupFileActions();
   setupEditActions();
   setupDebugActions();
   setupDockWidgets();
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
   tb->setWindowTitle(tr("File"));
   tb->setObjectName(tr("File"));
   addToolBar(tb);
   
   m_fileMenu = menuBar()->addMenu(tr("&File"));
   QMenu *menu = m_fileMenu;
   
   addAction(tb, menu, new QAction(QIcon(ICONS"/fileNew.png"), tr("&New"), this), m_editorPane, SLOT(newBlankTabAction()), QKeySequence(QKeySequence::New));
   addAction(tb, menu, new QAction(QIcon(ICONS"/fileOpen.png"), tr("&Open"), this), this, SLOT(fileOpenAction()), QKeySequence::Open);
   
   menu->addSeparator();
   tb->addSeparator();

   m_fileSaveAction = addAction(tb, menu, getSaveAction(), m_editorPane, SLOT(saveAction()), QKeySequence(QKeySequence::Save), false);

   addAction(tb, menu, new QAction(QIcon(ICONS"/fileSaveAs.png"), tr("Save &As.."), this), m_editorPane, SLOT(saveAsAction()));
   m_fileSaveAllAction = addAction(tb, menu, getSaveAllAction(), this, SLOT(fileSaveAllAction()));
   
   menu->addSeparator();
   tb->addSeparator();

   addAction(tb, menu, new QAction(QIcon(ICONS"/filePrint.png"), tr("&Print.."), this), this, SLOT(filePrintAction()), QKeySequence(QKeySequence::Print));
   
   menu->addSeparator();
   tb->addSeparator();

   addAction(NULL, menu, new QAction(tr("&Exit"), this), this, SLOT(fileExitAction()), QKeySequence(QKeySequence::Close));
   
   connect(m_editorPane, SIGNAL(isModified(bool)), m_fileSaveAction, SLOT(setEnabled(bool)));
}

void Gui::setupEditActions() {
   QToolBar *tb = new QToolBar(this);
   tb->setWindowTitle(tr("Edit"));
   tb->setObjectName(tr("Edit"));
   addToolBar(tb);
   
   m_editMenu = menuBar()->addMenu(tr("&Edit"));
   QMenu *menu = m_editMenu;
   
   m_editUndoAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editUndo.png"), tr("&Undo"), this), m_editorPane, SLOT(undo()), QKeySequence(QKeySequence::Undo), false);
   m_editRedoAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editRedo.png"), tr("&Redo"), this), m_editorPane, SLOT(redo()), QKeySequence(QKeySequence::Redo), false);
   
   menu->addSeparator();
   tb->addSeparator();

   m_editCutAction   = addAction(tb, menu, new QAction(QIcon(ICONS"/editCut.png"), tr("Cu&t"), this), m_editorPane, SLOT(cut()), QKeySequence(QKeySequence::Cut), false);
   m_editCopyAction  = addAction(tb, menu, new QAction(QIcon(ICONS"/editCopy.png"), tr("&Copy"), this), m_editorPane, SLOT(copy()), QKeySequence(QKeySequence::Copy), false);
   m_editPasteAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editPaste.png"), tr("&Paste"), this), m_editorPane, SLOT(paste()), QKeySequence(QKeySequence::Paste), !QApplication::clipboard()->text().isEmpty());
   
   menu->addSeparator();
   tb->addSeparator();

   m_editSelectAllAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editSelectAll.png"), tr("&Select All"), this), m_editorPane, SLOT(selectAll()), QKeySequence(tr("CTRL+A")), true);
   m_editToggleCommentAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editToggleComment.png"), tr("&Toggle Comment"), this), m_editorPane, SLOT(toggleComment()), QKeySequence(tr("CTRL+D")), true);
   
   // Setup automatic enabling/disabling of different menu-items
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editUndoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editRedoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editCutAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editPasteAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editSelectAllAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editToggleCommentAction, SLOT(setEnabled(bool)));
   
   connect(m_editorPane, SIGNAL(undoAvailabile(bool)), m_editUndoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(redoAvailabile(bool)), m_editRedoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(copyAvailabile(bool)), m_editCutAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(copyAvailabile(bool)), m_editCopyAction, SLOT(setEnabled(bool)));
   connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardModified()));
}

QMenu *Gui::getEditMenu() {
   return m_editMenu;
}

void Gui::setupDebugActions() {
   QToolBar *tb = new QToolBar(this);
   tb->setWindowTitle(tr("Debugger"));
   tb->setObjectName(tr("Debugger"));
   addToolBar(tb);
   
   m_debugMenu = menuBar()->addMenu(tr("&Debug"));
   QMenu *menu = m_debugMenu;
   
   m_debugRunAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugRun.png"), tr("&Run"), this), this, SLOT(debugRunAction()), QKeySequence(QString("CTRL+F5")));
   m_debugStopAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugStop.png"), tr("S&top"), this), this, SLOT(debugStopAction()), QKeySequence(), false);
  
   m_debugRestartAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugRestart.png"), tr("R&estart"), this), this, SLOT(debugRestartAction()), QKeySequence(), false);
  
 //  m_debugPauseAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugPause.png"), tr("&Pause"), this), this, SLOT(debugPauseAction()));

   menu->addSeparator();
   tb->addSeparator();
   
   m_debugBStepAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugBStep.png"), tr("&Back"), this), this, SLOT(debugBStepAction()), QKeySequence(), false);
   m_debugStepAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugStep.png"), tr("&Step"), this), this, SLOT(debugStepAction()), QKeySequence(), false);
 
   
   // TODO!  Add appropriate signals/slots
   
   


   
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

void Gui::setupDockWidgets() {
   QMenu *menu = menuBar()->addMenu(tr("&View"));
   m_output = new OutputConsole(this, m_editorPane);
   
   addDockWidget(Qt::BottomDockWidgetArea, m_output);
   
   m_viewOutputAction = m_output->toggleViewAction();
   m_viewOutputAction->setIcon(QIcon(ICONS"/viewOutput.png"));
   menu->addAction(m_viewOutputAction);
   
   m_registerView = new RegisterView(this, m_editorPane);
   addDockWidget(Qt::RightDockWidgetArea, m_registerView);
   
   m_viewRegistersAction = m_registerView->toggleViewAction();
   m_viewRegistersAction->setIcon(QIcon(ICONS"/viewRegisters.png"));
   menu->addAction(m_viewRegistersAction);
   
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
   saveSettings();
   
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
   if (m_editorPane->closeAllTabs()) {
      saveSettings();

      event->accept();
   } else // User cancelled close event
      event->ignore();
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

void Gui::saveSettings() {
   QFile file(SETTINGS_FILE);

   if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      cerr << PROJECT_NAME": error saving workspace settings\n";
      cerr << file.errorString().toStdString() << endl;

      return;
   }

   QByteArray geoData = saveGeometry();
   QByteArray layoutData = saveState();

   bool ok = file.putChar((uchar)geoData.size());
   if (ok)
      ok = (file.write(geoData) == geoData.size());
   if (ok)
      ok = (file.write(layoutData) == layoutData.size());

   if (!ok) {
      cerr << PROJECT_NAME": error saving workspace settings\n";
      cerr << file.errorString().toStdString() << endl;
   }
   
   file.close();
}

void Gui::loadSettings() {
   QFile file(SETTINGS_FILE);
   
   if (!file.open(QIODevice::ReadOnly)) {
      //cerr << PROJECT_NAME": error loading workspace settings\n";
      //cerr << file.errorString().toStdString() << endl;
      
      return;
   }
   
   uchar geoSize;
   QByteArray geoData = saveGeometry();
   QByteArray layoutData = saveState();
   
   bool ok = file.getChar((char*)&geoSize);
   if (ok) {
      geoData = file.read(geoSize);
      ok = (geoData.size() == geoSize);
   }
   if (ok) {
      layoutData = file.readAll();
      ok = (layoutData.size() > 0);
   }

   if (ok)
      ok = restoreGeometry(geoData);
   if (ok)
     ok = restoreState(layoutData);
 
   if (!ok) {
      cerr << PROJECT_NAME": error saving workspace settings\n";
      cerr << file.errorString().toStdString() << endl;
   }

   file.close();
}

// ------------------
// DEBUGGER - RELATED
// ------------------
void Gui::debugRunAction() {
   if (m_mode == M_NORMAL)
      m_mode = M_RUNNING;
   else if (m_mode == M_PAUSED)
      m_mode = M_RUNNING;
   else m_mode = M_PAUSED;

   updateDebugActions();
}

void Gui::updateDebugActions() {
   switch(m_mode) {
      case M_RUNNING:
         m_debugRunAction->setText(tr("&Pause"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugPause.png"));
         m_debugStopAction->setEnabled(true);
         m_debugRestartAction->setEnabled(true);
         m_debugStepAction->setEnabled(false);
         m_debugBStepAction->setEnabled(false);
         
         break;
      case M_PAUSED:
         m_debugRunAction->setText(tr("&Run"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugRun.png"));
         m_debugStopAction->setEnabled(true);
         m_debugRestartAction->setEnabled(true);
         m_debugStepAction->setEnabled(true);
         m_debugBStepAction->setEnabled(true);
 
         break;
      case M_NORMAL:
         m_debugRunAction->setText(tr("&Run"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugRun.png"));
         m_debugStopAction->setEnabled(false);
         m_debugRestartAction->setEnabled(false);
         m_debugStepAction->setEnabled(false);
         m_debugBStepAction->setEnabled(false);
         
      default:
         break;
   }
}

void Gui::debugStopAction() {
   m_mode = M_NORMAL;

   updateDebugActions();
}

//void debugPauseAction(); // only have one run menuitem
// which switches automatically to 'pause' upon invocation.
void Gui::debugStepAction() {
   
   // TODO
}

void Gui::debugBStepAction() {
   
   // TODO
}

void Gui::debugRestartAction() {
   
   // TODO
   m_mode = M_RUNNING;
   updateDebugActions();
}


