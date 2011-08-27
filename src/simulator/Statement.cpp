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

#include "Statement.H"
#include "StatementArgList.H"
#include "SyntaxList.H"
#include "typedefs.H"
#include "ParseNode.H"
#include "State.H"
#include <exception>
#include <assert.h>
#include <typeinfo>

// Include all Statement Implementations
#include "statements/Arithmetic.H"
#include "statements/DataTransfer.H"
#include "statements/Special.H"
#include "statements/AssertInstr.H"
#include "statements/ControlFlow.H"
#include "statements/Comparison.H"

Statement::Statement(unsigned int requiredSizeInBytes) 
   : m_size(requiredSizeInBytes)
{ }

unsigned int Statement::getSizeInBytes() const {
   return m_size;
}

unsigned int Statement::getPreferredAlignment() const {
   return 2; // preferred 4-byte boundary (may be overridden by subclasses)
   // (yes, it's supposed to be a 2)
}

void Statement::initialize(ParseNode *p, State *s) { p = NULL; s = NULL; }

void Statement::InitializeStatementMaps() {
   Instruction::InitializeInstructionMap();
   Directive::InitializeDirectiveMap();

   // as of 5/8/2007,  122 instructions and 11 directives implemented
//   cerr << instructionMap.size() << ", " << directiveMap.size() << endl;
}

bool Statement::isInstruction() const {
   return dynamic_cast<const Instruction*>(this);
}

bool Statement::isDirective() const {
   return dynamic_cast<const Directive*>(this);
}

bool Statement::isPreprocessor() const { // consts and #defines
   return dynamic_cast<const ConstStatement*>(this);
}

int Statement::importance() const {
   return 75;
}

Directive   *Statement::getDirective(Statement *s) {
   return dynamic_cast<Directive*>(s);
}

Instruction *Statement::getInstruction(Statement *s) {
   return dynamic_cast<Instruction*>(s);
}

ConstStatement *Statement::getConst(Statement *s) {
   return dynamic_cast<ConstStatement*>(s);
}

int Done::importance() const {
   return 90;
}

int ConstStatement::importance() const {
   return 0;
}

// ----------------------
// Instruction Superclass
// ----------------------
Instruction::Instruction(StatementArgList *args) 
   : Statement(4), m_args(args) // every instruction only takes a word of the address space
{ }

Instruction::~Instruction() { safeDelete(m_args); }

StatementArgList *Instruction::getArguments() const {
   return m_args;
}

bool Instruction::autoIncrementPC() const {
   return true;
}

// Store the address of the next instruction in $a0 (by default)
void Instruction::link(State *s, unsigned int registerNo) const {
   ParseNode *p = s->getPC();
//   cerr << ">>>LINKING: " << p << ", registerNo = " << registerNo << endl;
   if (p == NULL)
      return;
   
   ParseNode *next = ParseList::getClosestInstruction(p->getNext());
   if (next == NULL)
      s->setRegister(registerNo, p->getAddress() + 4); // best guess
   else s->setRegister(registerNo, next->getAddress()); // accurate
}

int Instruction::getType() const {
   return ARITHMETIC; // overridden in subclasses
}

void Instruction::insert(Instruction *instr) {
   // try to ensure we didn't mess up w/ names of instructions..
   assert(!instructionMap.contains(instr->getName()));
   
   instructionMap.insert(instr->getName(), instr);
}

void Instruction::get_referenced_labels (std::vector<AddressIdentifier*>& labels)
{
      for(int i = 0; i < m_args->noArgs(); i++) {
         StatementArg *cur = (*m_args)[i];
         
         if (cur->hasAddressIdentifier()) {
            labels.push_back(cur->getID()->getAddressIdentifier());
	 }
      }
}

InstructionMap instructionMap;
void Instruction::InitializeInstructionMap() {
   // Arithmetic.H
   insert(new Abs());
   insert(new Add());
   insert(new Addu());
   insert(new Addi());
   insert(new Addiu());
   insert(new And());
   insert(new Andi());
   insert(new Clo());
   insert(new Clz());
   insert(new Div());
   insert(new Divu());
   insert(new Mult());
   insert(new Multu());
   insert(new Mul());
   insert(new Mulo());
   insert(new Mulou());
   insert(new Madd());
   insert(new Maddu());
   insert(new Msub());
   insert(new Msubu());
   insert(new Neg());
   insert(new Negu());
   insert(new Nor());
   insert(new Not());
   insert(new Or());
   insert(new Ori());
   insert(new Rem());
   insert(new Remu());
   insert(new Sll());
   insert(new Sllv());
   insert(new Sra());
   insert(new Srav());
   insert(new Srl());
   insert(new Srlv());
   insert(new Rol());
   insert(new Ror());
   insert(new Sub());
   insert(new Subu());
   insert(new Xor());
   insert(new Xori());

   // DataTransfer.H
   insert(new Lui());
   insert(new Li());
   insert(new La());
   insert(new Lw());
   insert(new Lwl());
   insert(new Lwr());
   insert(new Ld());
   insert(new Ulh());
   insert(new Ulhu());
   insert(new Ulw());
   insert(new Lb());
   insert(new Lbu());
   insert(new Lh());
   insert(new Lhu());
   insert(new Sw());
   insert(new Usw());
   insert(new Swl());
   insert(new Swr());
   insert(new Sd());
   insert(new Sb());
   insert(new Sh());
   insert(new Ush());
   insert(new Move());
   insert(new Movn());
   insert(new Movz());
   insert(new Mflo());
   insert(new Mfhi());
   insert(new Mtlo());
   insert(new Mthi());
   
   // Special.H
   insert(new Nop());
   insert(new Syscall());
   insert(new Done());
   
   // AssertInstr.H
   insert(new AssertEquals());
   
   // ControlFlow.H
   insert(new B());
   insert(new Beq());
   insert(new Bgez());
   insert(new Bgezal());
   insert(new Bgtz());
   insert(new Blez());
   insert(new Bltzal());
   insert(new Bltz());
   insert(new Bne());
   insert(new Beqz());
   insert(new Bge());
   insert(new Bgeu());
   insert(new Bgt());
   insert(new Bgtu());
   insert(new Ble());
   insert(new Bleu());
   insert(new Blt());
   insert(new Bltu());
   insert(new Bnez());
   insert(new J());
   insert(new Jal());
   insert(new Jalr());
   insert(new Jr());
   
   // Comparison.H
   insert(new Slt());
   insert(new Sltu());
   insert(new Slti());
   insert(new Sltiu());
   insert(new Seq());
   insert(new Sge());
   insert(new Sgeu());
   insert(new Sgt());
   insert(new Sgtu());
   insert(new Sle());
   insert(new Sleu());
   insert(new Sne());
   
   
   int i = 0; // temporary, to test parsing
   while(Instructions[i] != NULL) {// TODO:  remove eventually!
      if (!instructionMap.contains(Instructions[i])) 
         instructionMap.insert(Instructions[i], NULL/*TEMP*/);
      ++i;
   }
}

