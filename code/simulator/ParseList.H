#ifndef PARSELIST_H_
#define PARSELIST_H_

#include "typedefs.H"

struct ParseNode {
	LINENUMBER line;
	Statement* statement;
	TIMESTAMP first_executed;
		
}

class ParseList : public QOBject {
	
	Q_OJBECT
	
	public:
		ParseList(void);
		
		
	private:
}

#endif /*PARSELIST_H_*/
