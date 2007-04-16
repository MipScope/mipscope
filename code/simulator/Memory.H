#ifndef MEMORY_H_
#define MEMORY_H_

#include "typedefs.H"

class Memory : public QOBject {
	
	Q_OJBECT
	
	public:
		Memory(unsigned int size);
		void setByte(ADDRESS address, unsigned char value);		
		BYTE getByte(ADDRESS address);
		
	private:
		char* m_memory;
		unsigned int m_size;	
}
		  

#endif /*MEMORY_H_*/
