#ifndef CONTROLFLOW_H_
#define CONTROLFLOW_H_

#include "../Statement.H"
#include "../StatementArgList.H"
#include "../Identifier.H"
#include "../State.H"
#include "../ParseList.H"

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
      
      bool autoIncrementPC() { return false; }
      
      // @overridden from Statement
      virtual const char *getName() const { return "j"; }
      virtual const char *getSyntax() const { return "&lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Unconditionally jumps to &lt;address&gt;";
      }
};

#endif // CONTROLFLOW_H_

