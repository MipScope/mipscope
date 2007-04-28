#include "Statement.H"
#include "StatementArgList.H"
#include "SyntaxList.H"
#include "typedefs.H"
#include <exception>

// Include all Statement Implementations
#include "statements/Arithmetic.H"
#include "statements/DataTransfer.H"
#include "statements/Special.H"
#include "statements/Assert.H"
#include "statements/ControlFlow.H"

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

void Instruction::insert(Instruction *instr) {
   instructionMap.insert(instr->getName(), instr);
}

InstructionMap instructionMap;
void Instruction::InitializeInstructionMap() {
   // Arithmetic.H
   insert(new Add());
   insert(new Addi());
   insert(new Sub());
   insert(new Div());
   insert(new Mult());
   insert(new Mul());

   // DataTransfer.H
   insert(new Li());
   insert(new La());
   insert(new Lw());
   insert(new Sw());
   insert(new Move());
   insert(new Mflo());
   insert(new Mfhi());
   
   // Special.H
   insert(new Nop());
   insert(new Syscall());
   
   // Assert.H
   insert(new AssertEquals());
   
   // ControlFlow.H
   insert(new J());
   
   
   int i = 0; // temporary, to test parsing
   while(Instructions[i] != NULL) {// TODO:  remove eventually!
      if (!instructionMap.contains(Instructions[i])) 
         instructionMap.insert(Instructions[i], NULL/*TEMP*/);
      ++i;
   }
}

