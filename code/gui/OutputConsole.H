/* ---------------------------------------------- *\
   file: OutputConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __OUTPUT_CONSOLE_H__
#define __OUTPUT_CONSOLE_H__
#include <QDockWidget>
#include <QVector>

class QTextEdit;
class Gui;
class EditorPane;

class OutputConsole : public QDockWidget {
   
   Q_OBJECT
   
   public:
      OutputConsole(Gui *parent, EditorPane *editorPane);

      void push(const QString &newOutput);
      void pop();
      void flush();

   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      QTextEdit *m_display;
      
      QVector<QString> m_strings;
      
      void updateDisplay();
};

#endif // __OUTPUT_CONSOLE_H__

