#ifndef STATEMENT_H_
#define STATEMENT_H_
#include "typedefs.H"

class StatementArgList;
class State;

class Statement {
   public:
      Statement(unsigned int requiredSizeInBytes);
      virtual ~Statement() { }
      
      static void InitializeStatementMaps();
       
      // Returns the number of bytes this Statement should take up in the address space
      unsigned int getSizeInBytes() const;
      
      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
      
      bool isInstruction();
      bool isDirective();

   protected:
      unsigned int m_size; // in bytes  (4 for all instructions)
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
      Directive(unsigned int requiredSizeInBytes = 0);
      
      static void InitializeDirectiveMap();
      
      // Factory for creating more of this specific type of Directive
      //virtual Directive *create(unsigned int size = 0) const = 0;
      
      // Performs any necessary (static) initialization to s
      // Note: it's important for Directives to know about their ParseNodes
      // so that they know what address they begin at
      // // (default is no initialization)
      virtual void initialize(ParseNode *p, State *s);
      
      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
};

#endif /*STATEMENT_H_*/

