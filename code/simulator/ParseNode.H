#ifndef PARSENODE_H_
#define PARSENODE_H_

class ParseNode : public QOBject {
	
	Q_OJBECT
	
	public:
		ParseNode(unsigned int size);
		void setByte(unsigned int address, unsigned char value);		
		char getByte(unsigned int address);
		
	private:
		char* m_memory;
		unsigned int m_size;	
}

#endif /*PARSENODE_H_*/
