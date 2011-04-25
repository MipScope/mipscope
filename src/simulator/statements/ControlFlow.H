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

#ifndef CONTROLFLOW_H_
#define CONTROLFLOW_H_

#include "../Statement.H"
#include "../StatementArgList.H"
#include "../Identifier.H"
#include "../State.H"
#include "../ParseList.H"

// reused a lot
#define LINK_DESCRIPTION  " Save the address of the next instruction in $ra"

// ----------------------------------
// Instruction j CONST
// ----------------------------------
class J : public Instruction {
   public:
      J(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new J(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         //cerr << "addr: " << m_args->getValue(s, 0) << ", ";
         //cerr << (void*)parseList->getNodeForAddress(m_args->getValue(s, 0)) << endl;
         //cerr << parseList->getNodeForAddress(m_args->getValue(s, 0)) << endl;

         s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 0)));
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      // @overridden from Statement
      virtual const char *getName() const { return "j"; }
      virtual const char *getSyntax() const { return "&lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to &lt;address&gt;";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// ----------------------------------
// Instruction jal target
// ----------------------------------
class Jal : public Instruction {
   public:
      Jal(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER | IMMEDIATE | ADDRESS);
      }
      
      virtual int importance() const { return 90; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Jal(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         link(s); // store the addr of the next instruction in $ra
         //cerr << parseList->getNodeForAddress(m_args->getValue(s, 0)) << endl;

         s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 0)));
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      virtual const char *getName() const { return "jal"; }
      virtual const char *getSyntax() const { return "&lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to &lt;address&gt;."LINK_DESCRIPTION;
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// ----------------------------------
// Instruction jalr $dest, $instr
// ----------------------------------

/*
 * IMPORTANT: Order of arguments to jalr
 *
 * There is a *lot* of confusion about the correct order of arguments to this
 * instruction.  Even Hennessy and Patterson got it wrong.  The correct order
 * (according to the MIPS specification, which gcc follows) is
 *
 *    jalr rd, rs
 *
 * where rs is the address of the instruction to execute, and rd is the register
 * in which to store the next instruction.  This order makes no sense, but it is
 * correct.
 *
 * MipScope used to do it the wrong way.  Spim probably does it the wrong way too.
 * DO NOT CHANGE THE ORDER BACK, REGARDLESS OF WHAT THE INTERNET OR HENNESSY AND
 * PATTERSON SAYS!  IT IS CORRECT NOW!
 *
 * If only one argument is specified like this:
 *
 *	jalr rs
 *
 * Then rd defaults to $ra
 *
 *  -- andrew (20 Nov 2010)
 */
class Jalr : public Instruction {
   public:
      Jalr(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | NONE);
      }
      
      virtual int importance() const { return 90; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Jalr(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         // Do not change the order of arguments -- it is correct!
         // (see comment above this class for details)
         unsigned int linkRegister;
	 unsigned int jumpDestination;
         if (m_args->noArgs() == 2) {
		 linkRegister = (*m_args)[0]->getRegister();
		 jumpDestination = m_args->getValue(s, 1);
	 } else {
		 // Link register not specified - default to $ra
		 linkRegister = ra;
		 jumpDestination = m_args->getValue(s, 0);
	 }
         
         link(s, linkRegister); // store the addr of the next instruction
         s->setPC(parseList->getNodeForAddress(jumpDestination));
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      virtual const char *getName() const { return "jalr"; }
      // Do not change the order of arguments -- it is correct!
      // (see comment above this class for details)
      virtual const char *getSyntax() const { return "$link, $dest OR $link"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to the address stored in register $dest. Save the address of the next instruction in register $link (default $ra).";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// ----------------------------------
// Instruction jr $dest
// ----------------------------------
class Jr : public Instruction {
   public:
      Jr(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 80; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Jr(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
//         cerr << "Jr '" << m_args->getValue(s, 0) << "'\n";
//         cerr << "Jr '" << (*m_args)[0]->getRegister() << "'\n";
         // TODO:  DEBUG!!

         
         s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 0)));
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      virtual const char *getName() const { return "jr"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to the address stored in register $dest";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};


// ----------------------------------
// Instruction b address
// ----------------------------------
class B : public J {
   public:
      B(StatementArgList *args = NULL) : J(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new B(args);
      }
      
      virtual int importance() const { return 50; }

      virtual const char *getName() const { return "b"; }
};

// -------------------------------------
// Instruction bgez $src, address
// -------------------------------------
class Bgez : public Instruction {
   public:
      Bgez(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgez(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);

         if (val0 >= 0)
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bgez"; }
      virtual const char *getSyntax() const { return "$src1, &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than or equal to zero";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bgezal $src, address
// -------------------------------------
class Bgezal : public Bgez {
   public:
      Bgezal(StatementArgList *args = NULL) : Bgez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgezal(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         if (val0 >= 0) {
            link(s); // stores the addr of the next instruction into $ra
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         } else {
            s->incrementPC();
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bgezal"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than or equal to zero."LINK_DESCRIPTION;
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bgtz $src, address
// -------------------------------------
class Bgtz : public Bgez {
   public:
      Bgtz(StatementArgList *args = NULL) : Bgez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgtz(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);

         if (val0 > 0) // only change from Bgez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bgtz"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than zero";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction blez $src, address
// -------------------------------------
class Blez : public Bgez {
   public:
      Blez(StatementArgList *args = NULL) : Bgez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Blez(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);

         if (val0 <= 0) // only change from Bgez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "blez"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than or equal to zero";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bltzal $src, address
// -------------------------------------
class Bltzal : public Blez {
   public:
      Bltzal(StatementArgList *args = NULL) : Blez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bltzal(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         if (val0 < 0) { // only change from Blez
            link(s); // stores the addr of the next instruction into $ra
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         } else {
            s->incrementPC();
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bltzal"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than zero."LINK_DESCRIPTION;
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bltz $src, address
// -------------------------------------
class Bltz : public Blez {
   public:
      Bltz(StatementArgList *args = NULL) : Blez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bltz(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         if (val0 < 0) // only change from Blez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bltz"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than zero.";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bne $src1, src2 address
// -------------------------------------
class Bne : public Instruction {
   public:
      Bne(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bne(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         int val1 = m_args->getValue(s, 1);
         
         if (val0 != val1)
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bne"; }
      virtual const char *getSyntax() const { return "$src1, src2, &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is not equal to src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction beq $src1, src2 address
// -------------------------------------
class Beq : public Bne {
   public:
      Beq(StatementArgList *args = NULL) : Bne(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Beq(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         int val1 = m_args->getValue(s, 1);
         
         if (val0 == val1)
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "beq"; }
      virtual const char *getSyntax() const { return "$src1, src2, &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is equal to src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};


// -------------------------------------
// Instruction beqz $src, address
// -------------------------------------
class Beqz : public Blez {
   public:
      Beqz(StatementArgList *args = NULL) : Blez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Beqz(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         if (val0 == 0) // only change from Blez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "beqz"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 equal to zero.";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bge $src1, src2 address
// -------------------------------------
class Bge : public Bne {
   public:
      Bge(StatementArgList *args = NULL) : Bne(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bge(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         int val1 = m_args->getValue(s, 1);
         
         if (val0 >= val1) // only change from Bne
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bge"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than or equal to src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bgeu $src1, src2 address
// -------------------------------------
class Bgeu : public Bge {
   public:
      Bgeu(StatementArgList *args = NULL) : Bge(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgeu(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val0 = m_args->getValue(s, 0); // note: unsigned important
         unsigned int val1 = m_args->getValue(s, 1);
         
         if (val0 >= val1) // only change from Bne
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "bgeu"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than or equal to src2 (unsigned)";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bgt $src1, src2 address
// -------------------------------------
class Bgt : public Bne {
   public:
      Bgt(StatementArgList *args = NULL) : Bne(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgt(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         int val1 = m_args->getValue(s, 1);
         
         if (val0 > val1) // only change from Bne
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "bgt"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bgtu $src1, src2 address
// -------------------------------------
class Bgtu : public Bgt {
   public:
      Bgtu(StatementArgList *args = NULL) : Bgt(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bgtu(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val0 = m_args->getValue(s, 0); // note: unsigned important
         unsigned int val1 = m_args->getValue(s, 1);
         
         if (val0 > val1) // only change from Bgt
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "bgtu"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than src2 (unsigned)";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction ble $src1, src2 address
// -------------------------------------
class Ble : public Bne {
   public:
      Ble(StatementArgList *args = NULL) : Bne(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Ble(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0); // note: signed important
         int val1 = m_args->getValue(s, 1);
         
         if (val0 <= val1) // only change from Bne
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "ble"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than or equal to src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bleu $src1, src2 address
// -------------------------------------
class Bleu : public Ble {
   public:
      Bleu(StatementArgList *args = NULL) : Ble(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bleu(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val0 = m_args->getValue(s, 0); // note: unsigned important
         unsigned int val1 = m_args->getValue(s, 1);
         
         if (val0 <= val1) // only change from Ble
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "bleu"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than or equal to src2 (unsigned)";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction blt $src1, src2 address
// -------------------------------------
class Blt : public Bne {
   public:
      Blt(StatementArgList *args = NULL) : Bne(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Blt(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0); // note: signed important
         int val1 = m_args->getValue(s, 1);
         
         if (val0 < val1) // only change from Bne
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "blt"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than src2";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bltu $src1, src2 address
// -------------------------------------
class Bltu : public Blt {
   public:
      Bltu(StatementArgList *args = NULL) : Blt(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bltu(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int val0 = m_args->getValue(s, 0); // note: unsigned is important
         unsigned int val1 = m_args->getValue(s, 1);
         
         if (val0 < val1) // only change from Blt
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 2)));
         else s->incrementPC();
      }
      
      virtual const char *getName() const { return "bltu"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than src2 (unsigned)";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

// -------------------------------------
// Instruction bnez $src, address
// -------------------------------------
class Bnez : public Bgez {
   public:
      Bnez(StatementArgList *args = NULL) : Bgez(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Bnez(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);

         if (val0 != 0) // only change from Bnez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bnez"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is not equal to zero";
      }

      virtual int getType() const { return CONTROL_FLOW; }
};

#endif // CONTROLFLOW_H_

