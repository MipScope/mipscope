/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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

/*typedef int Status;
//#define Status    int
#define STOPPED   (0)
#define PAUSED    (1)
#define RUNNING   (2)*/
enum Status { STOPPED, PAUSED, RUNNING };
enum TerminationReason { T_COMPLETED, T_TERMINATED, T_INVALID_PROGRAM, T_ABNORMAL, T_FORCE_ROLLBACK, T_UNCAUGHT_EXCEPTION };

//#include <QMetaType>
//Q_DECLARE_METATYPE(Status);

#define CLEAN_TIMESTAMP    (0)

#define SENTINEL_ADDRESS      (0)
#define TEXT_BASE_ADDRESS     (0x400000)     //     4,194,304
#define DATA_BASE_ADDRESS     (0x10000000)   //   268,435,456
#define STACK_BASE_ADDRESS    (0x7ffffffc)   // 2,147,483,644  (grows down)

#define MAX_TEXT_SIZE         (DATA_BASE_ADDRESS - TEXT_BASE_ADDRESS - 4)
#define MAX_DATA_SIZE         (STACK_BASE_ADDRESS - DATA_BASE_ADDRESS)
#define MAX_STACK_SIZE        (MAX_DATA_SIZE)

// 2,143,289,340  --> 2 gigs
#define MEMORY_SIZE           (STACK_BASE_ADDRESS - TEXT_BASE_ADDRESS)

typedef unsigned long   TIMESTAMP;
#define MAX_TIMESTAMP   (ULONG_MAX)

typedef vector<ParseError> SyntaxErrors;
typedef SyntaxErrors::iterator SyntaxErrorIterator;

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

typedef QHash<unsigned int, QList<TIMESTAMP>* > MemoryUseMap;
typedef MemoryUseMap::iterator MemoryUseMapIterator;

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

