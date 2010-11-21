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
class OutputHelper;

class OutputConsole : public Console, 
                      public SyscallHandler 
{
   public:
      friend class NewOutputAction;
      friend class ClearOutputAction;
      
      OutputConsole(Gui *parent, EditorPane *editorPane);
      
      void update();
      void updateBatch();
      
      // @overridden from Console
      void push(const QString &newOutput);
      void pop();

      // @overridden from SyscallHandler
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undoSyscall(int syscallNo);
      virtual void reset();
      
      void setStrings(QVector<QString> *strings);
   
   protected:
      QVector<OutputAction*> m_outputActions;
      OutputHelper *m_outputHelper;
      int m_noUpdates; // pseudo-stack to only update once-per-batch
};

class OutputHelper : public QObject {
   
   Q_OBJECT
   
   public:
      OutputHelper(OutputConsole *outputConsole);
 
   public slots:
      void updateBatch();
      
   private:
      OutputConsole *m_output;
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
     void handleInputSyscall(State *s, int status, int syscallNo, int valueOfa0);
   public:
      
      TextOutputConsole(SyscallListener* listener);
     
      // @overridden from SyscallHandler
      virtual void syscall(State *s, int status, int syscallNo, int valueOfa0);
      virtual void undoSyscall(int);
      virtual void reset(void);
};

class InputSyscallHandler : public SyscallHandler {
   public:
      InputSyscallHandler(Gui *gui, SyscallListener *listener);
      
      void syscall(State *s, int status, int syscallNo, int valueOfa0);

      void undoSyscall(int syscallNo);
      void reset();

   protected:
      Gui *m_gui;
};

#endif // __OUTPUT_CONSOLE_H__

