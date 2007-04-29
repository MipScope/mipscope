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
};

// ----------------------------------
// Instruction jal target
// ----------------------------------
class Jal : public Instruction {
   public:
      Jal(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(ADDRESS | IMMEDIATE);
      }
      
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
};

// ----------------------------------
// Instruction jalr $dest, $instr
// ----------------------------------
class Jalr : public Instruction {
   public:
      Jalr(StatementArgList *args = NULL) : Instruction(args) { }
      
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Jalr(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         unsigned int registerNo = (*m_args)[1]->getRegister();
         
         link(s, registerNo); // store the addr of the next instruction
         s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 0)));
      }
      
      virtual bool autoIncrementPC() const { return false; }
      
      virtual const char *getName() const { return "jalr"; }
      virtual const char *getSyntax() const { return "$dest, $instr"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to the address stored in register $dest. Save the address of the next instruction in register $instr.";
      }
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
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         link(s); // stores the addr of the next instruction into $ra
         
         if (val0 >= 0)
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bgezal"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is greater than or equal to zero."LINK_DESCRIPTION;
      }
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
      
      virtual void execute(State *s, ParseList* parseList) const {
         int val0 = m_args->getValue(s, 0);
         
         link(s); // stores the addr of the next instruction into $ra
         
         if (val0 < 0) // only change from Blez
            s->setPC(parseList->getNodeForAddress(m_args->getValue(s, 1)));
         else s->incrementPC();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "bltzal"; }
      virtual const char *getDescription() const {
         return "Conditionally branch to &lt;address&gt; if $src1 is less than zero."LINK_DESCRIPTION;
      }
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
};

#endif // CONTROLFLOW_H_

