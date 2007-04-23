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

int StatementArgList::noPlainRegisterArgs() const {
   int count = 0;
   
   for(int i = m_count; i--;)
      count += (m_args[i]->isRegister());

   return count;
}

StatementArg *StatementArgList::operator[](int ind) const {
   return m_args[ind];
}

// Returns true iff this list contains EXACTLY noRequired plain register arguments
bool StatementArgList::allArgumentsArePlainRegisters(int noRequired) const {
   return (noPlainRegisterArgs() == noRequired);
}

// Returns true iff this list contains AT LEAST noRequired plain register arguments (all coming at the beginning) and EXACTLY noTotal arguments
bool StatementArgList::hasRequiredPlainRegisters(int noRequired, int noTotal) const {
   if (m_count != noTotal)
      return false;
   
   for(int i = 0; i < noRequired; i++)
      if (!m_args[i]->isRegister())
         return false;

   return true;
}

// Returns true iff this list has noRegisters, followed to noTotal - noRegisters StatementArgs which can either be plain Registers or ImmediateIdentifiers
bool StatementArgList::fitsRegisterImmediateType(int noRegisters, int noTotal) const
{
   if (hasRequiredPlainRegisters(noRegisters, noTotal)) {
      
      for(int i = noRegisters; i < noTotal; i++)
         if (!m_args[i]->isRegisterOrImmediate())
            return false;
      
      return true;
   }

   return false;
}

StatementArg::StatementArg(Identifier *id, int r, bool dereference) 
   : m_id(id), m_register(r), m_dereference(dereference)
{ }

StatementArg::StatementArg(int r, bool dereference) 
   : m_id(NULL), m_register(r), m_dereference(dereference)
{ }

bool StatementArg::isRegister() const {
   return (hasRegister() && !hasIdentifier() && !hasDereference());
}

bool StatementArg::isAddress() const {
   return (hasIdentifier() || (hasRegister() && hasDereference()));
}

bool StatementArg::isImmediate() const {
   return (!hasRegister() && hasIdentifier() && m_id->isImmediate());
}

bool StatementArg::isRegisterOrImmediate() const {
   return (isRegister() || isImmediate());
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

