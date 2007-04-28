#ifndef ASSER_H_
#define ASSER_H_

// ----------------------------------
// Instruction assertequals 
// ----------------------------------
class AssertEquals : public Instruction {
   public:
      AssertEquals(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new AssertEquals(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1);

         s->assertEquals(val1, val2);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "assertequals"; }
      virtual const char *getSyntax() const { return "$reg, imm"; }
      virtual const char *getDescription() const {
         return "Asserts that the value in register $reg is equal to the immediate value given, terminating the program if the assertion fails";
      }
};

#endif /*ASSER_H_*/
