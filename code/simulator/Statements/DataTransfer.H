/* ---------------------------------------------- *\
   file: DataTransfer.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */

#ifndef __DATATRANSFER_H__
#define __DATATRANSFER_H__

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
      virtual const char *getSyntax() const { return "$dest, <immediate>"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to <immediate>.";
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
      virtual const char *getSyntax() const { return "$dest, <label>"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the memory address <label>";
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
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryWord((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lw"; }
      virtual const char *getSyntax() const { return "$dest, <addr>"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the value of the word stored at <addr>";
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
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryWord(m_args->getValue(s, 1), m_args->getValue(s, 0));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sw"; }
      virtual const char *getSyntax() const { return "$src, <addr>"; }
      virtual const char *getDescription() const {
         return "Stores the word in register $src into <addr>";
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
         s->setRegister(lo, (*m_args)[0]->getValue(s));
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
         s->setRegister(hi, (*m_args)[0]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mfhi"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in the special register HI";
      }
};

#endif // __DATATRANSFER_H__

