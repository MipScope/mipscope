/* ---------------------------------------------- *\
   file: Program.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007

   Acts as a proxy for the currently active Debugger
\* ---------------------------------------------- */
#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include <QObject>
#include "../simulator/typedefs.H"

class State;
class ParseNode;
class TextEditor;
class EditorPane;
class Gui;
class Debugger;
class ParseList;
class QTextBlock;
class QEvent;

class Program : public QObject {
   
   Q_OBJECT
   
   // slots for everything from Gui -> Debugger
   //    Only passes onto Debugger if m_current
   //
   // slots for everything from Debugger -> Gui
   //    On
      
   public:
      friend class TextEditor;
      
      Program(Gui *gui, EditorPane *editorPane, TextEditor *parent);
      
      bool isRunnable() const;

      SyntaxErrors *getSyntaxErrors() const;
      int getStatus() const; // gets status of Debugger
      
      ParseNode *getPC() const;
      State *getState() const;
      
      void getLastXInstructions(int no, QVector<ParseNode*> &instrs) const;
//      void customEvent(QEvent *e);
      
   protected:
      void setRunnable(bool);
      void updateSyntaxErrors(SyntaxErrors *newS);
      
   private slots:
      // Slots outside of Debugger/Gui Relationship
      // ------------------------------------------
      void currentChanged(TextEditor*);
      // TODO:  add contentChanged thing
   
      // Slots from Debugger -> Gui
      // --------------------------
      void syscallReceived(int no, int valueOfa0);
      void undoSyscallReceived(int no);
      void undoAvailabilityChangeReceived(bool isAvailable);
      
      // only when debugger is paused
      void pcChangeReceived(ParseNode *pc);
      void registerChangeReceived(unsigned int reg, unsigned int current, ParseNode *pc);
      void memoryChangeReceived(unsigned int address, unsigned int value, ParseNode *pc);
      
      void programStatusChangeReceived(int s);
      // called whenever Debugger thread exits
      void programTerminated(int reason);
      
      // Slots from Gui -> Debugger
      // --------------------------
      void stop();
      void pause();
      void run();
      void stepForward();
      void stepBackward();
      void stepBackwardToTimestamp(TIMESTAMP stamp);
      void jumpTo(const QTextBlock&);
      
   signals:
      // Signals -> Gui  only if m_current
      void syscall(State *s, int status, int syscallNo, int valueOfa0);
      void undoSyscall(int syscallNo);
      void undoAvailabilityChanged(bool isAvailable);
      
      void pcChanged(ParseNode *pc);
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc);
      void memoryChanged(unsigned int address, unsigned int value, ParseNode *pc);
      
      // emitted upon run, pause, stop of Debugger
      void programStatusChanged(int s);
      
      // emitted whenever the runnability of this program changes
      void validityChanged(bool isValid);
      
   protected:
      bool m_current; // if this Program is the currently active one
      bool m_runnable;

      Gui *m_gui;
      EditorPane *m_editorPane;
      TextEditor *m_parent;
      
      // Related to Debugger
      SyntaxErrors *m_syntaxErrors;
      ParseList *m_parseList;
      Debugger *m_debugger;
};

#endif // __PROGRAM_H__

