#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <QString>
#include <QHash>
#include <stdio.h>

class Statement;
class ParseNode;
class StatementArgList;
class Instruction;
class Directive;
//class StatementInfo;

#define CLEAN_TIMESTAMP    (0)

typedef unsigned int TIMESTAMP;
typedef uint32_t ADDRESS;
typedef uint32_t WORD;
typedef unsigned char BYTE;
//typedef Statement*(*STATEMENT_FACTORY)(StatementArgList*);
//typedef bool(*STATEMENT_IS_VALID)(StatementArgList*) const;

typedef QHash<QString, Instruction*> InstructionMap;
typedef InstructionMap::iterator InstructionMapIterator;
typedef QHash<QString, Directive*> DirectiveMap;
typedef DirectiveMap::iterator DirectiveMapIterator;

// Global QHashes containing representative instances (factories) of each supported Statement
extern InstructionMap instructionMap;
extern DirectiveMap directiveMap;

/* these are in order so the n'th register is n'th in the list */
enum REGISTER {	zero, at, 
   v0, v1, 
   a0, a1, a2, a3,
   t0, t1, t2, t3, t4, t5, t6, t7,
   s0, s1, s2, s3, s4, s5, s6, s7,
   t8, t9,
   k0, k1,
   gp, sp, fp, s8 = fp, ra, 
   pc, 
   hi, lo, // users can't access these, maybe they don't belong here
   register_count // dummy just to keep track of how many registers
};

// Strings indexed by the enum above
extern const char *const registerAliases[];

#define safeDelete(p)         {if ((p)) { delete (p); (p) = NULL; }}
#define safeDeleteArray(p)    {if ((p)) { delete[] (p); (p) = NULL; }}

#define safeFree(p)           {if ((p)) { free((p)); (p) = NULL; }}

#endif // TYPEDEFS_H_

