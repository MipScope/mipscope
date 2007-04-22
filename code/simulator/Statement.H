#ifndef STATEMENT_H_
#define STATEMENT_H_
#include "typedefs.H"

class StatementArgList;
class State;

class Statement {
   public:
      Statement();
      virtual ~Statement() { }
      
      static void InitializeStatementMaps();
      
      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
      
      bool isInstruction();
      bool isDirective();
};

class Instruction : public Statement {
   public:
      Instruction(StatementArgList *args = NULL);
      virtual ~Instruction();
      
      static void InitializeInstructionMap();
      
      // Returns true if the arguments passed to this Instruction are valid
      virtual bool isSyntacticallyValid(StatementArgList *args) const = 0;

      // Factory for creating more of this specific type of Instruction
      virtual Instruction *create(StatementArgList *args) const = 0;
      
      // Executes this Instruction, modifying the given State accordingly
      virtual void execute(State* state) const = 0;

      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
      
   protected:
      StatementArgList *m_args;
};

class Directive : public Statement {
   public:
      Directive();
      
      static void InitializeDirectiveMap();
      
      // Factory for creating more of this specific type of Directive
      virtual Directive *create() const = 0;

      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
};

#endif /*STATEMENT_H_*/

