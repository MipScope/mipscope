#include "Statement.H"
#include "StatementArgList.H"
#include "SyntaxList.H"
#include "typedefs.H"
#include <exception>

// Include all Statement Implementations
#include "Statements/Arithmetic.H"
#include "Statements/DataTransfer.H"
#include "Statements/Special.H"
#include "Statements/Assert.H"

Statement::Statement(unsigned int requiredSizeInBytes) 
   : m_size(requiredSizeInBytes)
{ }

unsigned int Statement::getSizeInBytes() const {
   return m_size;
}

void Statement::initialize(ParseNode *p, State *s) { p = NULL; s = NULL; }

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
   : Statement(4), m_args(args) // every instruction only takes a word of the address space
{ }

Instruction::~Instruction() { safeDelete(m_args); }

StatementArgList *Instruction::getArguments() const {
   return m_args;
}

bool Instruction::autoIncrementPC() const {
   return true;
}

InstructionMap instructionMap;
void Instruction::InitializeInstructionMap() {
   // TODO:  initialize global instructionMap declared extern in typedefs.h
   
   // temporary, to test parsing  
   int i = 0;
   while(Instructions[i] != NULL)
      instructionMap.insert(Instructions[i++], NULL/*TEMP*/);
   
   // Arithmetic.H
   instructionMap.insert("add", new Add());
   instructionMap.insert("addi", new Addi());

   // DataTransfer.H
   instructionMap.insert("li", new Li());
   instructionMap.insert("la", new La());
   instructionMap.insert("lw", new Lw());
   instructionMap.insert("sw", new Sw());
   
   // Special.H
   instructionMap.insert("nop", new Nop());
   instructionMap.insert("syscall", new Syscall());
   
   // Assert.H
   instructionMap.insert("assertequals", new AssertEquals());
   
}

