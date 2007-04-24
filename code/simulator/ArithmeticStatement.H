#ifndef ARITHMETICSTATEMENT_H_
#define ARITHMETICSTATEMENT_H_
#include "Statement.H"

class Add : public Instruction {	
   public:
      Add(StatementArgList *args = NULL);
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const;
      virtual Instruction *create(StatementArgList *args) const;
      virtual void execute(State *state) const;
      
      // @overridden from Statement
      virtual const char *getName() const;
      virtual const char *getSyntax() const;
      virtual const char *getDescription() const;
};

#endif // ARITHMETICSTATEMENT_H_

