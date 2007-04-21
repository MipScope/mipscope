#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <QString>

class Statement;
class ParseNode;

typedef unsigned int TIMESTAMP;
typedef uint32_t ADDRESS;
typedef uint32_t WORD;
typedef unsigned char BYTE;
typedef Statement*(*STATEMENT_FACTORY)(const QString&, ParseNode*);
typedef bool(*STATEMENT_IS_VALID)(const QString&, ParseNode*);  

/* these are in order so the n'th register is n'th in the list */
enum REGISTER {	zero, at, 
				v0, v1, 
				a0, a1, a2, a3,
	 			t0, t1, t2, t3, t4, t5, t6, t7,
	 			s0, s1, s2, s3, s4, s5, s6, s7,
	 			t8, t9,
	 			k0, k1,
	 			gp, sp, fp, ra,
	 			hi, lo, // users can't access these, maybe they don't belong here
	 			pc,
	 			register_count // dummy just to keep track of how many registers
};

#endif /*TYPEDEFS_H_*/
