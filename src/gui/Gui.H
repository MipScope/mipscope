/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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
#include <QHash>
#include "Utilities.H"
#include "SPIMSyscalls.H"
#include "UI.H"

#define SETTINGS_FILE      "."PROJECT_NAME".cfg"
#define MAX_RECENT_FILES   (5)

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
class MemoryView;

// TableView_dev
class TableMemView;

class TextEditor;
class DirectoryListing;
class StatementListing;
class SyscallListener;
class ParseNode;
class QFont;
class Program;
class State;
class OptionsDialog;
class Options;
class PluginHandler;

class Gui : public QMainWindow, public UI {
   
   Q_OBJECT
   
   public:
      Gui(QStringList args);
	   virtual ~Gui(void);
      
      void ensureVisibility(QDockWidget *widget);

      QAction *getSaveAction();
      QAction *getSaveAllAction();
      QMenu *getEditMenu();

      virtual SyscallListener *getSyscallListener();
      RegisterView *getRegisterView() const;
      StackView *getStackView() const;
      ErrorConsole *getErrorConsole() const;
      OutputConsole *getOutputConsole() const;
      TextEditor *getActiveProgram() const;
      StatementListing *getStatementListing() const;
   
      void watchPointModified(unsigned int reg, bool watchPoint);
      bool *getWatchpoints() const; // register watchpoints
      QHash<unsigned int, bool> *getMemoryWatchpoints() const;
      bool handleProgramTerminated(int reason);
      
      void activeEditorChanged(TextEditor *current);
      
      void addRecentFile(const QString &fileName);
      void updateRecentFileActions();
      QString strippedName(const QString &fullFileName);
      
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
      void gotoDeclaration(unsigned int address);
       
      // emitted upon run, pause, or stop of Debugger
      void programStatusChanged(int s);
      
      // emitted whenever the runnability of a program changes
      void validityChanged(bool isValid);
      // passes on to RegisterView
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode*);
      // passes on to StackView
      void memoryChanged(unsigned int address, unsigned int value, ParseNode*);
      
      void programUndoAvailabilityChanged(bool isAvailable);
      
      void updateMemory(Program*, State *state, int status);

      void stopCurrentProgram();      

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
      void fileOpenRecentFile();
      
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
      
      void showOptionsDialog();
      
   protected:
      Options *m_options;

      // Global listener for syscalls
      SyscallListener m_syscallListener;

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
              *m_viewMemoryAction, 
              *m_viewDirectoryListingAction, 
              *m_viewStatementListingAction, 
              *m_viewTableMemoryAction, 
              
              // Options-Related
              *m_optionsAction;
      
      // Central EditorPane and Dockable Widgets
      ErrorConsole *m_errors;
      EditorPane *m_editorPane;
      LineNoPane *m_lineNoPane;
      QStatusBar *m_statusBar;
      OutputConsole *m_output;
      RegisterView *m_registerView;
      StackView *m_stackView;
      MemoryView *m_memoryView; // requires OpenGL support to be enabled
      // TableView_dev
      TableMemView *m_tableMemoryView;
      
      DirectoryListing *m_directorylisting;
      StatementListing *m_statementListing;
      OptionsDialog *m_optionsDialog;
      
      QFont m_guiFont;
      QFont m_originalFont;
      //QStringList m_recentFiles;
      QAction *m_recentFileActs[MAX_RECENT_FILES];
      QAction *m_separatorAct;

      //@overridden
      void closeEvent(QCloseEvent *event);
      
      int getNoInstructionsToStep(const QString &title, const QString &text, int min, int max, bool *ok);
      
      // IMPORTANT:
      //    Editing, Debugging, or Paused
      int m_mode;
      // TextEditor which is currently running / being debugged
      TextEditor *m_runningEditor;
      bool m_restarted;
      
      SPIMSyscalls m_spim_syscalls;
      PluginHandler *m_pluginHandler;

   private:  // methods
      void setupGui();
      void setupPlugins();
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

      void updateMemoryView(Program *active);
};

#endif // __QGUI_H__

