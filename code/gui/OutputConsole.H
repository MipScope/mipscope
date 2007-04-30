/* ---------------------------------------------- *\
   file: OutputConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __OUTPUT_CONSOLE_H__
#define __OUTPUT_CONSOLE_H__

#include <QObject>

#include "Console.H"
#include "SyscallHandler.H"



class Gui;
class EditorPane;
class OutputAction;

class OutputConsole : public Console, 
                      public SyscallHandler 
{
   Q_OBJECT
   
   public:
      friend class NewOutputAction;
      friend class ClearOutputAction;
      
      OutputConsole(Gui *parent, EditorPane *editorPane);
     
      // @overridden from Console
      void push(const QString &newOutput);
      void pop();

      // @overridden from SyscallHandler
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undoSyscall(int syscallNo);
      virtual void reset();
      
      void setStrings(QVector<QString> *strings);
      
   protected:
      bool m_first; // since last reset

      QVector<OutputAction*> m_outputActions;
};

class OutputAction {
   public:
      OutputAction(int syscallNo);
      virtual ~OutputAction();
      
      virtual void undo(OutputConsole *output) = 0;

      int m_syscallNo;
};

class NewOutputAction : public OutputAction {
   public:
      NewOutputAction(OutputConsole *out, const QString &s, int syscallNo);

      virtual void undo(OutputConsole *output);
      
   protected:
      QString m_string;
};

class ClearOutputAction : public OutputAction {
   public:
      ClearOutputAction(OutputConsole *out, QVector<QString> strings);
      virtual ~ClearOutputAction();

      virtual void undo(OutputConsole *output);

   protected:
      QVector<QString> *m_strings;
};

class TextOutputConsole : public SyscallHandler
{
   Q_OBJECT
   
   public:
      
      TextOutputConsole(SyscallListener* listener);
     
      // @overridden from SyscallHandler
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undoSyscall(int);
      virtual void reset(void);
};


#endif // __OUTPUT_CONSOLE_H__

