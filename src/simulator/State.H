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

#ifndef STATE_H_
#define STATE_H_

#include <QObject>
#include <QVector>
#include "ParseList.H"

#include "typedefs.H"
class ParseNode;
class ParseList;
class StateAction;
class Debugger;
class SyscallListener;

extern bool VERBOSE;

class State : public QObject {
   Q_OBJECT

   public:
      friend class SyscallAction;
      friend class PCChangedAction;
      friend class RegisterChangedAction;
      friend class MemoryChangedAction;
      
      State(SyscallListener *listener, Debugger *debugger);

      // increments timestamp
      TIMESTAMP newTimestamp();
      TIMESTAMP getCurrentTimestamp(void) const;
      
      // memory
      void setMemoryWord(unsigned int address, unsigned int value);
      void setMemoryByte(unsigned int address, unsigned char value);
      unsigned int getMemoryWordUnobserved(unsigned int address);
      unsigned char getMemoryByteUnobserved(unsigned int address);
      unsigned int getMemoryWord(unsigned int address);
      unsigned char getMemoryByte(unsigned int address);

      void ensureValidAlignment(unsigned int address, unsigned int align) const;

      // utility methods for bulk-operations (mainly for Directive-initialization)
      void memcpy(unsigned int destAddress, const void *src, unsigned int size);
      void memset(unsigned int destAddress, const int value, unsigned int size);
      
      QString getString(unsigned int address, bool affect = true);
      
      // registers
      void setRegister(int reg, unsigned int value);
      unsigned int getRegister(int reg) const;

      // pc
      void incrementPC(int status = ParseList::m_status);
      void setPC(ParseNode* value);
      ParseNode* getPC(void) const;

      // undo
      void undoLastInstruction();
      void undoUntilTimestamp(TIMESTAMP timestamp);
      
      // convenience-methods
      void getLastXInstructions(int no, QVector<ParseNode*> &instrs) const;
      void getStack(QVector<int> &stack);
      
      // special
      void doSyscall(void);
      void assertEquals(int val1, int val2); // throws AssertionFailure
      
      // completely resets the state to its original state
      void reset();
      
      bool undoIsAvailable() const;
      
      MemoryUseMap *getMemoryUseMap();
      
      void setSyscallListener(SyscallListener *listener);
      void exit(); // syscall 10

      void breakError();
      void breakError(const QString &error);

      // syscalls can tell debugger to break upon encountering
      // an error - this is the only safe way for proxied syscalls to raise an error
      void syscallError(const QString &error);
      
   signals:
      //void syscall(int, int);
      //void undoSyscall(int);
      
      // emitted whenever the availability of 'undo' or 'step back' changes
      void undoAvailabilityChanged(bool isAvailable);
      
      /* this is emitted whenever a word in memory is changed, even if only
       * one byte of this word was changed with a call to setMemoryByte. */
      void memoryChanged(unsigned int address, unsigned int value, ParseNode *pc);
      void registerChanged(unsigned int register, unsigned int value, ParseNode *pc);
      void pcChanged(ParseNode *pc);

   private:
      unsigned int m_registers[register_count];
      QHash<unsigned int, unsigned int> m_memory;
      //QVector<char> m_text;

      ParseNode* m_pc;

      TIMESTAMP m_currentTimestamp;
      QVector<StateAction*> m_undoList;
      bool m_undoIsAvailable;
      
      Debugger *m_debugger;
      MemoryUseMap m_memoryUseMap;
      
      // proxy for all syscalls; ensures syscall numbers get mapped to the right 
      // handlers -- all of this happens in the debugger's thread!
      SyscallListener *m_syscallListener;
      
      void setUndoAvailability(bool isAvailable, bool force = false);
};

class StateAction {
   public:
      StateAction(TIMESTAMP timestamp);
      virtual ~StateAction();

      TIMESTAMP m_timestamp;
      virtual void undo(State *s) = 0;
};

class SyscallAction : public StateAction {
   public:
      SyscallAction(State *s, int syscallNo);
      virtual ~SyscallAction();
      
      void undo(State *s);
   private:
      int m_syscallNo;
      unsigned int m_baseAddress;
      unsigned int m_affectedLength;
};

class PCChangedAction : public StateAction {
   public:
      PCChangedAction(State *s);
      
      void undo(State *s);
      
      ParseNode *m_oldPC;
};

class RegisterChangedAction : public StateAction {
   public:
      RegisterChangedAction(TIMESTAMP timestamp, unsigned int reg, unsigned int oldVal);
      
      void undo(State *s);
   private:
      unsigned int m_register;
      unsigned int m_value;
};

// Records a change of one *word* of memory (bytes are handled w/ their associated words)
class MemoryChangedAction : public StateAction {
   public:
      MemoryChangedAction(TIMESTAMP timestamp, unsigned int address, unsigned int oldVal);
      
      void undo(State *s);
   private:
      unsigned int m_address;
      unsigned int m_value;
};

#endif // STATE_H_

