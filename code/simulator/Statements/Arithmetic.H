/* ---------------------------------------------- *\
   file: Arithmetic.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */
#ifndef __ARITHMETIC_H__
#define __ARITHMETIC_H__

#include "../Statement.H"
#include "../StatementArgList.H"
#include "../Identifier.H"
#include "../State.H"

// ----------------------------------
// Instruction add $dest, $src1, src2
// ----------------------------------
class Add : public Instruction {
   public:
      Add(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Add(args);
      }

      virtual void execute(State *s) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 + val2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "add"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2 OR <immediate>"; }
      virtual const char *getDescription() const {
         return "Adds two signed integers, storing the result in $dest.";
      }
};


// ----------------------------------
// Instruction addi $dest, $src1, CONSTANT
// We implement it as a type of Add.
// ----------------------------------
class Addi : public Add {
   public:
      Addi(StatementArgList *args = NULL) : Add(args) { }
      
      // @overridden from Add
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE );
      }
      
      // @overridden from Add
      virtual Instruction *create(StatementArgList *args) const {
         return new Addi(args);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "addi"; }
      virtual const char *getSyntax() const { return "$dest, $src1, <immediate>"; }
      virtual const char *getDescription() const {
         return "Adds two signed integers, storing the result in $dest.";
      }
};

#endif // __ARITHMETIC_H__

