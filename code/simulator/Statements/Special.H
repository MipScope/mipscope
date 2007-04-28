#ifndef SPECIAL_H_
#define SPECIAL_H_

// TODO: Break

// ----------------------------------
// Instruction nop
// ----------------------------------
class Nop : public Instruction {
   public:
      Nop(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(NONE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Nop(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {}
      
      // @overridden from Statement
      virtual const char *getName() const { return "nop"; }
      virtual const char *getSyntax() const { return "<void>"; }
      virtual const char *getDescription() const {
         return "Does nothing.";
      }
};


class Syscall : public Instruction {
   public:
      Syscall(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(NONE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Syscall(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->doSyscall();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "syscall"; }
      virtual const char *getSyntax() const { return "<void>"; }
      virtual const char *getDescription() const {
         return "Makes a system call to the operating system.";
      }
};




#endif /*SPECIAL_H_*/
