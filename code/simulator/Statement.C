#include "Statement.H"
#include "StatementArgList.H"
#include "SyntaxList.H"
#include "ArithmeticStatement.H"
#include <exception>
#include <iostream>
using namespace std;

Statement::Statement() { }

bool Statement::isInstruction() {
   Instruction *instr = NULL;

   try {
      instr = dynamic_cast<Instruction*>(this);
   } catch(bad_cast&) { }

   return (instr != NULL);
}

void Statement::InitializeStatementMaps() {
   Instruction::InitializeInstructionMap();
   Directive::InitializeDirectiveMap();
}

bool Statement::isDirective() {
   return (!isInstruction());
}

// ----------------------
// Instruction Superclass
// ----------------------
Instruction::Instruction(StatementArgList *args) 
   : Statement(), m_args(args)
{ }

Instruction::~Instruction() { safeDelete(m_args); }

InstructionMap instructionMap;
void Instruction::InitializeInstructionMap() {
   // TODO:  initialize global instructionMap declared extern in typedefs.h
   
   // temporary, to test parsing
   
   int i = 0;
   while(Instructions[i] != NULL)
      instructionMap.insert(Instructions[i++], NULL/*TEMP*/);

   instructionMap.insert("add", new Add());
}

// --------------------
// Directive Superclass
// --------------------
Directive::Directive() : Statement() { }

DirectiveMap directiveMap;
void Directive::InitializeDirectiveMap() {
   int i = 0;
   while(Directives[i] != NULL)
      directiveMap.insert(Directives[i++], NULL);
}

