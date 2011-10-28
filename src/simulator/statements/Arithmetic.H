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

#include <iostream>

#define fabs(x)  ((signed)(x) >= 0 ? (signed)(x) : -(signed)(x))

/* only handles non-negative integers */
int pow(int base, int exponent) {
   int r = 1;
   for (int i = 0 ; i < exponent ; i++) r *= base;
   return r;
}
   

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
         
         if ( (qint64) val1 + (qint64) val2 != val1 + val2) 
             throw ArithmeticOverflow(s->getPC()->getTextBlock()); 

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 + val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "add"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Adds two integers (with overflow detection), storing the result in $dest.";
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
      
      virtual int importance() const { return 25; }

      // @overridden from Add
      virtual Instruction *create(StatementArgList *args) const {
         return new Addi(args);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "addi"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src3&gt;"; }
      virtual const char *getDescription() const {
         return "Adds two integers (with overflow detection), storing the result in $dest.";
      }
};

// ----------------------------------
// Instruction addu $dest, $src1, CONSTANT
// ----------------------------------
class Addu : public Instruction {
   public:
      Addu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Addu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);
         
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 + val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "addu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Adds two integers (without overflow detection), storing the result in $dest.";
      }
      
      
      
};

// Instruction addiu $dest, $src1, CONSTANT
// ----------------------------------
class Addiu : public Addu {
   public:
      Addiu(StatementArgList *args = NULL) : Addu(args) { }      
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE );
      }
      
      virtual int importance() const { return 25; }

      // @overridden from Addu
      virtual Instruction *create(StatementArgList *args) const {
         return new Addiu(args);
      }      
      // @overridden from Statement
      virtual const char *getName() const { return "addiu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt;"; }
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
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Clo(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         
         int count;
         for (count = 0 ; val >> 31 ; count++, val <<= 1);
            
         s->setRegister((*m_args)[0]->getRegister(), count);
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

      virtual int importance() const { return 25; }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         
         int count;
         for (count = 0 ; !(val >> 31) && count < 32 ; count++, val <<= 1);
            
         s->setRegister((*m_args)[0]->getRegister(), count);
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
            throw DivideByZero(s->getPC()->getTextBlock());
            
         if (m_args->noArgs() == 2) {
            s->setRegister(lo , numerator / denominator);
            s->setRegister(hi , numerator % denominator);
         }
         else {
            s->setRegister((*m_args)[0]->getRegister(), numerator / denominator);
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "div"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt; OR div $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Divides signed integers $src1 by &lt;src2&gt;, storing the result in $dest. OR: divides $src1 by $src2, storing the integer quotient in LO and the remainder in HI.";
      }
};

// ----------------------------------
// Instruction Divu $dest, $src1, src2
// ----------------------------------
class Divu : public Instruction {
   public:
      Divu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE | NONE);
      }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Divu(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         /* There are two syntaxes for Divu:
          * divu $src1, $src2 -- store $src1 / $src2 in LO, $src1 % $src2 in HI
          * divu $dest, $src1, $src2 -- store $src1 / $src2 in $dest
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
            throw DivideByZero(s->getPC()->getTextBlock());
            
         if (m_args->noArgs() == 2) {
            s->setRegister(lo , ( (unsigned) numerator) / ( (unsigned) denominator));
            s->setRegister(hi , ( (unsigned) numerator) % ( (unsigned) denominator));
         }
         else {   
            s->setRegister((*m_args)[0]->getRegister(), ( (unsigned) numerator) / ( (unsigned) denominator));
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "divu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt; OR divu $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Divides unsigned integers in $src1 by &lt;src2&gt;, storing the result in $dest. OR: divides $src1 by $src2, storing the integer quotient in LO and the remainder in HI.";
      }
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
         
         qint64 result = (qint64) val1 * (qint64) val2; // at least 64 bit
         
         s->setRegister(lo, result); // 32 least significant
         s->setRegister(hi, result >> 32); // 32 most significant
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "mult"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiplies signed integers in $src1 by $src2, storing the 32 most-significant bits of the product in HI and the 32 least-significant bits in LO";
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
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Multu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 0);
         unsigned int val2 = m_args->getValue(s, 1);
         
         quint64 result = (quint64) val1 * (quint64) val2; // at least 64 bit
         
         s->setRegister(lo , result); // 32 least significant
         s->setRegister(hi , result >> 32); // 32 most significant
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "multu"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiplies unsigned integers in $src1 by $src2, storing the 32 most-significant bits of the product in HI and the 32 least-significant bits in LO";
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
         return "Multiplies signed integers in $src1 by &lt;src2&gt;, storing the result in $dest.";
      }
};

// -----------------------------------
// Instruction Mulo $dest, $src1, src2
// -----------------------------------
class Mulo : public Mul {
   public:
      Mulo(StatementArgList *args = NULL) : Mul(args) { }
            
      virtual Instruction *create(StatementArgList *args) const {
         return new Mulo(args);
      }

      virtual int importance() const { return 25; }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);
         
         if ( (qint64) val1 * (qint64) val2 != val1 * val2)
            throw ArithmeticOverflow(s->getPC()->getTextBlock()); 

         s->setRegister((*m_args)[0]->getRegister(), val1 * val2);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mulo"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Multiplies, with overflow detection, signed integers in $src1 by &lt;src2&gt;, storing the result in $dest.";
      }
};

// -----------------------------------
// Instruction mulou $dest, $src1, src2
// -----------------------------------
class Mulou : public Mul {
   public:
      Mulou(StatementArgList *args = NULL) : Mul(args) { }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Mulou(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);
         
         quint64 result_64 = (quint64) (unsigned int) val1 * (quint64) (unsigned int) val2;
                  
         if ( result_64 != (unsigned int) result_64)
            throw ArithmeticOverflow(s->getPC()->getTextBlock());

         s->setRegister((*m_args)[0]->getRegister(), (int) result_64);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mulou"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Multiplies, with overflow detection, unsigned integers in $src1 by &lt;src2&gt;, storing the result in $dest.";
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
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Madd(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1);        
         
         qint64 sum = (((qint64) (unsigned int) s->getRegister(hi)) << 32) | (qint64) ((unsigned int) s->getRegister(lo));
         sum += (qint64) val1 * (qint64) val2;
         s->setRegister(hi, sum >> 32);
         s->setRegister(lo, sum);
         
         //cerr << "Sum: " << std::hex << sum << "\n";
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "madd"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiply registers $src1 and $src2, and add the resulting 64-bit product to the 64-bit value in the concatenated registers LO and HI.";
      }
};


// -----------------------------------
// Instruction maddu $src1, $src2
// -----------------------------------
class Maddu : public Instruction {
   public:
      Maddu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Maddu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 0);
         unsigned int val2 = m_args->getValue(s, 1);        
         
         quint64 sum = (((quint64) (unsigned int) s->getRegister(hi)) << 32) | (quint64) ((unsigned int) s->getRegister(lo));
         
         //cerr << "Before Sum: " << std::hex << sum << "\n";
         
         sum += (quint64) val1 * (quint64) val2;
         s->setRegister(hi, sum >> 32);
         s->setRegister(lo, sum);
         
         //cerr << "After Sum: " << std::hex << sum << "\n";
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "maddu"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiply unsigned registers $src1 and $src2, and add the resulting 64-bit product to the 64-bit value in the concatenated registers LO and HI.";
      }
};

// -----------------------------------
// Instruction msub $src1, $src2
// -----------------------------------
class Msub : public Instruction {
   public:
      Msub(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Msub(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1); 
                   
         
         qint64 sum = (((qint64) (unsigned int) s->getRegister(hi)) << 32) | (qint64) ((unsigned int) s->getRegister(lo));
         
         //cerr << "Before msub Sum: " << std::hex << sum << "\n";    
         
         sum -= (qint64) val1 * (qint64) val2;
         s->setRegister(hi, (unsigned int) (sum >> 32));
         s->setRegister(lo, sum);
         
         //cerr << "After msub Sum: " << std::hex << sum << "\n";
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "msub"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiply registers $src1 and $src2, and subtract the resulting 64-bit product from the 64-bit value in the concatenated registers LO and HI.";
      }
};


// -----------------------------------
// Instruction maddu $src1, $src2
// -----------------------------------
class Msubu : public Instruction {
   public:
      Msubu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 25; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Msubu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 0);
         unsigned int val2 = m_args->getValue(s, 1);        
         
         quint64 sum = (((quint64) (unsigned int) s->getRegister(hi)) << 32) | (quint64) ((unsigned int) s->getRegister(lo));
         
         //cerr << "Before Sum: " << std::hex << sum << "\n";
         
         sum -= (quint64) val1 * (quint64) val2;
         s->setRegister(hi, sum >> 32);
         s->setRegister(lo, sum);
         
         //cerr << "After Sum: " << std::hex << sum << "\n";
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "msubu"; }
      virtual const char *getSyntax() const { return "$src1, $src2"; }
      virtual const char *getDescription() const {
         return "Multiply unsigned registers $src1 and $src2, and subtract the resulting 64-bit product from the 64-bit value in the concatenated registers LO and HI.";
      }
};


// ----------------------------------
// Instruction neg $dest, $src1
// ----------------------------------
class Neg : public Instruction {
   public:
      Neg(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Neg(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
         
         // this is the only value that doesn't have a negative:
         if (val == (signed long long)-2147483648LL) throw ArithmeticOverflow(s->getPC()->getTextBlock());
         
         s->setRegister((*m_args)[0]->getRegister(),  -1 * val);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "neg"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores in signed register $dest the value (-1 * $src), with overflow detection";
      }
};

// ----------------------------------
// Instruction add $dest, $src1
// ----------------------------------
class Negu : public Instruction {
   public:
      Negu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Negu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
                
         s->setRegister((*m_args)[0]->getRegister(),  -1 * val);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "negu"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores in signed register $dest the value (-1 * $src), without overflow detection";
      }
};

// ----------------------------------
// Instruction nor $dest, $src1, src2
// ----------------------------------
class Nor : public Instruction {
   public:
      Nor(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Nor(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), ~(val1 | val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "nor"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of NOR'ing the bits of $src1 and &lt;src2&gt;.";
      }
};

// ----------------------------------
// Instruction not $dest, $src1
// ----------------------------------
class Not : public Instruction {
   public:
      Not(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Not(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val = m_args->getValue(s, 1);
                
         s->setRegister((*m_args)[0]->getRegister(),  ~val);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "not"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Stores in register $dest the bitwise logical negation of the register $src";
      }
};


// ----------------------------------
// Instruction or $dest, $src1, src2
// ----------------------------------
class Or : public Instruction {
   public:
      Or(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Or(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 1);
         unsigned int val2 = m_args->getValue(s, 2);
         
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 | val2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "or"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the logical OR of $src1 and &lt;src2&gt;.";
      }
};

// ----------------------------------
// Instruction ori $dest, $src1, imm
// ----------------------------------
class Ori : public Or {
   public:
      Ori(StatementArgList *args = NULL) : Or(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ori(args);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "ori"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the logical OR of $src1 and &lt;immediate&gt;.";
      }
};



// ----------------------------------
// Instruction rem $dest, $src1, src2
// ----------------------------------
class Rem : public Instruction {
   public:
      Rem(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Rem(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), val1 % val2 );
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "rem"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the remainder of the signed register $src1 divided by &lt;src2&gt;";
      }
};


// ----------------------------------
// Instruction remu $dest, $src1, src2
// ----------------------------------
class Remu : public Instruction {
   public:
      Remu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Remu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val1 = m_args->getValue(s, 1);
         unsigned int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), val1 % val2);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "remu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the remainder of the unsigned register $src1 divided by &lt;src2&gt;";
      }
};


// ----------------------------------
// Instruction sll $dest, $src1, src2
// ----------------------------------
class Sll : public Instruction {
   public:
      Sll(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE ) && args->getValue(NULL, 2) <= 31 && args->getValue(NULL, 2) >= 0;
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sll(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int value = m_args->getValue(s, 1);
         unsigned int shiftAmount = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), value << shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "sll"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt; (between 0 and 31)"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting the bits of $src1 &lt;immediate&gt; places to the left, inserting zeros into the empty bits";
      }
};


// ----------------------------------
// Instruction sllv $dest, $src1, src2
// ----------------------------------
class Sllv : public Instruction {
   public:
      Sllv(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sllv(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int value = m_args->getValue(s, 1);
         unsigned int shiftAmount = m_args->getValue(s, 2) & (0x1F); // use only low-order 5 bits

         s->setRegister((*m_args)[0]->getRegister(), value << shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "sllv"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting to the left the bits of $src1 by an amount given by the low-order 5 bits of the register $src2, inserting zeros for the empty bits";
      }
};


// ----------------------------------
// Instruction srl $dest, $src1, src2
// ----------------------------------
class Srl : public Instruction {
   public:
      Srl(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE ) && args->getValue(NULL, 2) <= 31 && args->getValue(NULL, 2) >= 0;
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Srl(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int value = m_args->getValue(s, 1);
         unsigned int shiftAmount = m_args->getValue(s, 2);
         
         s->setRegister((*m_args)[0]->getRegister(), value >> shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "srl"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt; (between 0 and 31)"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting the bits of $src1 &lt;immediate&gt; places to the right, inserting zeros into the empty bits";
      }
};

// ----------------------------------
// Instruction srlv $dest, $src1, src2
// ----------------------------------
class Srlv : public Instruction {
   public:
      Srlv(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Srlv(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int value = m_args->getValue(s, 1);
         unsigned int shiftAmount = m_args->getValue(s, 2) & (0x1F); // use only low-order 5 bits

         s->setRegister((*m_args)[0]->getRegister(), value >> shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "srlv"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting to the right the bits of $src1 by an amount given by the low-order 5 bits of the register $src2, inserting zeros for the empty bits";
      }
};


// ----------------------------------
// Instruction srl $dest, $src1, src2
// ----------------------------------
class Sra : public Instruction {
   public:
      Sra(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE ) && args->getValue(NULL, 2) <= 31 && args->getValue(NULL, 2) >= 0;
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sra(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int value = m_args->getValue(s, 1);
         int shiftAmount = m_args->getValue(s, 2);
         
         s->setRegister((*m_args)[0]->getRegister(), value >> shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "sra"; }
      virtual const char *getSyntax() const { return "$dest, $src, &lt;immediate&gt; (between 0 and 31)"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting $src &lt;immediate&gt; places to the right, inserting zeros if $src >= 0 and ones if $src < 0";
      }
};

// ----------------------------------
// Instruction srav $dest, $src1, src2
// ----------------------------------
class Srav : public Instruction {
   public:
      Srav(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Srav(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int value = m_args->getValue(s, 1);
         int shiftAmount = m_args->getValue(s, 2) & (0x1F); // use only low-order 5 bits

         s->setRegister((*m_args)[0]->getRegister(), value >> shiftAmount);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "srav"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of shifting to the right $src1 by an amount given by the low-order 5 bits of the register $src2, inserting zeros if $src >= 0 and ones if $src < 0";
      }
};


// ----------------------------------
// Instruction rol $dest, $src1, src2
// ----------------------------------
class Rol : public Instruction {
   public:
      Rol(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Rol(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int value = m_args->getValue(s, 1);
         int shiftAmount = m_args->getValue(s, 2) & (0x1F); // use only low-order 5 bits

         s->setRegister((*m_args)[0]->getRegister(), (value << shiftAmount) | ( (value >> (32 - shiftAmount)) & (pow(2, shiftAmount) - 1)));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "rol"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of rotating to the left $src1 by an amount given by the low-order 5 bits of the register $src2";
      }
};


// ----------------------------------
// Instruction ror $dest, $src1, src2
// ----------------------------------
class Ror : public Instruction {
   public:
      Ror(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ror(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int value = m_args->getValue(s, 1);
         unsigned int shiftAmount = m_args->getValue(s, 2) & (0x1F); // use only low-order 5 bits

         s->setRegister((*m_args)[0]->getRegister(), (value >> shiftAmount) | ( (value << (32 - shiftAmount))));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "ror"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of rotating to the right $src1 by an amount given by the low-order 5 bits of the register $src2";
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
         
         if ( (qint64) val1 - (qint64) val2 != val1 - val2) 
            throw ArithmeticOverflow(s->getPC()->getTextBlock()); 

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 - val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "sub"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Subtracts (with overflow detection) &lt;src2&gt; from $src1, storing the result in $dest.";
      }
};


// ----------------------------------
// Instruction Sub $dest, $src1, src2
// ----------------------------------
class Subu : public Instruction {
   public:
      Subu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Subu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);
         
         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 - val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "subu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Subtracts (without overflow detection) &lt;src2&gt; from $src1, storing the result in $dest.";
      }
};


// ----------------------------------
// Instruction xor $dest, $src1, src2
// ----------------------------------
class Xor : public Instruction {
   public:
      Xor(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE );
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Xor(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 1);
         int val2 = m_args->getValue(s, 2);

         s->setRegister((*m_args)[0]->getRegister(), (unsigned)(val1 ^ val2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "xor"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;src2&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of XOR'ing the bits of $src1 and &lt;src2&gt;.";
      }
};


// ----------------------------------
// Instruction xor $dest, $src1, src2
// ----------------------------------
class Xori : public Xor {
   public:
      Xori(StatementArgList *args = NULL) : Xor(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, IMMEDIATE );
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Xori(args);
      }

      // @overridden from Statement
      
      virtual const char *getName() const { return "xori"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;IMMEDIATE&gt;"; }
      virtual const char *getDescription() const {
         return "Stores in $dest the result of XOR'ing the bits of $src1 and &lt;IMMEDIATE&gt;.";
      }
};






#endif // __ARITHMETIC_H__

