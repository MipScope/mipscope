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

      virtual void execute(State *s) const {
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

      virtual void execute(State *s) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "la"; }
      virtual const char *getSyntax() const { return "$dest, <label>"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the memory address <label>.";
      }
};


#endif // __DATATRANSFER_H__
