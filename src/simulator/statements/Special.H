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

#ifndef SPECIAL_H_
#define SPECIAL_H_

// TODO: Break

// ----------------------------------
// Instruction nop
// ----------------------------------
class Nop : public Instruction {
   public:
      Nop(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(NONE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Nop(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {}
      
      // @overridden from Statement
      virtual const char *getName() const { return "nop"; }
      virtual const char *getSyntax() const { return ""; }
      virtual const char *getDescription() const {
         return "Does nothing.";
      }

      virtual int getType() const { return SPECIAL; }
};


class Syscall : public Instruction {
   public:
      Syscall(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(NONE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Syscall(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->doSyscall();
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "syscall"; }
      virtual const char *getSyntax() const { return ""; }
      virtual const char *getDescription() const {
         return "System call to the operating system; $v0 holds syscall number.";
      }

      virtual int getType() const { return SPECIAL; }
};

// ----------------------------------
// Instruction done
// ----------------------------------
Done::Done(StatementArgList *args) : Instruction(args) { }

bool Done::isSyntacticallyValid(StatementArgList *args) const {
   return args->matchesSyntax(NONE);
}

Instruction *Done::create(StatementArgList *args) const {
   return new Done(args);
}

void Done::execute(State *s, ParseList* parseList) const { }

// @overridden from Statement
const char *Done::getName() const { return "done"; }
const char *Done::getSyntax() const { return ""; }
const char *Done::getDescription() const {
   return "Terminates the program.";
}

int Done::getType() const { return SPECIAL; }

#endif // SPECIAL_H_

