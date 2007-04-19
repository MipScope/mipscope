#ifndef STATE_H_
#define STATE_H_

#include <QObject>

#include "typedefs.H"
class ParseNode;

class State : public QObject {
	Q_OBJECT
	
	public:
		State(ADDRESS memorySize);		
		
		/* updates the state's current caller, increments timestamp, returns
		 * new timestamp. */
		TIMESTAMP newTimestamp(ParseNode* caller);
		
		// memory
		void setMemoryWord(ADDRESS address, WORD value);
		void setMemoryByte(ADDRESS address, WORD value);
		WORD getMemoryWord(ADDRESS address);
		BYTE getMemoryByte(ADDRESS address);
		
		// registers
		void setRegister(REGISTER reg, WORD value);
		WORD getRegister(REGISTER reg);
		
		// pc
		void setPC(ParseNode* value);
		ParseNode* getPC(void);
		
		// undo
		void undoUntilTimestamp(TIMESTAMP timestamp);
//		TIMESTAMP getCurrentTimestamp(void);
		
	signals:
		
		/* this is emitted whenever a word in memory is changed, even if only
		 * one byte of this word was changed with a call to setMemoryByte. */
		void memoryWordChanged(ParseNode* who, ADDRESS address, WORD previous, WORD current);
		
		void registerChanged(ParseNode* who, REGISTER reg, WORD previous, WORD current);
		
		void pcChanged(ParseNode* who, ParseNode* previous, ParseNode* current);
		
	private:
		char* m_memory;
		ADDRESS m_size;
		
		WORD* m_registers;
		
		ParseNode* m_pc;
		ParseNode* m_caller;
		
		TIMESTAMP m_currentTimestamp;
		
};



#endif /*STATE_H_*/
