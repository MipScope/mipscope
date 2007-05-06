#include "Statement.H"
#include "StatementArgList.H"
#include "SyntaxList.H"
#include "typedefs.H"
#include "ParseNode.H"
#include "State.H"
#include <exception>

// Include all Statement Implementations
#include "statements/Arithmetic.H"
#include "statements/DataTransfer.H"
#include "statements/Special.H"
#include "statements/Assert.H"
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
}

bool Statement::isInstruction() {
   Instruction *instr = NULL;

   try {
      instr = dynamic_cast<Instruction*>(this);
   } catch(bad_cast&) { }

   return (instr != NULL);
}

bool Statement::isDirective() {
   return (!isInstruction());
}

bool Statement::isPreprocessor() { // consts and #defines
   ConstStatement *constSt = NULL;

   try {
      constSt = dynamic_cast<ConstStatement*>(this);
   } catch(bad_cast&) { }

   return (constSt != NULL);
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

void Instruction::insert(Instruction *instr) {
   // try to ensure we didn't mess up w/ names of instructions..
   assert(!instructionMap.contains(instr->getName()));
   
   instructionMap.insert(instr->getName(), instr);
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
   insert(new Li());
   insert(new La());
   insert(new Lw());
   insert(new Lb());
   insert(new Sw());
   insert(new Sb());
   insert(new Move());
   insert(new Mflo());
   insert(new Mfhi());
   insert(new Mtlo());
   insert(new Mthi());
   
   // Special.H
   insert(new Nop());
   insert(new Syscall());
   insert(new Done());
   
   // Assert.H
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

