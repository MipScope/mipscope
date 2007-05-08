/* ---------------------------------------------- *\
   file: Console.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Utilities.H"
#include <QDockWidget>
#include <QVector>
#include <QString>

class Gui;
class EditorPane;
class QTextEdit;
class QFont;

class Console : public QDockWidget {

   Q_OBJECT
   
   public:
      Console(Gui *parent, EditorPane *editorPane, const QString &name, bool createDisplay = true);
      virtual ~Console();

   protected slots:
      // Clears the console's display
      void flush();
      
      // Keeps the console's font in sync w/ that of the EditorPane
      void fontChanged(const QFont &newFont);
      
      virtual void visibilityChanged(bool isVisible);
      
   signals:
      void updateBatchSignal();
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      QTextEdit *m_display;
      
      QVector<QString> m_strings;
      bool m_visible;
      
      // Updates the QTextEdit used as a display
      virtual void updateDisplay();
};

#endif // __CONSOLE_H__

