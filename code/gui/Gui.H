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

#define FILE_FILTER        tr("MIPS Files (*.s *.a);;All Files (*)")
#define STATUS_DELAY       (2500)
#define IMAGES             "images/"

extern QStatusBar *STATUS_BAR;

class QMenu;
class QAction;
class EditorPane;
class QCloseEvent;
class QAction;
class QToolBar;

class Gui : public QMainWindow {
   
   Q_OBJECT
   
   public:
      Gui(int argc, char **argv);
	   virtual ~Gui(void);
      
      QAction *getSaveAction();
      
   //signals:
      
      
   private slots:
      // MenuBar
      void fileOpenAction();
      void fileSaveAllAction();
      void filePrintAction();
      void fileExitAction();
      void aboutMipScope();
      void clipboardModified();
      
   protected:
      // MenuBar
      QMenu *m_fileMenu, *m_editMenu;
      QString m_lastDirectoryOpened; // TODO
      QAction *m_fileSaveAction, 
              *m_editUndoAction, 
              *m_editRedoAction, 
              *m_editCutAction, 
              *m_editCopyAction, 
              *m_editPasteAction;
      
      // Central EditorPane
      EditorPane *m_editorPane;
      QStatusBar *m_statusBar;
      
      //@overridden
      void closeEvent(QCloseEvent *event);
      
   private:  // methods
      void setupGui();
      void setupStatusBar();
      QAction *addAction(QToolBar *tb, QMenu *menu, QAction *a, const QObject *receiver, const char *member, const QKeySequence &key = QKeySequence(), bool isEnabled = true);

      void setupActions();
      void setupFileActions();
      void setupEditActions();
      void setupOptionsMenu();
};

#endif // __QGUI_H__

