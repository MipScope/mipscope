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

      virtual void execute(State *s, ParseList* parseList) const {
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

// ----------------------------------
// Instruction Sub $dest, $src1, src2
// ----------------------------------
class Sub : public Instruction {
   public:
      Sub(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sub(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 - val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "sub"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2 OR <immediate>"; }
      virtual const char *getDescription() const {
         return "Subtracts $src2 from $src1, storing the result in $dest.";
      }
};

// ----------------------------------
// Instruction Mult $dest, $src1, src2
// ----------------------------------
class Mult : public Instruction {
   public:
      Mult(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mult(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1);
         
         long long int result = val1 * val2; // at least 64 bit

         s->setRegister(lo , (result << 32) >> 32); // 32 least significant
         s->setRegister(hi , result >> 32); // 32 most significant
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "mult"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiplies $src1 by $src2, storing the 32 most-significant bits of the product in HI and the 32 least-significant bits in LO";
      }
};


// ----------------------------------
// Instruction Div $dest, $src1, src2
// ----------------------------------
class Div : public Instruction {
   public:
      Div(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Div(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1);

         s->setRegister(lo , (unsigned)(val1 / val2));
         s->setRegister(hi , (unsigned)(val1 % val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "div"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Divides $src1 by $src2, storing the integer quotient in LO and the remainder in HI";
      }
};

#endif // __ARITHMETIC_H__

