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
#include "ErrorConsole.H"
#include "RegisterView.H"
#include "StackView.H"
#include "FindDialog.H"
#include "DirectoryListing.H"
#include "SyscallHandler.H"
#include "Program.H"
#include <QtGui>

Gui::Gui(QStringList args) : QMainWindow(), 
   m_syscallListener(new SyscallListener(this)), m_fileSaveAction(NULL), 
   m_fileSaveAllAction(NULL), m_debugRunAction(NULL), m_debugStepAction(NULL), 
   m_errors(NULL), m_editorPane(new EditorPane(this)), 
   m_lineNoPane(new LineNoPane(this, m_editorPane)), m_mode(STOPPED), 
   m_runningEditor(NULL), m_restarted(false)
{
   QApplication::setStyle(new QPlastiqueStyle());
   
   // load files
   args.removeFirst(); // first element is argv[0]
   foreach (QString fileName, args) {
      if (fileName == QString("-v"))
         continue; // disregard verbose flag

      if (QFile::exists(fileName))
         m_editorPane->openFile(fileName);
      else
         cerr << "Error loading file '" << fileName.toStdString() << "'\n";
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
   palette.setColor(QPalette::Highlight, QColor(157, 187, 227));//180, 201, 233));
   palette.setColor(QPalette::Window, QColor(239, 239, 239));
   qApp->setPalette(palette);
   m_originalFont = qApp->font();
//   m_guiFont = QFont("tahoma", 12);
//   qApp->setFont(m_guiFont);
   
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

   m_editFindAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editFind.png"), tr("&Find.."), this), m_editorPane, SLOT(find()), QKeySequence(tr("CTRL+F")), true);
   m_editReplaceAction = addAction(tb, menu, new QAction(/*QIcon(ICONS"/editReplace.png"), */tr("&Replace.."), this), m_editorPane, SLOT(findAndReplace()), QKeySequence(tr("CTRL+R")), true);  // TODO: find suitable editReplace icon
 
   menu->addSeparator();
   tb->addSeparator();
   
   m_editSelectAllAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editSelectAll.png"), tr("&Select All"), this), m_editorPane, SLOT(selectAll()), QKeySequence(tr("CTRL+A")), true);
   m_editToggleCommentAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editToggleComment.png"), tr("&Toggle Comment"), this), m_editorPane, SLOT(toggleComment()), QKeySequence(tr("CTRL+D")), true);
   m_editGotoLineAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editGotoLine.png"), tr("&Goto Line.."), this), m_editorPane, SLOT(gotoLine()), QKeySequence(tr("CTRL+G")), true);
   m_editGotoDeclarationAction = addAction(tb, menu, new QAction(QIcon(ICONS"/editGotoDeclaration.png"), tr("Goto &Declaration"), this), m_editorPane, SLOT(gotoDeclaration()), QKeySequence(tr("F3")), true);
   
   // Setup automatic enabling/disabling of different menu-items
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editUndoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editRedoAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editCutAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editPasteAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editSelectAllAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editToggleCommentAction, SLOT(setEnabled(bool)));
   //connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editReplaceAction, SLOT(setEnabled(bool)));
   connect(m_editorPane, SIGNAL(isModifiable(bool)), m_editorPane->m_findDialog, SLOT(setModifiable(bool)));
   
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
 
   m_debugBStepXAction = addAction(NULL, menu, new QAction(QIcon(ICONS"/debugBStepX.png"), tr("Back.."), this), this, SLOT(debugBStepXAction()), QKeySequence(), false);
   m_debugStepXAction = addAction(NULL, menu, new QAction(QIcon(ICONS"/debugStepX.png"), tr("Step.."), this), this, SLOT(debugStepXAction()), QKeySequence(), false);
   
   menu->addSeparator();
   tb->addSeparator();

   m_debugRunXSpimAction = addAction(tb, menu, new QAction(QIcon(ICONS"/debugRunXSpim.png"), tr("Open in xspim"), this), this, SLOT(debugRunXSpimAction()), QKeySequence(QString("CTRL+F9")));
}

void Gui::setupOptionsMenu() {
   QMenu *menu = menuBar()->addMenu(tr("&Options"));
   
   // TODO
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
   
   m_errors = new ErrorConsole(this, m_editorPane);
   m_viewErrorsAction  = m_errors->toggleViewAction();
   m_viewErrorsAction->setIcon(QIcon(ICONS"/viewErrors.png"));
   
   m_output = new OutputConsole(this, m_editorPane);
   m_viewOutputAction = m_output->toggleViewAction();
   m_viewOutputAction->setIcon(QIcon(ICONS"/viewOutput.png"));
   menu->addAction(m_viewOutputAction);
   menu->addAction(m_viewErrorsAction);
   
   m_registerView = new RegisterView(this, m_editorPane);
   m_viewRegistersAction = m_registerView->toggleViewAction();
   m_viewRegistersAction->setIcon(QIcon(ICONS"/viewRegisters.png"));
   menu->addAction(m_viewRegistersAction);
 
   m_stackView = new StackView(this, m_editorPane);
   m_viewStackAction = m_stackView->toggleViewAction();
   m_viewStackAction->setIcon(QIcon(ICONS"/viewStack.png"));
   menu->addAction(m_viewStackAction);

   m_directorylisting = new DirectoryListing(this, m_editorPane);
   m_viewDirectoryListingAction = m_directorylisting->toggleViewAction();
   m_viewDirectoryListingAction->setIcon(QIcon(ICONS"/viewDirectoryListing.png"));
   menu->addAction(m_viewDirectoryListingAction);

   addDockWidget(Qt::BottomDockWidgetArea, m_output);
   addDockWidget(Qt::BottomDockWidgetArea, m_errors);
   addDockWidget(Qt::RightDockWidgetArea, m_stackView);
   addDockWidget(Qt::RightDockWidgetArea, m_registerView);
   addDockWidget(Qt::LeftDockWidgetArea, m_directorylisting);
   tabifyDockWidget(m_output, m_errors);
}

void Gui::ensureVisibility(QDockWidget *widget) {
//   cerr << "in: " << widget->isVisible() << endl;
   if (widget == NULL)
      return; // possibly use etwas w/ widget's size?  if in a tab?
   
   setUpdatesEnabled(false);
   Qt::DockWidgetArea a = dockWidgetArea(widget);
   removeDockWidget(widget);
   addDockWidget(a, widget);
   if (!widget->isVisible())
      widget->toggleViewAction()->trigger();
   
   if (widget == m_errors)
      tabifyDockWidget(widget, m_output);
   else tabifyDockWidget(widget, m_errors);
   
   setUpdatesEnabled(true);
   //cerr << "out: " << widget->isVisible() << endl;
}

SyscallListener *Gui::getSyscallListener() const {
   return m_syscallListener;
}

RegisterView *Gui::getRegisterView() const {
   return m_registerView;
}

StackView *Gui::getStackView() const {
   return m_stackView;
}

ErrorConsole *Gui::getErrorConsole() const {
   return m_errors;
}

// Returns the currently active (running) programing for debugging purposes
TextEditor *Gui::getActiveProgram() const {
   return (m_runningEditor == NULL ? m_editorPane->m_activeEditor : m_runningEditor);
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
   "<span style=\"font-size: 17pt;font-family: arial;font-weight:bold\">"PROJECT_NAME"</span>"
   "<span style=\"font-size: 12pt;font-family: arial;\">"
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
      "<li><a href=\"mailto:tim@cs.brown.edu\">Tim O'Donnell</a> (tim)</li>"
   "</ul></span>"
"</html>";

void Gui::aboutMipScope() {
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
   m_restarted = false;

   if (m_mode == STOPPED) {
      /*if (m_editorPane->m_activeEditor->isModified()) {
         QMessageBox::StandardButton ret = m_editorPane->m_activeEditor->promptUnsavedChanges();
         
         if (ret == QMessageBox::Cancel)
            return;
      }*/
      
      m_syscallListener->reset();
      m_registerView->reset();
      m_runningEditor = m_editorPane->m_activeEditor;
   }
/*      m_mode = RUNNING;
   else if (m_mode == PAUSED)
      m_mode = RUNNING;
   else m_mode = PAUSED;*/
   
   run();
   //updateDebugActions();
}

void Gui::updateDebugActions() {
   if (m_debugStepAction == NULL)
      return;
   
   switch(m_mode) {
      case RUNNING:
         m_debugRunAction->setText(tr("&Pause"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugPause.png"));
         m_debugStopAction->setEnabled(true);
         m_debugRestartAction->setEnabled(true);
         m_debugStepAction->setEnabled(false);
         m_debugBStepAction->setEnabled(false);
         m_debugStepXAction->setEnabled(false);
         m_debugBStepXAction->setEnabled(false);
         if (m_runningEditor != NULL)
            m_runningEditor->setModifiable(false);
//         m_editorPane->setModifiable(false);
         
         break;
      case PAUSED:
         setUpdatesEnabled(false);
         
         m_debugRunAction->setText(tr("&Run"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugRun.png"));
         m_debugStopAction->setEnabled(true);
         m_debugRestartAction->setEnabled(true);
         m_debugStepAction->setEnabled(true);
         m_debugBStepAction->setEnabled(true);

         m_debugStepXAction->setEnabled(true);
         m_debugBStepXAction->setEnabled(true);

         // Allow for on-the-fly editing!
         if (m_runningEditor != NULL) {
            m_runningEditor->setModifiable(true);
            
            Program *program = m_runningEditor->getProgram();
            if (program != NULL) {
               bool undoIsAvailable = program->undoIsAvailable();
               
               m_debugBStepAction->setEnabled(undoIsAvailable);
               m_debugBStepXAction->setEnabled(undoIsAvailable);
            }
         }

         setUpdatesEnabled(true);
         break;
      case STOPPED:
         m_debugRunAction->setText(tr("&Run"));
         m_debugRunAction->setIcon(QIcon(ICONS"/debugRun.png"));
         m_debugStopAction->setEnabled(false);
         m_debugRestartAction->setEnabled(false);
         m_debugStepAction->setEnabled(false);
         m_debugBStepAction->setEnabled(false);
         m_debugStepXAction->setEnabled(false);
         m_debugBStepXAction->setEnabled(false);
         //m_editorPane->setModifiable(true);
         if (m_runningEditor != NULL)
            m_runningEditor->setModifiable(true);
         
      default:
         break;
   }
}

void Gui::debugStopAction() {
   m_restarted = false;
   stop();
   //m_mode = STOPPED;
//   updateDebugActions();
}

//void debugPauseAction(); // only have one run menuitem
// which switches automatically to 'pause' upon invocation.
void Gui::debugStepAction() {
   stepForward();
}

void Gui::debugBStepAction() {
   stepBackward();
}

void Gui::debugStepXAction() {
   bool okay = false;
   int max = 2147483647; // max signed int as defined by Qt
   
   int noInstructions = getNoInstructionsToStep(QString("Step Forward"), 
         QString("Number of instructions to skip:"), 1, max, &okay);
   
   if (okay)
      stepForward(noInstructions);
}

int Gui::getNoInstructionsToStep(const QString &title, const QString &text, int min, int max, bool *okay) {
   return QInputDialog::getInteger(this, title, text, 1, min, max, 1, okay);
}

void Gui::debugBStepXAction() {
   bool okay = false;
   if (m_runningEditor == NULL)
      return;

   int max = m_runningEditor->getProgram()->getCurrentTimestamp() - 1;
   
   int noInstructions = getNoInstructionsToStep(QString("Step Backward"), 
         QString("Number of instructions to roll back:"), 1, max, &okay);
   
   if (okay)
      stepBackward(noInstructions);
}


void Gui::debugRestartAction() {
   m_restarted = true;
   stop();
}

// Allow user to load file into a separate process running in xspim (cs31-version)
void Gui::debugRunXSpimAction() {
   QString fileName;
   bool load = true;
   
   if (m_editorPane->m_activeEditor->isModified()) {
      QMessageBox::StandardButton ret = m_editorPane->m_activeEditor->promptUnsavedChanges();

      if (ret == QMessageBox::Cancel)
         return;
      if (ret == QMessageBox::No) {
         load = false;
         
         QFile file(".temp.s");
         if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            cerr << PROJECT_NAME": error creating temporary file to load into xspim\n" << "Save your file before running to avoid this error.\n";
            cerr << file.errorString().toStdString() << endl;

            return;
         }

         QTextStream t(&file);
         t << m_editorPane->m_activeEditor->toPlainText();
         file.close();

         fileName = file.fileName();
      }
   }
   
   if (load) {
      QFile *file = m_editorPane->m_activeEditor->file();
      if (file == NULL)
         return;
      
      fileName = file->fileName();
   }
   
   if (fileName == "" || fileName.isEmpty())
      return;
   
   const char *path = "/course/cs031/pro/spim/xspim";
   switch(fork()) {
      case 0: // child
         {
         char *filename = fileName.toAscii().data();
         cerr << path << " -file " << filename << endl;

         execl(path, "xspim", "-file", filename, NULL);

         cerr << "Error launching: " << path << endl;
         exit(1);
         }
         break;
      case -1: // error
         cerr << "Error launching: " << path << endl;
         perror("fork()");
         break;
      default: // parent
         break;
   }
}

// emitted upon run, pause, or stop of Debugger
void Gui::programStatusChanged(int s) {
   m_mode = s;
   updateDebugActions();
   
   if (s != RUNNING) {
      m_registerView->updateDisplay();
      m_output->update();
   }
}

// returns true if the Gui wants control over handling program termination
bool Gui::handleProgramTerminated(int reason) {
   if (reason == T_TERMINATED && m_restarted) {
      m_restarted = false;
      debugRunAction();
      
      return true;
   }

   return false;
}

// emitted whenever the runnability of a program changes
void Gui::validityChanged(bool isValid) {
   if (m_debugStepXAction != NULL) {
      m_debugRunAction->setEnabled(isValid);
      m_debugStepAction->setEnabled(isValid);
      m_debugBStepAction->setEnabled(isValid);
      m_debugStepXAction->setEnabled(isValid);
      m_debugBStepXAction->setEnabled(isValid);
   }
}

// passes on to RegisterView
void Gui::registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc) {
   if (m_registerView != NULL)
      m_registerView->registerChanged(reg, value, status, pc);
   if (m_stackView != NULL && reg == sp)
      m_stackView->registerChanged(reg, value, status, pc);
}

// passes on to StackView
void Gui::memoryChanged(unsigned int address, unsigned int value, ParseNode *pc) {
   m_stackView->memoryChanged(address, value, pc);
}

void Gui::programUndoAvailabilityChanged(bool isAvailable) {
   m_debugBStepAction->setEnabled(isAvailable);
   m_debugBStepXAction->setEnabled(isAvailable);
}

void Gui::watchPointModified(unsigned int reg, bool watchPoint) {
   TextEditor *editor = m_runningEditor;
   if (editor == NULL)
      editor = m_editorPane->m_activeEditor;
   
   editor->getProgram()->watchPointModified(reg, watchPoint);
}

bool *Gui::getWatchpoints() const {
   return m_registerView->getWatchpoints();
}

void Gui::activeEditorChanged(TextEditor *current) {
   if (current == NULL)
      return;
   
   Program *program = current->getProgram();
   if (program == NULL)
      return;

   m_mode = program->getStatus();
   if (m_mode != STOPPED)
      m_runningEditor = current;

   updateDebugActions();
}

