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
class QStringList;
class EditorPane;
class QCloseEvent;
class QAction;
class QToolBar;
class LineNoPane;
class OutputConsole;
class ErrorConsole;
class RegisterView;
class StackView;
class TextEditor;
class DirectoryListing;
class SyscallListener;
class ParseNode;
class QFont;

class Gui : public QMainWindow {
   
   Q_OBJECT
   
   public:
      Gui(QStringList args);
	   virtual ~Gui(void);
      
      void ensureVisibility(QDockWidget *widget);

      QAction *getSaveAction();
      QAction *getSaveAllAction();
      QMenu *getEditMenu();

      SyscallListener *getSyscallListener() const;
      RegisterView *getRegisterView() const;
      StackView *getStackView() const;
      ErrorConsole *getErrorConsole() const;
      TextEditor *getActiveProgram() const;
   
      void watchPointModified(unsigned int reg, bool watchPoint);
      bool *getWatchpoints() const;
      bool handleProgramTerminated(int reason);
      
      void activeEditorChanged(TextEditor *current);
      
      // Proxy-DEBUGGER Related
      // ----------------------
   signals:
      void stop();
      void pause();
      void run();
      void stepForward();
      void stepBackward();
      void stepForward(int noInstructions);
      void stepBackward(int noInstructions);
      //void stepBackwardToTimestamp(TIMESTAMP stamp);
      //void jumpTo(const QTextBlock&);
      
   public slots:
      // emitted upon run, pause, or stop of Debugger
      void programStatusChanged(int s);
      
      // emitted whenever the runnability of a program changes
      void validityChanged(bool isValid);
      // passes on to RegisterView
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode*);
      // passes on to StackView
      void memoryChanged(unsigned int address, unsigned int value, ParseNode*);
      
      void programUndoAvailabilityChanged(bool isAvailable);
      
      
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
      void debugStepXAction();
      void debugBStepXAction();
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
              *m_editGotoDeclarationAction, 
              *m_editFindAction, 
              *m_editReplaceAction, 
              
              // Debugger-Related
              *m_debugRunAction, 
              *m_debugStopAction, 
              *m_debugStepAction, 
              *m_debugBStepAction, // Step backwards
              *m_debugStepXAction, // Step forwards x number of instructions
              *m_debugBStepXAction, // Step backwards x number of instructions
              *m_debugRestartAction, 
              *m_debugRunXSpimAction, 
              
              // View-Related
              *m_viewOutputAction, 
              *m_viewErrorsAction, 
              *m_viewRegistersAction, 
              *m_viewStackAction, 
              *m_viewDirectoryListingAction;
      
      // Central EditorPane and Dockable Widgets
      ErrorConsole *m_errors;
      EditorPane *m_editorPane;
      LineNoPane *m_lineNoPane;
      QStatusBar *m_statusBar;
      OutputConsole *m_output;
      RegisterView *m_registerView;
      StackView *m_stackView;
      DirectoryListing *m_directorylisting;
      
      QFont m_guiFont;
      QFont m_originalFont;
      
      //@overridden
      void closeEvent(QCloseEvent *event);
      
      int getNoInstructionsToStep(const QString &title, const QString &text, int min, int max, bool *ok);
      
      // IMPORTANT:
      //    Editing, Debugging, or Paused
      int m_mode;
      // TextEditor which is currently running / being debugged
      TextEditor *m_runningEditor;
      bool m_restarted;
      
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

