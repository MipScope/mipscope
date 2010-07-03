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

      virtual int getType() const { return SPECIAL; }
};

#endif /*ASSER_H_*/

