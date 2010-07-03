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

      // Use to index into and retrieve a specific StatementArg in this list
      StatementArg *operator[](int ind) const;
      
      // Use to get the value of a StatementArg within this list, given the current State
      int getValue(State *s, int ind) const;
      
      // returns true iff each arg in the argument list matches the corresponding
      // argument type provided. For example,
      // matchesSyntax(PLAIN_REGISTER, ADDRESS) returns true iff there are exactly two arguments and the first is a plain register, and the second is an address.
      // You can also do:
      // matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE) which works as you'd expect.
      bool matchesSyntax(int arg1, int arg2 = NONE, int arg3 = NONE) const;
      bool isArgType(int argNumber, int what) const;
      
      
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
      
      bool isRegister(void) const;
      bool isAddress(void) const;
      bool isImmediate(void) const;
      
      bool hasAddressIdentifier() const;
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

