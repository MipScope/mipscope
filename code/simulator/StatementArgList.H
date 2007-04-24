/* ---------------------------------------------- *\
   file: StatementArgList.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef __STATEMENTARGLIST_H__
#define __STATEMENTARGLIST_H__
#include "typedefs.H"

class State;
class ParseNode;
class Identifier;
class StatementArg;

enum ArgType {PLAIN_REGISTER = 1, ADDRESS = 2, IMMEDIATE = 4, NONE = 8};

class StatementArgList {
   public:
      StatementArgList(StatementArg *a1 = NULL, StatementArg *a2 = NULL, StatementArg *a3 = NULL);
      
      // Returns the number of arguments in this list
      int noArgs() const;

      // Returns the number of plain register arguments in this list
      int noPlainRegisterArgs() const;

      // Use to index into and retrieve a specific StatementArg in this list
      StatementArg *operator[](int ind) const;
      
      // Use to get the value of a StatementArg within this list, given the current State
      int getValue(State *s, int ind) const;
      
      // returns true iff each arg in the argument list matches the corresponding
      // argument type provided. For example,
      // matchesSyntax(PLAIN_REGISTER, ADDRESS) returns true iff there are exactly two arguments and the first is a plain register, and the second is an address.
      // You can also do:
      // matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE) which works as you'd expect.
      bool matchesSyntax(int arg1, int arg2 = NONE, int arg3 = NONE, int arg4 = NONE) const;
      bool isArgType(int argNumber, int what) const;
      
      // Returns true iff this list contains exactly noRequired plain register arguments
      // Ex:  MoveInstruction::isValid() would only need to check for allArgumentsArePlainRegisters(3);
      bool allArgumentsArePlainRegisters(int noRequired = 3) const;
      
      // Returns true iff this list contains AT LEAST noRequired plain register arguments (all coming at the beginning) and EXACTLY noTotal arguments
      bool hasRequiredPlainRegisters(int noRequired, int noTotal) const;
      
      // Returns true iff this list has noRegisters, followed by (noTotal - noRegisters) StatementArgs which can either be plain Registers or ImmediateIdentifiers
      // Ex:  Add::isValid() would only need fitsRegisterImmediateType(2, 3);
      // for its syntactic-validity check
      bool fitsRegisterImmediateType(int noRegisters, int noTotal) const;
      
   protected:
      StatementArg *m_args[3];
      int m_count;
};

/*enum ArgTypes {
   A_Register = 1, // Set anytime StatementArg contains a register
   A_Identifier = 2, // Set for immediate and Label
   A_Dereference = A_Register | 4, // Set when dereferencing a register
   
   A_MaybeAddress = ~A_Register
};*/

class StatementArg {
   public:
      StatementArg(Identifier *id, int reg = -1, bool dereference = false);
      StatementArg(int reg, bool dereference = false);
      
      // Utility methods to be used in Statement::isValid implementations      
      bool isType(int kind);
      int getType(void);
      
      bool hasIdentifier() const;
      bool hasRegister() const;
      bool hasDereference() const;
      
      Identifier *getID() const;
      int getRegister() const;
      
      // Use to get the value of this StatementArg, given the current State
      int getValue(State *s) const;
      
   protected:
      Identifier *m_id;
      int m_register;
      
      // if parenthesis are placed around m_register  ($sp)
      bool m_dereference;
};

#endif // __STATEMENTARGLIST_H__

