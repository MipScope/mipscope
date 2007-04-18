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

class QMenu;
class QAction;
class EditorPane;
class QCloseEvent;
class QAction;
class QToolBar;
class LineNoPane;
class OutputConsole;

class Gui : public QMainWindow {
   
   Q_OBJECT
   
   public:
      Gui(int argc, char **argv);
	   virtual ~Gui(void);
      
      QAction *getSaveAction();
//      QAction *getSaveAsAction();
      QAction *getSaveAllAction();
      
      QMenu *getEditMenu();
      
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

      
   protected:
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
              
              // Debugger-Related
              *m_debugRunAction, 
              *m_debugStopAction, 
              *m_debugStepAction, 
              *m_debugBStepAction, // Step backwards
              *m_debugRestartAction, 
              
              // View-Related
              *m_viewOutputAction;
      
      // Central EditorPane
      EditorPane *m_editorPane;
      LineNoPane *m_lineNoPane;
      QStatusBar *m_statusBar;
      OutputConsole *m_output;
      
      //@overridden
      void closeEvent(QCloseEvent *event);
      
      // IMPORTANT:
      //    Editing, Debugging, or Paused
      DEBUG_MODE m_mode;

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
};

#endif // __QGUI_H__

