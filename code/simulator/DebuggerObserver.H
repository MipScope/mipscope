#ifndef DEBUGGEROBSERVER_H_
#define DEBUGGEROBSERVER_H_

#include "typedefs.H"

struct memory_word_log {
	ParseNode* who;
	WORD previous;
	WORD current;
};

struct register_log {
	ParseNode* who;
	WORD previous;
	WORD current;
};

class DebuggerObserver : public QOBject {
	
	Q_OJBECT
	
	public:
	
		DebuggerObserver(int memorySize);
		~DebuggerObserver(void);
	
		ParseNode* getMemoryWordWho(ADDRESS address);
		ParseNode* getRegisteWho(REGISTER reg);
		ParseNode* getPCWho(void);
		
		WORD getMemoryWordPrevious(ADDRESS address);
		WORD getRegisterPrevious(REGISTER reg);
		WORD getPCPrevious(void);
		
		WORD getMemoryWordCurrent(ADDRESS address);
		WORD getRegisterCurrent(ADDRESS reg);
		ParseNode* getPCCurrent(void);		
	
	public slots:
		void memoryWordChanged(ParseNode* who, unsigned int address, int previous, int current);
		void registerChanged(ParseNode* who, unsigned int reg, unsigned int previous, unsigned int current);
		void pcChanged(ParseNode* who, ParseNode* previous, ParseNode* current);
		
	private:
		int m_memorySize;
		struct memory_word* m_memoryLog;
		struct register_log* m_registerLog;
		ParseNode* m_pcWho;
		ParseNode* m_pcPrevious;
		ParseNode* m_pcCurrent;
		
};

#endif /*DEBUGGEROBSERVER_H_*/
