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

Gui::Gui(int argc, char **argv) : QMainWindow(), m_editorPane(new EditorPane(this))
{
   if (argc > 1) {
      int i = 1;
      
      // load multiple files via the command-line
      while(i < argc) {
         if (!strcmp(argv[i++], "-f")) {
            QString fileName(argv[i]);

            if (!QFile::exists(fileName))
               cerr << "Error loading file '" << argv[i] << "'\n";
            else m_editorPane->openFile(fileName);
            ++i;
         }
      }
   }
   
   setupGui();
   show();
}

Gui::~Gui(void) { }

void Gui::setupGui() {
   setWindowTitle(tr("Spim sucks"));
   
   setupMenuBar();
   setCentralWidget(m_editorPane);
   setupStatusBar();

   resize(640, 640);
}

void Gui::setupMenuBar() {
   m_fileMenu = menuBar()->addMenu(tr("&File"));

   QAction *action = new QAction(tr("&Open"), this);
   connect(action, SIGNAL(triggered()), this, SLOT(fileOpenAction()));
   action->setShortcut(tr("Ctrl+O"));
   action->setStatusTip(tr("Open an existing source file"));
   m_fileMenu->addAction(action);
   
   m_fileMenu->addAction("&Save", this, SLOT(fileSaveAction()), tr("Ctrl+S"));
   m_fileMenu->addAction("&Save As..", this, SLOT(fileSaveAsAction()));
   m_fileMenu->addAction("&Save All", this, SLOT(fileSaveAllAction()));

   m_fileMenu->addAction("&Exit", this, SLOT(fileExitAction()));
   
   // TODO: add more (standard) menu options
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

void Gui::fileSaveAction() {
   bool modified = m_editorPane->m_activeEditor->isModified();
   unsigned int ret = m_editorPane->m_activeEditor->save();

   if (modified && ret & Accepted) {
      m_statusBar->showMessage(QString("File ") + m_editorPane->m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void Gui::fileSaveAsAction() {
   unsigned int ret = m_editorPane->m_activeEditor->saveAs();

   if (ret & Accepted) {
      m_statusBar->showMessage(QString("File ") + m_editorPane->m_activeEditor->fileName() + 
            QString(" saved successfully."), STATUS_DELAY);
   }
}

void Gui::fileExitAction() {
   if (m_editorPane->closeAllTabs())
      qApp->quit();
}

void Gui::fileSaveAllAction() {
   m_editorPane->saveAllFiles(true); // do not prompt user
}

//@overridden
void Gui::closeEvent(QCloseEvent *event) {
   if (m_editorPane->closeAllTabs())
      event->accept();
   else event->ignore();
}

