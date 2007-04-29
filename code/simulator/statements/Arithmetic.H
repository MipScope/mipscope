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
#include "../StateException.H"

//#include <cmath>  // want to keep as cross-platform and portable as possible..
#define fabs(x)  ((signed)(x) >= 0 ? (signed)(x) : -(signed)(x))

// ----------------------------------
// Instruction add $dest, $src1
// ----------------------------------
class Abs : public Instruction {
   public:
      Abs(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Abs(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)( fabs(val) ));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "abs"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores the absolute value of register $src in register $dest.";
      }
};

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
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Adds two integers, storing the result in $dest.";
      }
};


// ----------------------------------
// Instruction addi $dest, $src1, src2
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
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src3&gt;"; }
      virtual const char *getDescription() const {
         return "Adds two integers, storing the result in $dest.";
      }
};

// ----------------------------------
// Instruction addu $dest, $src1, CONSTANT
// We implement it as a type of Add.
// ----------------------------------
class Addu : public Add {
   public:
      Addu(StatementArgList *args = NULL) : Add(args) { }
      
      // @overridden from Add
      virtual Instruction *create(StatementArgList *args) const {
         return new Addu(args);
      }      
      // @overridden from Statement
      virtual const char *getName() const { return "addu"; }
};

// Instruction addiu $dest, $src1, CONSTANT
// We implement it as a type of Addi.
// ----------------------------------
class Addiu : public Addi {
   public:
      Addiu(StatementArgList *args = NULL) : Addi(args) { }
      
      // @overridden from Add
      virtual Instruction *create(StatementArgList *args) const {
         return new Addiu(args);
      }      
      // @overridden from Statement
      virtual const char *getName() const { return "addiu"; }
};

// ----------------------------------
// Instruction and $dest, $src1, src2
// ----------------------------------
class And : public Instruction {
   public:
      And(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new And(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 & val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "and"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of AND'ing the bits of $src1 and &lt;src2&gt;.";
      }
};

// ----------------------------------
// Instruction andi $dest, $src1, src2
// ----------------------------------
class Andi : public And {
   public:
      Andi(StatementArgList *args = NULL) : And(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Andi(args);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "andi"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;constant&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of AND'ing the bits of $src1 and &lt;constant&gt;.";
      }
};

// ----------------------------------
// Instruction clo $dest, $src1
// ----------------------------------
class Clo : public Instruction {
   public:
      Clo(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Clo(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         
         for (int count = 0 ; (val << 1) >> 1 != val ; count++, val <<= 1);
            
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)( fabs(val) ));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "clo"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the number of leading 1's in $src.";
      }
};

// ----------------------------------
// Instruction clo $dest, $src1
// ----------------------------------
class Clz : public Instruction {
   public:
      Clz(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Clz(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         
         for (int count = 0 ; (val << 1) >> 1 == val ; count++, val <<= 1);
            
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)( fabs(val) ));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "clz"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the number of leading 0's in $src.";
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
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE | NONE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Div(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         
         /* There are two syntaxes for Div:
          * div $src1, $src2 -- store $src1 / $src2 in LO, $src1 % $src2 in HI
          * div $dest, $src1, $src2 -- store $src1 / $src2 in $dest
          */
         
         int numerator, denominator;
         if (m_args->noArgs() == 2) {
            numerator = m_args->getValue(s, 0);
            denominator = m_args->getValue(s, 1);
         }
         else {
            numerator = m_args->getValue(s, 1);
            denominator = m_args->getValue(s, 2);
         }
         
         if (denominator == 0)
            throw DivideByZero();
            
         if (m_args->noArgs() == 2) {
            s->setRegister(lo , (unsigned)(numerator / denominator));
            s->setRegister(hi , (unsigned)(numerator % denominator));
         }
         else {
            s->setRegister((*m_args)[0]->getRegister(), numerator / denominator);
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "div"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt; OR div $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Divides $src1 by &lt;src2&gt;, storing the result in $dest. OR: divides $src1 by $src2, storing the integer quotient in LO and the remainder in HI.";
      }
};

// ----------------------------------
// Instruction Divu $dest, $src1, src2
// ----------------------------------
class Divu : public Div {
   public:
      Divu(StatementArgList *args = NULL) : Div(args) { }
      
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Divu(args);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "divu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt; OR divu $src1, $src2"; }
      
};

// ----------------------------------
// Instruction Mult $src1, src2
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
         return "Multiplies (signed) $src1 by $src2, storing the 32 most-significant bits of the product in HI and the 32 least-significant bits in LO";
      }
};


// ----------------------------------
// Instruction Multu $src1, src2
// ----------------------------------
class Multu : public Instruction {
   public:
      Multu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Multu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 0);
         unsigned int val2 = m_args->getValue(s, 1);
         
         long long unsigned int result = val1 * val2; //  64 bit
         
         s->setRegister(lo , (result << 32) >> 32); // 32 least significant
         s->setRegister(hi , result >> 32); // 32 most significant
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "multu"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiplies (unsigned) $src1 by $src2, storing the 32 most-significant bits of the product in HI and the 32 least-significant bits in LO";
      }
};

// ----------------------------------
// Instruction Mul $dest, $src1, src2
// ----------------------------------
class Mul : public Instruction {
   public:
      Mul(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mul(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), val1 * val2);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mul"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Multiplies (signed) $src1 by &lt;src2&gt;, storing the result in $dest.";
      }
};

// -----------------------------------
// Instruction Mulo $dest, $src1, src2
// -----------------------------------
class Mulo : public Mul {
   public:
      Mulo(StatementArgList *args = NULL) : Mul(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mulo(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), val1 * val2);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mulo"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Multiplies (signed) $src1 by &lt;src2&gt;, storing the result in $dest.";
      }
};

// -----------------------------------
// Instruction mulou $dest, $src1, src2
// -----------------------------------
class Mulou : public Mul {
   public:
      Mulou(StatementArgList *args = NULL) : Mul(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mulou(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 1);
         unsigned int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), val1 * val2);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mulou"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Multiplies (unsigned) $src1 by &lt;src2&gt;, storing the result in $dest.";
      }
};

// -----------------------------------
// Instruction madd $src1, $src2
// -----------------------------------
class Madd : public Instruction {
   public:
      Madd(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Madd(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);
         
         long long int sum = (((long long int) s->getRegister(hi)) << 32) + s->getRegister(lo);
         sum += val1 * val2;
         s->setRegister(hi, (unsigned)( sum >> 32 ));
         s->setRegister(lo, ((unsigned long long int)( sum << 32 )) >> 32);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "madd"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiply registers $src1 and $src2, and add the resulting 64-bit product to the 64-bit value in the concatenated registers LO and HI.";
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
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Subtracts &lt;src2&gt; from $src1, storing the result in $dest.";
      }
};







#endif // __ARITHMETIC_H__

