#include "State.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "typedefs.H"
#include <string.h>

#define INTERNAL_MEMORY_SIZE (16384)

State::State() : m_pc(NULL), m_currentTimestamp(CLEAN_TIMESTAMP), 
                 m_undoList(4096), m_undoIsAvailable(false)
{
   m_memory.reserve(INTERNAL_MEMORY_SIZE);
   reset();
}

TIMESTAMP State::newTimestamp() {
   if (m_currentTimestamp != CLEAN_TIMESTAMP)
      setUndoAvailability(true);

   return ++m_currentTimestamp;
}

void State::setMemoryWord(unsigned int address, unsigned int value) {
   ensureValidAlignment(address, 3);   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new MemoryChangedAction(m_currentTimestamp, address, m_memory[address]));
   
   m_memory[address] = value;
   memoryChanged(address, value);
}

void State::setMemoryByte(unsigned int address, unsigned char value) {
   ensureValidAlignment(address, 0);   

   // if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
   const unsigned int aligned = (address & ~3);
   unsigned int result = 
      (m_memory[aligned] |= (value << ((address & 3) << 3)));

   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new MemoryChangedAction(m_currentTimestamp, aligned, m_memory[aligned]));
   memoryChanged(aligned, result);
}

unsigned int State::getMemoryWord(unsigned int address) const {
   ensureValidAlignment(address, 3);
    
   if (m_memory.contains(address))
      return m_memory[address];
   
   return 0;
   //return *((unsigned int*) (m_memory.data() + address) );
}

unsigned char State::getMemoryByte(unsigned int address) const {
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
      
      throw InvalidAddress(address);
   }
   
   if (address & align)
      throw BusError(address);
}

void State::memcpy(unsigned int destAddress, const void *src, unsigned int size) {
   char *ptr = (char*)src;
   
   while(size-- > 0)
      setMemoryByte(destAddress++, *ptr++);
}

void State::memset(unsigned int destAddress, const int value, unsigned int size) {
   while(size-- > 0)
      setMemoryByte(destAddress++, value);
}

QString State::getString(unsigned int address) const {
   unsigned char byte;
   QString s;
   
   while((byte = getMemoryByte(address++)) != 0)
      s += QChar(byte);
   
   return s;
}

void State::setRegister(int reg, unsigned int value) {
   if (reg <= zero || reg >= pc) {
      cerr << "set: " << reg << " = " << value << endl;
      cerr << m_pc->getTextBlock()->text().toStdString() << endl;
      // TODO:  add iostream operator to ParseNode

      throw InvalidRegister(reg);
   }
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new RegisterChangedAction(m_currentTimestamp, reg, m_registers[reg]));
   m_registers[reg] = value;
   registerChanged((unsigned)reg, value);
}

unsigned int State::getRegister(int reg) const {
   if (reg < zero || reg >= pc) {
      cerr << "get: " << reg << endl;
      throw InvalidRegister(reg);
   }
   
   return m_registers[reg];
}

void State::incrementPC() {
   setPC(ParseList::getClosestInstruction(m_pc->getNext()));
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
      
      PCChangedAction *pcAction = NULL;
      try { // see if we have a PCChangedAction
         pcAction = dynamic_cast<PCChangedAction*>(action);
      } catch(bad_alloc&) { }

      if (pcAction != NULL) {
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

TIMESTAMP State::getCurrentTimestamp(void) const {
   return m_currentTimestamp;
}

void State::reset() {
   ::memset(m_registers, 0, sizeof(int) * register_count);
   m_currentTimestamp = CLEAN_TIMESTAMP;
   m_pc = NULL;
   m_memory.clear();
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
   cerr << "\t\tSyscall called v0 = " << syscallNo << ", a0 = " << getRegister(a0) << endl;
   
   if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
      m_undoList.push_back(new SyscallAction(m_currentTimestamp, syscallNo));
   syscall(syscallNo, getRegister(a0));
}

void State::assertEquals(int val1, int val2) {
   if (val1 != val2) throw AssertionFailure(val1, val2);
}

StateAction::StateAction(TIMESTAMP timestamp)//, ParseNode *isPC) 
   : m_timestamp(timestamp)//, m_isPCAction(isPC)
{ }

StateAction::~StateAction() { }

SyscallAction::SyscallAction(TIMESTAMP timestamp, int syscallNo)
   : StateAction(timestamp), m_syscallNo(syscallNo)
{ }

void SyscallAction::undo(State *s) {
   s->undoSyscall(m_syscallNo);
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
   s->registerChanged(m_register, m_value);
   //cerr << "Reg " << registerAliases[m_register] << " -> " << m_value << endl;
}

MemoryChangedAction::MemoryChangedAction(TIMESTAMP timestamp, unsigned int address, unsigned int oldVal) 
   : StateAction(timestamp), m_address(address), m_value(oldVal)
{ }

void MemoryChangedAction::undo(State *s) {
   s->m_memory[m_address] = m_value;
   s->memoryChanged(m_address, m_value);
   
   //cerr << "Mem " << m_address << " -> " << m_value << endl;
}

