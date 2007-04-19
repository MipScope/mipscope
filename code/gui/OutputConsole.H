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
class QFont;

class OutputConsole : public QDockWidget {
   
   Q_OBJECT
   
   public:
      OutputConsole(Gui *parent, EditorPane *editorPane);
      
      // Adds a new string to the output console's display
      void push(const QString &newOutput);
      
      // Removes (undoes) the last push of new output
      void pop();

   private slots:
      // Clears the console's display
      void flush();
      
      // Keeps the console's font in sync w/ that of the EditorPane
      void fontChanged(const QFont &newFont);
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      QTextEdit *m_display;
      
      QVector<QString> m_strings;
      
      // Updates the QTextEdit used as a display after a change to m_strings
      void updateDisplay();
};

#endif // __OUTPUT_CONSOLE_H__

