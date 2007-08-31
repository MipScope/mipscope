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
   file: Comparison.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */

#ifndef COMPARISON_H_
#define COMPARISON_H_

#include "../Statement.H"
#include "../StatementArgList.H"
#include "../Identifier.H"
#include "../State.H"
#include "../StateException.H"

#include <iostream>

// ----------------------------------
// Instruction slt $dest, $src1, $src2
// ----------------------------------
class Slt : public Instruction {
   public:
      Slt(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Slt(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed) m_args->getValue(s, 1) < (signed) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      
      virtual const char *getName() const { return "slt"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if signed register $src1 is less than $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sltu $dest, $src1, $src2
// ----------------------------------
class Sltu : public Instruction {
   public:
      Sltu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sltu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (unsigned) m_args->getValue(s, 1) < (unsigned) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sltu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if unsigned register $src1 is less than $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction slti $dest, $src1, immediate
// ----------------------------------
class Slti : public Instruction {
   public:
      Slti(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Slti(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed) m_args->getValue(s, 1) < (signed) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "slti"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if signed register $src1 is less than &lt;immediate&gt;, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sltiu $dest, $src1, immediate
// ----------------------------------
class Sltiu : public Instruction {
   public:
      Sltiu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sltiu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (unsigned) m_args->getValue(s, 1) < (unsigned) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sltiu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if unsigned register $src1 is less than &lt;immediate&gt;, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sltiu $dest, $src1, immediate
// ----------------------------------
class Seq : public Instruction {
   public:
      Seq(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Seq(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), m_args->getValue(s, 1) == m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "seq"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if register $src1 equals register $src2 and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sge $dest, $src1, $src2
// ----------------------------------
class Sge : public Instruction {
   public:
      Sge(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sge(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed) m_args->getValue(s, 1) >= (signed) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sge"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if signed register $src1 is greater than or equal to $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction Sgeu $dest, $src1, $src2
// ----------------------------------
class Sgeu : public Instruction {
   public:
      Sgeu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sgeu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (unsigned) m_args->getValue(s, 1) >= (unsigned) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sgeu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if unsigned register $src1 is greater than or equal to $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sgt $dest, $src1, $src2
// ----------------------------------
class Sgt : public Instruction {
   public:
      Sgt(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sgt(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed) m_args->getValue(s, 1) > (signed) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sgt"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if signed register $src1 is greater than $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sgtu $dest, $src1, $src2
// ----------------------------------
class Sgtu : public Instruction {
   public:
      Sgtu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sgtu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (unsigned) m_args->getValue(s, 1) > (unsigned) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sgtu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if unsigned register $src1 is greater than $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};


// ----------------------------------
// Instruction sle $dest, $src1, $src2
// ----------------------------------
class Sle : public Instruction {
   public:
      Sle(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sle(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed) m_args->getValue(s, 1) <= (signed) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sle"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if signed register $src1 is less than or equal to $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sle $dest, $src1, $src2
// ----------------------------------
class Sleu : public Instruction {
   public:
      Sleu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sleu(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (unsigned) m_args->getValue(s, 1) <= (unsigned) m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sleu"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if unsigned register $src1 is less than or equal to $src2, and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

// ----------------------------------
// Instruction sne $dest, $src1, immediate
// ----------------------------------
class Sne : public Instruction {
   public:
      Sne(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER | IMMEDIATE, PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sne(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), m_args->getValue(s, 1) != m_args->getValue(s, 2));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sne"; }
      virtual const char *getSyntax() const { return "$dest, $src1, $src2"; }
      virtual const char *getDescription() const {
         return "Set register $dest to 1 if register $src1 does not equal register $src2 and 0 otherwise.";
      }

      virtual int getType() const { return COMPARISON; }
};

#endif /*COMPARISON_H_*/

