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

#ifndef STATEMENT_H_
#define STATEMENT_H_
#include "typedefs.H"

class StatementArgList;
class State;
class ParseList;
class ConstStatement;
class AddressIdentifier;
class ImmediateIdentifier;

enum StatementType {
   ARITHMETIC, COMPARISON, CONTROL_FLOW, DATA_TRANSFER, SPECIAL, DIRECTIVE
};

class Statement {
   public:
      Statement(unsigned int requiredSizeInBytes);
      virtual ~Statement() { }
      
      static void InitializeStatementMaps();
      
      // defaults to no initialization, overridden by some Directives
      virtual void initialize(ParseNode *p, State *s);
      
      // Returns the number of bytes this Statement should take up in the address space
      virtual unsigned int getSizeInBytes() const;
      virtual unsigned int getPreferredAlignment() const;
      
      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
      
      // should return an int from 0 to 100 inclusive, rating 
      // this Statement's importance in relation to other 
      // statements (for reference purposes)
      virtual int importance() const;

      virtual bool isInstruction() const;
      virtual bool isDirective() const;
      virtual bool isPreprocessor() const; // consts and #defines
      
      static Directive   *getDirective(Statement *s);
      static Instruction *getInstruction(Statement *s);
      static ConstStatement *getConst(Statement *s);

      // Populate the given vector with AddressIdentifiers for all of the labels
      // referenced by this statement (could be arguments for instructions, or the
      // initial values of directives
      virtual void get_referenced_labels (std::vector<AddressIdentifier*>&) { }
      
   protected:
      unsigned int m_size; // in bytes  (4 for all instructions)
};

class Instruction : public Statement {
   public:
      Instruction(StatementArgList *args = NULL);
      virtual ~Instruction();
      
      static void insert(Instruction *instr);
      static void InitializeInstructionMap();
      
      // returns true if ParseNode should automatically increment PC to next ParseNode
      // returns false for branch-like instructions which update the PC automatically
      virtual bool autoIncrementPC() const;
      StatementArgList *getArguments() const;
      
      // Returns true if the arguments passed to this Instruction are valid
      virtual bool isSyntacticallyValid(StatementArgList *args) const = 0;
       
      // Factory for creating more of this specific type of Instruction
      virtual Instruction *create(StatementArgList *args) const = 0;
      
      // Executes this Instruction, modifying the given State accordingly
      virtual void execute(State* state, ParseList* parseList) const = 0;

      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
      virtual int getType() const;

      virtual void get_referenced_labels (std::vector<AddressIdentifier*>&);
      
   protected:
      // Store the address of the next instruction in registerNo
      void link(State *s, unsigned int registerNo = ra) const;
      
      StatementArgList *m_args;
};

class Directive : public Statement {
   public:
      Directive(unsigned int requiredSizeInBytes = 0);
      
      static void InitializeDirectiveMap();
      
      virtual unsigned int getPreferredAlignment() const;
      
      // utility methods..
      virtual bool isAlign() const;
      virtual bool isData() const;
      virtual bool isText() const;
      
      // Factory for creating more of this specific type of Directive
      //virtual Directive *create(unsigned int size = 0) const = 0;
      
      // Performs any necessary (static) initialization to s
      // Note: it's important for Directives to know about their ParseNodes
      // so that they know what address they begin at
      // // (default is no initialization)
      virtual void initialize(ParseNode *p, State *s);
      
      virtual const char *getName() const = 0;
      virtual const char *getSyntax() const = 0;
      virtual const char *getDescription() const = 0;
};

// (special, needs to go here to be referenced by Debugger)
// ----------------------------------
// Instruction done
// ----------------------------------
class Done : public Instruction {
   public:
      Done(StatementArgList *args = NULL);
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const;
      virtual Instruction *create(StatementArgList *args) const;
      virtual void execute(State *s, ParseList* parseList) const;
      
      virtual int importance() const;

      // @overridden from Statement
      virtual const char *getName() const;
      virtual const char *getSyntax() const;
      virtual const char *getDescription() const;
      virtual int getType() const;
};

class ConstStatement : public Statement {
   public:
      ConstStatement(AddressIdentifier *name, ImmediateIdentifier *value);
      
      virtual bool isInstruction();
      virtual bool isDirective();
      
      virtual int importance() const;

      virtual AddressIdentifier *getID() const;
      virtual ImmediateIdentifier *getImmediate() const;
      
      // @overridden from Statement
      virtual const char *getName() const;
      virtual const char *getSyntax() const;
      virtual const char *getDescription() const;
      
   protected:
      AddressIdentifier *m_id;
      ImmediateIdentifier *m_value;
};



#endif /*STATEMENT_H_*/

