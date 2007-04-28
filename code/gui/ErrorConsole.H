/* ---------------------------------------------- *\
   file: ErrorConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/28/2007
\* ---------------------------------------------- */
#ifndef __ERROR_CONSOLE_H__
#define __ERROR_CONSOLE_H__
#include "Console.H"
#include "Utilities.H"

class Gui;
class EditorPane;
class TextEditor;
class QUrl;

class ErrorConsole : public Console {

   Q_OBJECT
   
   public:
      ErrorConsole(Gui *parent, EditorPane *editorPane);
      
      void updateSyntaxErrors(SyntaxErrors *s, TextEditor *editor, bool forceUpdate = true);
      virtual void reset();
   
   private slots:
      void anchorClicked(const QUrl &link);
      
   protected:
      QString m_text;
      TextEditor *m_activeEditor; // one for which we're displaying errors
};

#endif // __ERROR_CONSOLE_H__

