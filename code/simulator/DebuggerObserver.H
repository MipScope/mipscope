#ifndef DEBUGGEROBSERVER_H_
#define DEBUGGEROBSERVER_H_

#include "typedefs.H"

struct memory_word_log {
	ParseNode* who;
	unsigned int previous;
	unsigned int current;
};

struct register_log {
	ParseNode* who;
	unsigned int previous;
	unsigned int current;
};

class DebuggerObserver : public QOBject {
	
	Q_OJBECT
	
	public:
	
		DebuggerObserver(int memorySize);
		~DebuggerObserver(void);
	
		ParseNode* getMemoryWordWho(unsigned int address);
		ParseNode* getRegisteWho(int reg);
		ParseNode* getPCWho(void);
		
		unsigned int getMemoryWordPrevious(unsigned int address);
		unsigned int getRegisterPrevious(int reg);
		unsigned int getPCPrevious(void);
		
		unsigned int getMemoryWordCurrent(unsigned int address);
		unsigned int getRegisterCurrent(unsigned int reg);
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
