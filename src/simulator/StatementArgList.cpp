/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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

#include <iostream>

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
bool StatementArgList::matchesSyntax(int arg0, int arg1, int arg2) const {
   
  // cerr << arg0 << "\n" << arg1 << "\n" << arg2 << "\n" << arg3;
   
   return ( isArgType(0, arg0) && isArgType(1, arg1) && isArgType(2, arg2));
}

// indexed from 0.
bool StatementArgList::isArgType(int argNumber, int what) const {
   
   return ((argNumber < noArgs()) ? (m_args[argNumber]->getType()) : NONE) & what;  
}




/*
 * StatementArg
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

// everything that takes an address should take IMMEDIATE | ADDRESS.
// a Statement is only of type ADDRESS if we KNOW it's an address.
// it's possible that it's a correct address, but if it's just a plain number,
// we call it an IMMEDIATE.
int StatementArg::getType(void) {
   
   //cerr << "StatementArg::getType: " << "identifier=" << m_id << " register=" << m_register << " dereference=" << m_dereference << "\n";
   
   int ret = 0;
   if (isRegister()) ret |= PLAIN_REGISTER;
   if (isAddress()) ret |= ADDRESS;
   if (isImmediate()) ret |=  IMMEDIATE;
   
   return ret;   
}

bool StatementArg::isRegister(void) const {
   return (hasRegister() && !hasIdentifier() && !hasDereference());
}

bool StatementArg::isAddress(void) const {
   return (hasIdentifier() || (hasRegister() && hasDereference()));
}

bool StatementArg::isImmediate(void) const {
   return (!hasRegister() && hasIdentifier() && m_id->isImmediate());
}

bool StatementArg::hasAddressIdentifier() const {
   return (hasIdentifier() && m_id->isAddress());
}

bool StatementArg::hasIdentifier(void) const {
   return (m_id != NULL);
}  

bool StatementArg::hasRegister(void) const {
   return (m_register >= 0);
}

bool StatementArg::hasDereference(void) const {
   return m_dereference;
}

Identifier *StatementArg::getID(void) const {
   return m_id;
}

int StatementArg::getRegister(void) const {
   return m_register;
}

int StatementArg::getValue(State *s) const {
   if (hasRegister()) {
      unsigned int registerVal = s->getRegister(m_register);
      
      if (!hasDereference())
         return (signed)registerVal;

      // TODO:  NOT SURE IF THIS IS HOW DEREFERENCING WORKS IN MIPS!
//      registerVal = s->getMemoryWord(registerVal);
      
      if (hasIdentifier())
         return (signed)(registerVal + m_id->getValue());

      return (signed)registerVal;
   }
   
//   cerr << "\tStatementArg::getValue() getting id's val: " << m_id->getID().toStdString() << " = " << m_id->getValue() << endl;

   // Must be identifier (value of label and/or immediate)
   return m_id->getValue();
}

