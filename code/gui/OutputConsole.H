/* ---------------------------------------------- *\
   file: OutputConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __OUTPUT_CONSOLE_H__
#define __OUTPUT_CONSOLE_H__
#include "Console.H"
#include "SyscallHandler.H"

class Gui;
class EditorPane;

class OutputConsole : public Console, 
                      public SyscallHandler 
{
   Q_OBJECT
   
   public:
      OutputConsole(Gui *parent, EditorPane *editorPane);
     
      // @overridden from Console
      void push(const QString &newOutput);
      void pop();

      // @overridden from SyscallHandler
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undoSyscall(int syscallNo);
      virtual void reset();

   protected:
      bool m_first; // since last reset
};

#endif // __OUTPUT_CONSOLE_H__

