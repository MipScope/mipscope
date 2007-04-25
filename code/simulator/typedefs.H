#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <QString>
#include <QHash>
#include <vector>
#include <stdio.h>
#include <iostream>
using namespace std;

class Statement;
class ParseNode;
class StatementArgList;
class Instruction;
class AddressIdentifier;
class Directive;
class ParseError;

#define CLEAN_TIMESTAMP    (0)

typedef unsigned int TIMESTAMP;

typedef vector<ParseError> SyntaxErrors;

typedef QHash<QString, Instruction*> InstructionMap;
typedef InstructionMap::iterator InstructionMapIterator;
typedef QHash<QString, Directive*> DirectiveMap;
typedef DirectiveMap::iterator DirectiveMapIterator;

// used by Parser::parseLine -- updated when it finds a new label
typedef QHash<QString, AddressIdentifier*> LabelMap;
typedef LabelMap::iterator LabelMapIterator;

// for copying/pasting consts and #defines during Parser::parseLine
typedef QHash<QString, QString> PreProcessorMap;
typedef PreProcessorMap::iterator PreProcessorMapIterator;

typedef list<ParseNode*> ParseNodeList;
typedef ParseNodeList::iterator ParseNodeListIterator;

// string -> ParseNodes for which that string has caused a semantic error
//    (not defined at the time when that ParseNode was inserted)
typedef QHash<QString, ParseNodeList*> SemanticErrors;
typedef SemanticErrors::iterator SemanticErrorsIterator;


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

