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

#define FILE_FILTER        tr("MIPS Files (*.s *.a)")
#define STATUS_DELAY       (2200)
extern QStatusBar *STATUS_BAR;

class QMenu;
class EditorPane;
class QCloseEvent;

class Gui : public QMainWindow {
   
   Q_OBJECT
   
   public:
      Gui(int argc, char **argv);
	   virtual ~Gui(void);
      
   signals:
      
      
   private slots:
      // MenuBar
      void fileOpenAction();
      void fileSaveAllAction();
      void fileExitAction();
      
      
   protected:
      // MenuBar
      QMenu *m_fileMenu;
      QString m_lastDirectoryOpened; // TODO

      // Central EditorPane
      EditorPane *m_editorPane;
      QStatusBar *m_statusBar;

      //@overridden
      void closeEvent(QCloseEvent *event);
      
   private:  // methods
      void setupGui();
      void setupMenuBar();
      void setupStatusBar();
};

#endif // __QGUI_H__

