#include "State.H"
#include "StateException.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "typedefs.H"
#include <string.h>

#define INTERNAL_MEMORY_SIZE (16384)

State::State(ParseList* parseList) : m_pc(NULL), m_currentTimestamp(CLEAN_TIMESTAMP), m_parseList(parseList)
{
   m_memory.reserve(INTERNAL_MEMORY_SIZE);
   reset();
}

TIMESTAMP State::newTimestamp() {
   return ++m_currentTimestamp;
}

ParseList* State::getParseList(void) {
   return m_parseList;
}

void State::setMemoryWord(unsigned int address, unsigned int value) {
   ensureValidAlignment(address, 3);   
   // if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
   
   m_memory[address] = value;
   memoryChanged(address, value);
}

void State::setMemoryByte(unsigned int address, unsigned char value) {
   ensureValidAlignment(address, 0);   

   // if (m_currentTimestamp != CLEAN_TIMESTAMP) // record change
   unsigned int result = 
      (m_memory[address & ~3] |= (value << ((address & 3) << 3)));

   memoryChanged(address & ~3, result);
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

   unsigned int word = getMemoryByte(address & ~3);
   return (unsigned char)(word & (0xff << ((address & 3) << 3)));
}

void State::ensureValidAlignment(unsigned int address, unsigned int align) const {
   if (address + align >= STACK_BASE_ADDRESS || address < DATA_BASE_ADDRESS)
      throw InvalidAddress(address);
   
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
   m_pc = value;
//   cerr << (void*)value << endl; 
   // TODO:  detect when next node does not exit and signal program completion/termination -- done in Debugger?
   
   
   pcChanged(m_pc);
}

ParseNode* State::getPC(void) const {
   return m_pc;	
}

void State::undoUntilTimestamp(TIMESTAMP timestamp) {
   // TODO: unimplemented
   timestamp = 0;
}

TIMESTAMP State::getCurrentTimestamp(void) const {
   return m_currentTimestamp;
}

void State::reset() {
   ::memset(m_registers, 0, sizeof(int) * register_count);
   m_currentTimestamp = CLEAN_TIMESTAMP;
   m_pc = NULL;
   m_memory.clear();
}

// does the OS action depending on what's in $v0
void State::doSyscall(void) {
   cerr << "\t\tSyscall called v0 = " << getRegister(v0) << ", a0 = " << getRegister(a0) << endl;
   
   syscall(getRegister(v0), getRegister(a0));
}

void State::assertEquals(int val1, int val2) {
   if (val1 != val2) throw AssertionFailure(val1, val2);
}

