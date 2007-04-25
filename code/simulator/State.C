#include <stdlib.h>
#include <string.h>

#include "State.H"
#include "typedefs.H"
#include "ParseNode.H"
#include "StateException.H"

State::State(unsigned int memorySize) :	m_memory(memorySize, 0),
                                          m_registers(register_count, 0),
                                          m_pc(NULL),
                                          m_caller(NULL),
                                          m_currentTimestamp(0)
{ }

TIMESTAMP State::newTimestamp(ParseNode* caller) {
   m_caller = caller;
   return ++m_currentTimestamp;
}

void State::setMemoryWord(unsigned int address, unsigned int value) {

   if (address + 4 >= (unsigned int) m_memory.size()) throw InvalidAddress(address);
   if (address % 4 != 0) throw BusError(address);
   
   m_memory[address + 0]	=	(value >> 0)	& 0xFF;
   m_memory[address + 1]	= 	(value >> 8)	& 0xFF;
   m_memory[address + 2]	= 	(value >> 16)	& 0xFF;
   m_memory[address + 3]	= 	(value >> 24)	& 0xFF;
}

void State::setMemoryByte(unsigned int address, unsigned char value) {

   if (address >= (unsigned int) m_memory.size()) throw InvalidAddress(address);

   m_memory[address] = value;
}

unsigned int State::getMemoryWord(unsigned int address) {

   if (address + 4 >= (unsigned int) m_memory.size()) throw InvalidAddress(address);
   if (address % 4 != 0) throw BusError(address);

   return *((unsigned int*) (m_memory.data() + address) ); 
}

unsigned char State::getMemoryByte(unsigned int address) {

   if (address >= (unsigned int) m_memory.size()) throw InvalidAddress(address);

   return m_memory[address]; 
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

void State::setRegister(int reg, unsigned int value) {

   if (reg >= m_registers.size() || reg == zero ||
         reg == hi || reg == lo || reg >= register_count)
      throw InvalidRegister(reg);
   
   m_registers[reg] = value;
}

unsigned int State::getRegister(int reg) {

   if (reg >= m_registers.size() ||
         reg == hi || reg == lo || reg == register_count)
      throw InvalidRegister(reg);

   return m_registers[reg];
}

void State::setPC(ParseNode* value) {
   m_pc = value;	
}

ParseNode* State::getPC(void) {
   return m_pc;	
}

void State::undoUntilTimestamp(TIMESTAMP timestamp) {
   // TODO: unimplemented
}

TIMESTAMP State::getCurrentTimestamp(void) {
   return m_currentTimestamp;
}

// does the OS action depending on what's in $v0
void State::syscall(void) {

   // TODO: make these legit.
   
   switch (getRegister(v0)) {
      case 1:
         // print_int: print out what's in $a0
         cout << getRegister(a0);
         break;
         
      case 4:
         // print_string: print out what's pointed to by $a0
      
      case 5:
         // read_int: read an integer, stick it in $v0
         
      default:
         assert(false); // the rest are unimplemented
         
   }
}

