/* ---------------------------------------------- *\
   file: StatementArgList.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/20/2007
\* ---------------------------------------------- */
#include "StatementArgList.H"
#include "Identifier.H"
#include "typedefs.H"
#include "State.H"

StatementArgList::StatementArgList(StatementArg *a1, StatementArg *a2, StatementArg *a3)
{
   m_args[0] = a1;
   m_args[1] = a2;
   m_args[2] = a3;
   
   m_count = (a1 != NULL) + (a2 != NULL) + (a3 != NULL);

   /*cerr << "ArgList: \n";
   cerr << "noArgs() = " << noArgs() << "; noPlain=" << noPlainRegisterArgs();
   cerr << ", allplain=" << allArgumentsArePlainRegisters(3) << 
      ", hasRequiredPlainRegisters=" << hasRequiredPlainRegisters(2, 3) << endl;*/
}

int StatementArgList::noArgs() const {
   return m_count;
}

int StatementArgList::getValue(State *s, int ind) const {
   return m_args[ind]->getValue(s);
}

StatementArg *StatementArgList::operator[](int ind) const {
   return m_args[ind];
}

// returns true iff each arg in the argument list matches the corresponding
// argument type provided. For example,
// matchesSyntax(PLAIN_REGISTER, ADDRESS) returns true iff there are exactly two arguments and the first is a plain register, and the second is an address.
// You can also do:
// matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE) which works as you'd expect.
bool StatementArgList::matchesSyntax(int arg0, int arg1, int arg2, int arg3) const {
   
   return ( isArgType(0, arg0) && isArgType(1, arg1) && isArgType(2, arg2) && isArgType(3, arg3));
}

// indexed from 0.
bool StatementArgList::isArgType(int argNumber, int what) const {
   return ((argNumber < noArgs()) ? (*this)[argNumber]->getType() : NONE) & what;  
}




/*
 * StatementArg
 * behold.
 */
StatementArg::StatementArg(Identifier *id, int r, bool dereference) 
   : m_id(id), m_register(r), m_dereference(dereference)
{ }

StatementArg::StatementArg(int r, bool dereference) 
   : m_id(NULL), m_register(r), m_dereference(dereference)
{ }

// Utility methods to be used in Statement::isValid implementations      
bool StatementArg::isType(int kind) {
     return (bool) (getType() & kind);   
}

int StatementArg::getType(void) {
   if (hasRegister() && !hasIdentifier() && !hasDereference()) return PLAIN_REGISTER;
   if (hasIdentifier() || (hasRegister() && hasDereference())) return ADDRESS;
   if (!hasRegister() && hasIdentifier() && m_id->isImmediate()) return IMMEDIATE;
   return NONE; // shouldn't get here
   
}

bool StatementArg::hasIdentifier() const {
   return (m_id != NULL);
}

bool StatementArg::hasRegister() const {
   return (m_register >= 0);
}

bool StatementArg::hasDereference() const {
   return m_dereference;
}

Identifier *StatementArg::getID() const {
   return m_id;
}

int StatementArg::getRegister() const {
   return m_register;
}

int StatementArg::getValue(State *s) const {
   if (hasRegister()) {
      unsigned int registerVal = s->getRegister(m_register);
      
      if (!hasDereference())
         return (signed)registerVal;

      registerVal = s->getMemoryWord(registerVal);
      
      if (hasIdentifier())
         return (signed)(registerVal + m_id->getValue());
   }
   
   // Must be identifier (value of label and/or immediate)
   return m_id->getValue();
}

