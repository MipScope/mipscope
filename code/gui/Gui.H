/* ---------------------------------------------- *\
   file: Gui.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __QGUI_H__
#define __QGUI_H__
#include <QMainWindow>
#include <QStatusBar>
#include <QKeySequence>
#include "Utilities.H"

#define SETTINGS_FILE      "."PROJECT_NAME".cfg"

class QMenu;
class QAction;
class EditorPane;
class QCloseEvent;
class QAction;
class QToolBar;
class LineNoPane;
class OutputConsole;
class RegisterView;
class TextEditor;
class DirectoryListing;
class SyscallListener;

class Gui : public QMainWindow {
   
   Q_OBJECT
   
   public:
      Gui(int argc, char **argv);
	   virtual ~Gui(void);
      
      QAction *getSaveAction();
//      QAction *getSaveAsAction();
      QAction *getSaveAllAction();
      
      QMenu *getEditMenu();
      
      SyscallListener *getSyscallListener() const;
      RegisterView *getRegisterView() const;
   
      // Proxy-DEBUGGER Related
      // ----------------------
   signals:
      void stop();
      void pause();
      void run();
      void stepForward();
      void stepBackward();
      //void stepBackwardToTimestamp(TIMESTAMP stamp);
      //void jumpTo(const QTextBlock&);
      
   public slots:
      // emitted upon run, pause, or stop of Debugger
      void programStatusChanged(int s);
      
      // emitted whenever the runnability of a program changes
      void validityChanged(bool isValid);
      // passes on to RegisterView
      void registerChanged(unsigned int reg, unsigned int value, int status);
      
      
      // Normal Gui/Menu Related
      // -----------------------
   private slots:
      // MenuBar
      void fileOpenAction();
      void fileSaveAllAction();
      void filePrintAction();
      void fileExitAction();
      void aboutMipScope();
      void clipboardModified();
      
      // Debugger-Related
      void debugRunAction();
      void debugStopAction();
      //void debugPauseAction(); // only have one run menuitem
      // which switches automatically to 'pause' upon invocation.
      void debugStepAction();
      void debugBStepAction();
      void debugRestartAction();
      void debugRunXSpimAction();
      
      
   protected:
      // Global listener for syscalls
      SyscallListener *m_syscallListener;

      // MenuBar
      QMenu *m_fileMenu, *m_editMenu, *m_debugMenu;
      QString m_lastDirectoryOpened; // TODO
      QAction // File-Related
              *m_fileSaveAction, 
              *m_fileSaveAllAction, 
              
              // Editor-Related
              *m_editUndoAction, 
              *m_editRedoAction, 
              *m_editCutAction, 
              *m_editCopyAction, 
              *m_editPasteAction, 
              *m_editSelectAllAction, 
              *m_editToggleCommentAction, 
              *m_editGotoLineAction, 
              
              // Debugger-Related
              *m_debugRunAction, 
              *m_debugStopAction, 
              *m_debugStepAction, 
              *m_debugBStepAction, // Step backwards
              *m_debugRestartAction, 
              *m_debugRunXSpimAction, 
              
              // View-Related
              *m_viewOutputAction, 
              *m_viewRegistersAction, 
              *m_viewDirectoryListingAction;
      
      // Central EditorPane and Dockable Widgets
      EditorPane *m_editorPane;
      LineNoPane *m_lineNoPane;
      QStatusBar *m_statusBar;
      OutputConsole *m_output;
      RegisterView *m_registerView;
      DirectoryListing *m_directorylisting;
      
      //@overridden
      void closeEvent(QCloseEvent *event);
      
      // IMPORTANT:
      //    Editing, Debugging, or Paused
      int m_mode;
      // TextEditor which is currently running / being debugged
      TextEditor *m_runningEditor;
      
   private:  // methods
      void setupGui();
      void setupStatusBar();
      QAction *addAction(QToolBar *tb, QMenu *menu, QAction *a, const QObject *receiver, const char *member, const QKeySequence &key = QKeySequence(), bool isEnabled = true);

      void setupActions();
      void setupFileActions();
      void setupEditActions();
      void setupDebugActions();
      void setupOptionsMenu();
      void setupDockWidgets();
      
      void updateDebugActions();
      void saveSettings();
      void loadSettings();
};

#endif // __QGUI_H__

