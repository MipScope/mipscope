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

      virtual void execute(State *s) const {
         int val1 = m_args->getValue(s, 0);
         int val2 = m_args->getValue(s, 1);

         s->assertEquals(val1, val2);
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "add"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2 OR <immediate>"; }
      virtual const char *getDescription() const {
         return "Adds two signed integers, storing the result in $dest.";
      }
};

#endif /*ASSER_H_*/
