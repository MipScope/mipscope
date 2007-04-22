#include "ArithmeticStatement.H"
#include "StatementArgList.H"
#include "Identifier.H"
#include "State.H"

// ----------------------------------
// Instruction add $dest, $src1, src2
// ----------------------------------
Add::Add(StatementArgList *args) : Instruction(args) { }
bool Add::isSyntacticallyValid(StatementArgList *args) const {
   // means that 2 registers must be present, followed by 
   // a third register or immediate value (including constants)
   
   return (args->fitsRegisterImmediateType(2, 3));
}

void Add::execute(State *s) const {
   int val1 = (signed) s->getRegister((*m_args)[1]->getRegister());
   int val2;

   if ((*m_args)[2]->isRegister())
      val2 = (signed) s->getRegister((*m_args)[2]->getRegister());
   else val2 = (*m_args)[2]->getID()->getValue();
   
   s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 + val2));
}

Instruction *Add::create(StatementArgList *args) const { return new Add(args); }
const char *Add::getName() const { return "add"; }
const char *Add::getSyntax() const { return "$dest, $src1, src2"; }
const char *Add::getDescription() const {
   return "Adds two signed integers, storing the result in $dest.";
}




