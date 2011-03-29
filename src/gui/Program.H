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
   file: Program.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007

   Acts as a proxy for the currently active Debugger
\* ---------------------------------------------- */
#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <QObject>
#include <QStack>
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
      
      bool isRunnable();

      SyntaxErrors *getSyntaxErrors() const;
      int getStatus() const; // gets status of Debugger
      
      ParseNode *getPC() const;
      State *getState() const;
      
      void getLastXInstructions(int no, QVector<ParseNode*> &instrs) const;
//      void customEvent(QEvent *e);
      
      // called whenever a ParseNode is deleted during a pause in program execution
      // ..  necessary because QTextBlocks automatically delete their associated 
      // content .. must assure that ParseNode is never referenced in the Gui
      void notifyParseNodeDeleted(ParseNode *wasDeleted);
      
      // called whenever the status of a register watchpoint changes
      void watchPointModified(unsigned int reg, bool watchPoint);
      bool *getWatchpoints() const;
      
      int lineNumber(ParseNode *parseNode);
      bool undoIsAvailable() const;
      
      ParseNode *getDeclaration(const QString &identifier);
      TIMESTAMP getCurrentTimestamp() const;
      
      MemoryUseMap *getMemoryUseMap() const;
      unsigned int getDataSize() const;

      LabelMap *getLabelMap() const;
      void gotoDeclaration(unsigned int address);
      bool getLabelForAddress(unsigned int address, QString &label);
      
      void setRunnable(bool);
   protected:
      void loadProgram(bool forceLoad);
      void updateSyntaxErrors(SyntaxErrors *newS);
      void forceUpdateOfSyntaxErrors(bool noChange = false);
      QString getName();
      
      void rollBackToEarliest(TIMESTAMP optionalTimestamp);
      
   private slots:
      // Slots outside of Debugger/Gui Relationship
      // ------------------------------------------
      void currentChanged(TextEditor*);
      void contentsChange(int position, int charsRemoved, int charsAdded);
      void singleShotSyntaxErrors();
      void loadProgram();
      
      // Slots from Debugger -> Gui
      // --------------------------
//      void syscallReceived(int no, int valueOfa0);
//      void undoSyscallReceived(int no);
      void undoAvailabilityChangeReceived(bool isAvailable);
      
      // Display a message notifying user of reason for pause in program execution in the status bar
      void notifyPause(const QString &reason);
      
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
      void forceStop();
      void pause();
      void run();
      void stepForward();
      void stepBackward();
      void stepForward(int noInstructions);
      void stepBackward(int noInstructions);
      void stepBackwardToTimestamp(TIMESTAMP stamp);
      void jumpTo(const QTextBlock&);
      
   signals:
      // Signals -> Gui  only if m_current
//      void syscall(State *s, int status, int syscallNo, int valueOfa0);
//      void undoSyscall(int syscallNo);
      void undoAvailabilityChanged(bool isAvailable);
      
      void pcChanged(ParseNode *pc, bool justRolledBack);
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc);
      void memoryChanged(unsigned int address, unsigned int value, ParseNode *pc);
      
      void updateMemory(Program*, State *state, int status);
      
      // emitted upon run, pause, stop of Debugger
      void programStatusChanged(int s);
      
      // emitted whenever the runnability of this program changes
      void validityChanged(bool isValid);
      
   protected:
      bool m_current; // if this Program is the currently active one
      bool m_runnable;

      enum ROLLING_BACK { R_NORMAL, R_ROLLING, R_JUST_ROLLED };
      ROLLING_BACK m_rollingBack;

      Gui *m_gui;
      EditorPane *m_editorPane;
      TextEditor *m_parent;
      
      // Related to Debugger
      SyntaxErrors *m_syntaxErrors;
      ParseList *m_parseList;
      Debugger *m_debugger;

      ParseNodeList m_modified;

      // Syntax errors aren't updated -immediately- cause it 
      // gets slow/annoying, so they're updated every couple 
      // hundred of ms. This stack keeps them in order as they're 
      // received
      QStack<bool> m_syntaxErrorStack;
};

#endif // __PROGRAM_H__

