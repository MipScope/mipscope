/* ---------------------------------------------- *\
   file: DataTransfer.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */

#ifndef __DATATRANSFER_H__
#define __DATATRANSFER_H__

// ----------------------------------
// Instruction lui $dest, <constant>
// ----------------------------------
class Lui : public Instruction {
   public:
      Lui(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lui(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), ((*m_args)[1]->getValue(s) << 16 ));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lui"; }
      virtual const char *getSyntax() const { return "$dest, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the upper half-word of the register $dest to the lower half-word of &lt;immediate&gt;, filling in 0s for the lower bits.";
      }
};


// ----------------------------------
// Instruction li $dest, <constant>
// ----------------------------------
class Li : public Instruction {
   public:
      Li(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Li(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "li"; }
      virtual const char *getSyntax() const { return "$dest, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to &lt;immediate&gt;.";
      }
};

// ----------------------------------
// Instruction la $dest, <label>
// ----------------------------------
class La : public Instruction {
   public:
      La(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new La(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "la"; }
      virtual const char *getSyntax() const { return "$dest, &lt;label&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the memory address &lt;label&gt;";
      }
};

// ----------------------------------
// Instruction lw $dest, addr
// ----------------------------------
class Lw : public Instruction {
   public:
      Lw(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryWord((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lw"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the value of the word stored at &lt;addr&gt;";
      }
};

// ----------------------------------
// Instruction ln $dest, addr
// ----------------------------------
class Lb : public Instruction {
   public:
      Lb(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lb(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryByte((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lb"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the value of the word stored at &lt;addr&gt;";
      }
};

// ----------------------------------
// Instruction sw $src, addr
// ----------------------------------
class Sw : public Instruction {
   public:
      Sw(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryWord(m_args->getValue(s, 1), m_args->getValue(s, 0));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sw"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the word in register $src into &lt;addr&gt;";
      }
};

// ----------------------------------
// Instruction sb $src, addr
// ----------------------------------
class Sb : public Instruction {
   public:
      Sb(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sb(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryByte(m_args->getValue(s, 1), (char) (m_args->getValue(s, 0) & 255) );
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sb"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the least-significant byte of register $src into &lt;addr&gt;";
      }
};


// ----------------------------------
// Instruction move $dest, $src
// ----------------------------------
class Move : public Instruction {
   public:
      Move(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Move(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "move"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in register $src";
      }
};

// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mflo : public Instruction {
   public:
      Mflo(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mflo(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getRegister(lo));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mflo"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in the special register LO";
      }
};

// ----------------------------------
// Instruction mfhi $dest, $src
// ----------------------------------
class Mfhi : public Instruction {
   public:
      Mfhi(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mfhi(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getRegister(hi));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mfhi"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in the special register HI";
      }
};

// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mtlo : public Instruction {
   public:
      Mtlo(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mtlo(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister(lo, (*m_args)[0]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mtlo"; }
      virtual const char *getSyntax() const { return "$src"; }
      virtual const char *getDescription() const {
         return "Sets the special LO register to the value in the register (or immediate value) $src";
      }
};


// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mthi : public Instruction {
   public:
      Mthi(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Mthi(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister(hi, (*m_args)[0]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mthi"; }
      virtual const char *getSyntax() const { return "$src"; }
      virtual const char *getDescription() const {
         return "Sets the special HI register to the value in the register (or immediate value) $src";
      }
};

#endif // __DATATRANSFER_H__

