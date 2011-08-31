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

#include "State.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "Debugger.H"
#include "typedefs.H"
#include <string.h>
#include "../gui/Syscalls.H"
#include "../gui/SyscallHandler.H"

// expected maximum memory usage (will grow if necessary)
#define INTERNAL_MEMORY_SIZE (16384)

State::State(SyscallListener *listener, Debugger *debugger) : 
   m_pc(NULL), m_currentTimestamp(CLEAN_TIMESTAMP), m_undoList(4096), 
   m_undoIsAvailable(false), m_debugger(debugger), 
   m_syscallListener(listener)
{
   m_memory.reserve(INTERNAL_MEMORY_SIZE);
   m_memoryUseMap.reserve(2048);
   reset();
}

TIMESTAMP State::newTimestamp() {
   if (m_currentTimestamp != CLEAN_TIMESTAMP)
      setUndoAvailability(true);

   return ++m_currentTimestamp;
}

void State::setMemoryWord(unsigned int address, unsigned int value) {
   ensureValidAlignment(address, 3);   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) { // record change
      m_undoList.push_back(new MemoryChangedAction(m_currentTimestamp, address, m_memory[address]));

      // record assignment in memory use map
      if (!m_memoryUseMap.contains(address))
         m_memoryUseMap.insert(address, new QList<TIMESTAMP>);
      QList<TIMESTAMP> *list = m_memoryUseMap[address];
      list->push_back(m_currentTimestamp);
   }

   m_memory[address] = value;
 
   if (m_currentTimestamp != CLEAN_TIMESTAMP) {
      // has to be direct
      m_debugger->memoryChanged(address, value, m_pc);
   }

   memoryChanged(address, value, m_pc);
}

void State::setMemoryByte(unsigned int address, unsigned char value) {
   ensureValidAlignment(address, 0);   

   // if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
   const unsigned int aligned = (address & ~3);
   m_memory[aligned] &= ~(0xFF << ((address & 3) << 3));  /* Zero out the byte. */
   m_memory[aligned] |= (value << ((address & 3) << 3));  /* Set the byte. */

   if (m_currentTimestamp != CLEAN_TIMESTAMP) { // record change
      m_undoList.push_back(new MemoryChangedAction(m_currentTimestamp, aligned, m_memory[aligned]));

      // record assignment in memory use map
      if (!m_memoryUseMap.contains(aligned))
         m_memoryUseMap.insert(aligned, new QList<TIMESTAMP>);
      QList<TIMESTAMP> *list = m_memoryUseMap[aligned];
      list->push_back(m_currentTimestamp);
   }
   
   memoryChanged(aligned, m_memory[aligned], m_pc);
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) {
      // has to be direct
      m_debugger->memoryChanged(aligned, value, m_pc);
   }
}

unsigned int State::getMemoryWord(unsigned int address) {
   ensureValidAlignment(address, 3);
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) {
      // record assignment in memory use map
      if (!m_memoryUseMap.contains(address))
         m_memoryUseMap.insert(address, new QList<TIMESTAMP>);
      QList<TIMESTAMP> *list = m_memoryUseMap[address];
      list->push_back(m_currentTimestamp);
   }
   
   if (m_memory.contains(address))
      return m_memory[address];
   
   return 0;
   //return *((unsigned int*) (m_memory.data() + address) );
}

unsigned int State::getMemoryWordUnobserved(unsigned int address) {
   if (m_memory.contains(address))
      return m_memory[address];

   return 0;
}

unsigned char State::getMemoryByteUnobserved(unsigned int address) {
   ensureValidAlignment(address, 0);

   unsigned int word = getMemoryWordUnobserved(address & ~3);
   if (word == 0)
      return 0;

   unsigned int shift = (address & 3) << 3;
   
   return (unsigned char)((word & (0xff << shift)) >> shift);
}

unsigned char State::getMemoryByte(unsigned int address) {
   ensureValidAlignment(address, 0);

   unsigned int word = getMemoryWord(address & ~3);
   if (word == 0)
      return 0;

   unsigned int shift = (address & 3) << 3;
   
   return (unsigned char)((word & (0xff << shift)) >> shift);
}

void State::ensureValidAlignment(unsigned int address, unsigned int align) const {
   if (address + align > STACK_BASE_ADDRESS || address < DATA_BASE_ADDRESS) {
      cerr << address << ", " << align << ", " << STACK_BASE_ADDRESS << ", " << DATA_BASE_ADDRESS << endl;
      
      throw InvalidAddress(address, m_pc->getTextBlock());
   }

   if (address > DATA_BASE_ADDRESS + m_debugger->getParseList()->getDataSize() 
       && address + align < m_registers[sp]) {
      cerr << address << ", " << align << ", " << endl;

      throw InvalidAddress(address, m_pc->getTextBlock());
   }
   
   if (address & align)
      throw BusError(address, m_pc->getTextBlock());
}

// for initializing strings
void State::memcpy(unsigned int destAddress, const void *src, unsigned int size) {
   char *ptr = (char*)src;
   TIMESTAMP old = m_currentTimestamp;
   m_currentTimestamp = CLEAN_TIMESTAMP;
   
   while(size-- > 0)
      setMemoryByte(destAddress++, *ptr++);

   m_currentTimestamp = old;
}

void State::memset(unsigned int destAddress, const int value, unsigned int size) {
   TIMESTAMP old = m_currentTimestamp;
   m_currentTimestamp = CLEAN_TIMESTAMP;

   while(size-- > 0)
      setMemoryByte(destAddress++, value);

   m_currentTimestamp = old;
}

QString State::getString(unsigned int address, bool affect) {
   unsigned char byte;
   QString s;
   TIMESTAMP old = m_currentTimestamp;
   if (!affect)
      m_currentTimestamp = CLEAN_TIMESTAMP;
   
   while((byte = getMemoryByte(address++)) != 0)
      s += QChar(byte);
   
   if (!affect)
      m_currentTimestamp = old;
   return s;
}

void State::setRegister(int reg, unsigned int value) {
   if (reg <= zero || reg >= register_count) {
      cerr << "set: " << reg << " = " << value << endl;
      cerr << m_pc->getTextBlock()->text().toStdString() << endl;
      // TODO:  add iostream operator to ParseNode

      throw InvalidRegister(reg, m_pc->getTextBlock());
   }
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new RegisterChangedAction(m_currentTimestamp, reg, m_registers[reg]));
   m_registers[reg] = value;
   registerChanged((unsigned)reg, value, m_pc);

   // has to be direct
   m_debugger->registerChanged((unsigned)reg, value, m_pc);
}

unsigned int State::getRegister(int reg) const {
   if (reg < zero || reg >= register_count) {
      cerr << "get: " << reg << endl;
      throw InvalidRegister(reg, m_pc->getTextBlock());
   }
   
   return m_registers[reg];
}

void State::incrementPC(int status) {
   setPC(ParseList::getClosestInstruction(m_pc->getNext(), status));
}

void State::setPC(ParseNode* value) {
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new PCChangedAction(this));
   
   m_pc = value;
//   cerr << (void*)value << endl; 
   
   pcChanged(m_pc);
}

ParseNode* State::getPC(void) const {
   return m_pc;	
}

void State::getStack(QVector<int> &stack) {
   unsigned int stackAddr = m_registers[sp];

   stack.clear();
   if (stackAddr > STACK_BASE_ADDRESS)
      return;
   
//   unsigned int stackSize = STACK_BASE_ADDRESS - stackAddr;
   for(unsigned int i = STACK_BASE_ADDRESS - 4; 
         i >= stackAddr; i -= 4)
      stack.push_back(getMemoryWord(i));
}

void State::getLastXInstructions(int no, QVector<ParseNode*> &instrs) const {
   instrs.clear();
   if (no < 1 || m_undoList.isEmpty() || m_undoList.back() == NULL)
      return;
   
//   cerr << "<<<getLast: cur = " << m_currentTimestamp << endl;
   int index = m_undoList.indexOf(m_undoList.back());
   if (index < 0)
      return;
   
   do {
      StateAction *action = m_undoList[index--];//*i;
      if (action == NULL)
         return;
      
      // see if we have a PCChangedAction
      if (PCChangedAction* pcAction = dynamic_cast<PCChangedAction*>(action)) {
         instrs.push_back(pcAction->m_oldPC);
         if (--no <= 0)
            return;
      }
   } while(index >= 0);
}

void State::undoLastInstruction() {
   undoUntilTimestamp(m_currentTimestamp - 1);
}

void State::undoUntilTimestamp(TIMESTAMP targetTimestamp) {
   StateAction *action;
   if (targetTimestamp >= m_currentTimestamp || m_undoList.isEmpty() || (action = m_undoList.back()) == NULL)
      return;

   //cerr << "<<<Undo: cur = " << m_currentTimestamp << ", target = " << targetTimestamp << endl;
   
   do {
      action->undo(this);
      m_currentTimestamp = action->m_timestamp;
      m_undoList.pop_back();
      delete action;
   } while(!m_undoList.isEmpty() && (action = m_undoList.back()) != NULL && action->m_timestamp >= targetTimestamp);

   pcChanged(m_pc);
   if (m_undoList.isEmpty() || action == NULL)
      setUndoAvailability(false, true);
   //cerr << ">>>done: cur = " << m_currentTimestamp << ", pc = '" << m_pc << "'\n";
}

void State::setUndoAvailability(bool isAvailable, bool force) {
   if (m_undoIsAvailable == isAvailable && !force)
      return;
   
   m_undoIsAvailable = isAvailable;
   undoAvailabilityChanged(isAvailable);
}

bool State::undoIsAvailable() const {
//   cerr << "State::undoIsAvailable: ";
//   cerr << m_undoIsAvailable << endl;
   
   return m_undoIsAvailable;
}

TIMESTAMP State::getCurrentTimestamp(void) const {
   return m_currentTimestamp;
}

// TODO:  should be protected by a MUTEX!
void State::reset() {
   ::memset(m_registers, 0, sizeof(int) * register_count);
   m_currentTimestamp = CLEAN_TIMESTAMP;
   m_pc = NULL;
   m_memory.clear();
   m_memoryUseMap.clear();

   foreach(StateAction *s, m_undoList) {
      if (s != NULL)
         delete s;
   }
   m_undoList.clear();
   
   setUndoAvailability(false, true);
}

// does the OS action depending on what's in $v0
void State::doSyscall(void) {
   int syscallNo = getRegister(v0);
   int valueOfA0 = getRegister(a0);
   if (VERBOSE) cerr << "\t\tSyscall called v0 = " << syscallNo << ", a0 = " << valueOfA0 << endl;
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new SyscallAction(this, syscallNo));
   
   // ALL syscalls handled in the Debugger Thread -- key to synching output w/ prog state
   if (syscallNo == 10) // syscall 10 is end program
      m_debugger->programStop();
   else m_syscallListener->syscall(this, m_debugger->getStatusWithoutLocking(), syscallNo, valueOfA0);
//      syscall(syscallNo, getRegister(a0));
}

void State::assertEquals(int val1, int val2) {
   if (val1 != val2) throw AssertionFailure(val1, val2, m_pc->getTextBlock());
}

MemoryUseMap *State::getMemoryUseMap() {
   return &m_memoryUseMap;
}


StateAction::StateAction(TIMESTAMP timestamp)//, ParseNode *isPC) 
   : m_timestamp(timestamp)//, m_isPCAction(isPC)
{ }

StateAction::~StateAction() { }

SyscallAction::SyscallAction(State *s, int syscallNo)
   : StateAction(s->m_currentTimestamp), m_syscallNo(syscallNo), 
   m_baseAddress(0), m_affectedLength(0)
{
   if (syscallNo == S_PRINT_STRING) {
      m_baseAddress = s->getRegister(a0);
      const QString &string = s->getString(m_baseAddress, false); // do not affect memoryUseMap
      
      m_affectedLength = string.length() + 1;
   }
}

SyscallAction::~SyscallAction() { }

void SyscallAction::undo(State *s) {
   s->m_syscallListener->undoSyscall(m_syscallNo);
//   s->undoSyscall(m_syscallNo);
   
   // undo memory accesses for printing strings
   if (m_syscallNo == S_PRINT_STRING) {
      for(unsigned int addr = 0; addr < m_affectedLength; addr++) {
         unsigned int address = ((m_baseAddress + addr) & ~3);
         QList<TIMESTAMP> *list = s->m_memoryUseMap[address];
         
         if (list != NULL && !list->isEmpty())
            list->pop_back();
      }
   }
   //cerr << "Undoing syscall " << m_syscallNo << endl;
}

PCChangedAction::PCChangedAction(State *s) 
   : StateAction(s->m_currentTimestamp), m_oldPC(s->m_pc)
{ }

void PCChangedAction::undo(State *s) {
   s->m_pc = m_oldPC;
   //cerr << "PC -> " << m_oldPC << endl;
}

RegisterChangedAction::RegisterChangedAction(TIMESTAMP timestamp, unsigned int reg, unsigned int oldVal) 
   : StateAction(timestamp), m_register(reg), m_value(oldVal)
{ }

void RegisterChangedAction::undo(State *s) {
   s->m_registers[m_register] = m_value;
   s->registerChanged(m_register, m_value, s->m_pc);
   s->m_debugger->registerChanged(m_register, m_value, s->m_pc);
   //cerr << "Reg " << registerAliases[m_register] << " -> " << m_value << endl;
}

MemoryChangedAction::MemoryChangedAction(TIMESTAMP timestamp, unsigned int address, unsigned int oldVal) 
   : StateAction(timestamp), m_address(address), m_value(oldVal)
{ }

void MemoryChangedAction::undo(State *s) {
   s->m_memory[m_address] = m_value;
   s->memoryChanged(m_address, m_value, s->m_pc);
   
   QList<TIMESTAMP> *list = s->m_memoryUseMap[m_address];
   list->pop_back();
   //cerr << "Mem " << m_address << " -> " << m_value << endl;
}

void State::setSyscallListener(SyscallListener *listener) {
   m_syscallListener = listener;
}

void State::exit() { // syscall 10
   throw new ExitSyscallHandlerCauseIAMTooLazyToWriteThisProperly(m_pc->getTextBlock());
}

void State::breakError() {
   breakError("");
}

void State::breakError(const QString &error) {
   throw BreakError(error, m_pc->getTextBlock());
}

void State::syscallError(const QString &error) {
   throw SyscallError(error, m_pc->getTextBlock());
}

