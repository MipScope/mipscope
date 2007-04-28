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
      virtual const char *getSyntax() const { return ""; }
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
      virtual const char *getSyntax() const { return ""; }
      virtual const char *getDescription() const {
         return "System call to the operating system; $v0 holds syscall number.";
      }
};

// ----------------------------------
// Instruction done
// ----------------------------------
Done::Done(StatementArgList *args) : Instruction(args) { }

bool Done::isSyntacticallyValid(StatementArgList *args) const {
   return args->matchesSyntax(NONE);
}

Instruction *Done::create(StatementArgList *args) const {
   return new Done(args);
}

void Done::execute(State *s, ParseList* parseList) const { }

// @overridden from Statement
const char *Done::getName() const { return "done"; }
const char *Done::getSyntax() const { return ""; }
const char *Done::getDescription() const {
   return "Terminates the program.";
}

#endif // SPECIAL_H_

