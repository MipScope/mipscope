#include <stdlib.h>
#include <string.h>

#include "State.H"
#include "typedefs.H"
#include "ParseNode.H"
#include "StateException.H"

State::State(ADDRESS memorySize) :	m_memory(memorySize, 0),
									m_registers(register_count, 0),
									m_pc(NULL),
									m_caller(NULL),
									m_currentTimestamp(0)
{ }

TIMESTAMP State::newTimestamp(ParseNode* caller) {
	
	m_caller = caller;
	return ++m_currentTimestamp;
}

void State::setMemoryWord(ADDRESS address, WORD value) {

	if (address + 4 >= (unsigned int) m_memory.size()) throw InvalidAddress(address);
	if (address % 4 != 0) throw BusError(address);
	
	m_memory[address + 0]	=	(value >> 0)	& 0xFF;
	m_memory[address + 1]	= 	(value >> 8)	& 0xFF;
	m_memory[address + 2]	= 	(value >> 16)	& 0xFF;
	m_memory[address + 3]	= 	(value >> 24)	& 0xFF;		  
}

void State::setMemoryByte(ADDRESS address, BYTE value) {

	if (address >= (unsigned int) m_memory.size()) throw InvalidAddress(address);
	
	m_memory[address] = value;
}

WORD State::getMemoryWord(ADDRESS address) {

	if (address + 4 >= (unsigned int) m_memory.size()) throw InvalidAddress(address);
	if (address % 4 != 0) throw BusError(address);

	return *((unsigned int*) (m_memory.data() + address) ); 
}

BYTE State::getMemoryByte(ADDRESS address) {

	if (address >= (unsigned int) m_memory.size()) throw InvalidAddress(address);

	return m_memory[address]; 
}

void State::setRegister(REGISTER reg, WORD value) {
	
	if (reg >= m_registers.size() || reg == zero ||
				reg == hi || reg == lo || reg == register_count)
				throw InvalidRegister(reg);

	m_registers[reg] = value;	
}

WORD State::getRegister(REGISTER reg) {
	
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
