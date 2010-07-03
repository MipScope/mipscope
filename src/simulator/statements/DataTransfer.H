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
   file: DataTransfer.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */

#ifndef __DATATRANSFER_H__
#define __DATATRANSFER_H__

// ----------------------------------
// Instruction lui $dest, <constant>
// ----------------------------------
class Lui : public Instruction {
   public:
      Lui(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lui(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), ((*m_args)[1]->getValue(s) << 16 ));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lui"; }
      virtual const char *getSyntax() const { return "$dest, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the upper half-word of the register $dest to the lower half-word of &lt;immediate&gt;, filling in 0s for the lower bits.";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction li $dest, <constant>
// ----------------------------------
class Li : public Instruction {
   public:
      Li(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Li(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "li"; }
      virtual const char *getSyntax() const { return "$dest, &lt;immediate&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to &lt;immediate&gt;.";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction la $dest, <label>
// ----------------------------------
class La : public Instruction {
   public:
      La(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new La(args);
      }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "la"; }
      virtual const char *getSyntax() const { return "$dest, &lt;label&gt;"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the memory address &lt;label&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction lw $dest, addr
// ----------------------------------
class Lw : public Instruction {
   public:
      Lw(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryWord((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lw"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the value of the word stored at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction lbu $dest, addr
// ----------------------------------
class Lb : public Instruction {
   public:
      Lb(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Lb(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (signed int) (signed char) s->getMemoryByte((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lb"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the sign-extended value of the byte stored at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction lbu $dest, addr
// ----------------------------------
class Lbu : public Instruction {
   public:
      Lbu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lbu(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryByte((*m_args)[1]->getValue(s)));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lbu"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the non-sign-extended value of the byte stored at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction lh $dest, addr
// ----------------------------------
class Lh : public Instruction {
   public:
      Lh(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lh(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int value = s->getMemoryByte((*m_args)[1]->getValue(s) + 1) << 8 | s->getMemoryByte((*m_args)[1]->getValue(s));
         
         s->setRegister((*m_args)[0]->getRegister(), (signed int) (signed short) value);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lh"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the sign-extended value of the half-word stored at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction lh $dest, addr
// ----------------------------------
class Lhu : public Instruction {
   public:
      Lhu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lhu(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int value = s->getMemoryByte((*m_args)[1]->getValue(s) + 1) << 8 | s->getMemoryByte((*m_args)[1]->getValue(s));
         
         s->setRegister((*m_args)[0]->getRegister(), value);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lhu"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Sets $dest to the non-sign-extended value of the half-word stored at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};



// ----------------------------------
// Instruction lwl $dest, addr
// ----------------------------------
class Lwl : public Instruction {
   public:
      Lwl(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lwl(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int address = (*m_args)[1]->getValue(s);
         unsigned int regValue = (*m_args)[0]->getValue(s);
         
         unsigned int baseAddress = address - (address % 4);
         
         //cerr << "lwl: address: " << std::hex << address << ", regValue: " << regValue << ", baseAddress: " << baseAddress << "\n";
                  
         switch (address % 4) {
            case 0:
               //cerr << "lwl: case 0\n";
               regValue = (regValue & 0x00FFFFFF) | s->getMemoryByte(baseAddress) << 24;
               break;
            case 1:
               //cerr << "lwl: case 1\n";
               regValue = (regValue & 0xFF00FFFF) | s->getMemoryByte(baseAddress) << 16;            
               regValue = (regValue & 0x00FFFFFF) | s->getMemoryByte(baseAddress+1) << 24;
               break;
            case 2:
               //cerr << "lwl: case 2\n";
               regValue = (regValue & 0xFFFF00FF) | s->getMemoryByte(baseAddress) << 8;
               regValue = (regValue & 0xFF00FFFF) | s->getMemoryByte(baseAddress+1) << 16;            
               regValue = (regValue & 0x00FFFFFF) | s->getMemoryByte(baseAddress+2) << 24;
               break;
            case 3:
               //cerr << "lwl: case 3\n";
               regValue = s->getMemoryWord(baseAddress);
               break;
         }
         
         //cerr << "lwl: newRegValue: " << std::hex << regValue << "\n";
                                    
         s->setRegister((*m_args)[0]->getRegister(), regValue);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lwl"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Loads the left (most significant) bytes from the word at the possibly unalligned &lt;addr&gt; into $dest.";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction lwr $dest, addr
// ----------------------------------
class Lwr : public Instruction {
   public:
      Lwr(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Lwr(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int address = (*m_args)[1]->getValue(s);
         unsigned int regValue = (*m_args)[0]->getValue(s);
         
         unsigned int baseAddress = address - (address % 4);
         
         // cerr << "lwr: address: " << std::hex << address << ", regValue: " << regValue << ", baseAddress: " << baseAddress << "\n";
                  
         switch (address % 4) {
            case 3:
                // cerr << "lwr: case 3\n";
                              
               regValue = (regValue & 0xFFFFFF00) | s->getMemoryByte(baseAddress+3);
               break;
            case 2:
                // cerr << "lwr: case 2\n";
               regValue = (regValue & 0xFFFF00FF) | s->getMemoryByte(baseAddress+3) << 8;            
               regValue = (regValue & 0xFFFFFF00) | s->getMemoryByte(baseAddress+2);
               break;
            case 1:
                // cerr << "lwr: case 1\n";
               regValue = (regValue & 0xFF00FFFF) | s->getMemoryByte(baseAddress+3) << 16;
               regValue = (regValue & 0xFFFF00FF) | s->getMemoryByte(baseAddress+2) << 8;            
               regValue = (regValue & 0xFFFFFF00) | s->getMemoryByte(baseAddress+1);
               break;
            case 0:
                // cerr << "lwr: case 0\n";
               regValue = s->getMemoryWord(baseAddress);
               break;
         }
         
          // cerr << "lwr: newRegValue: " << std::hex << regValue << "\n";
                                    
         s->setRegister((*m_args)[0]->getRegister(), regValue);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "lwr"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Loads the right (least signficant) bytes from the word at the possibly unalligned &lt;addr&gt; into $dest.";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};






// ----------------------------------
// Instruction ld $dest, addr
// ----------------------------------
class Ld : public Instruction {
   public:
      Ld(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE) && ( (*args)[0]->hasRegister() ? (*args)[0]->getRegister() < ra : true);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ld(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getMemoryWord((*m_args)[1]->getValue(s)));
         s->setRegister((*m_args)[0]->getRegister() + 1, s->getMemoryWord((*m_args)[1]->getValue(s) + 4));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "ld"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Load the 64-bit quantity at &lt;addr&gt; into registers $dest and $dest+1";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction ulh $dest, addr
// ----------------------------------
class Ulh : public Instruction {
   public:
      Ulh(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ulh(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         int value = (short) (s->getMemoryByte((*m_args)[1]->getValue(s)+1) << 8 | s->getMemoryByte((*m_args)[1]->getValue(s)));
         
         s->setRegister((*m_args)[0]->getRegister(), value);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "ulh"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Load (sign extending) the 16-bit half-word at the possibly unaligned address &lt;addr&gt; into register $dest";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction ulhu $dest, addr
// ----------------------------------
class Ulhu : public Instruction {
   public:
      Ulhu(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ulhu(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int value = (s->getMemoryByte((*m_args)[1]->getValue(s)+1) << 8 | s->getMemoryByte((*m_args)[1]->getValue(s)));
         
         s->setRegister((*m_args)[0]->getRegister(), value);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "ulhu"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Load (not sign extending) the 16-bit half-word at the possibly unaligned address &lt;addr&gt; into register $dest";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction ulw $dest, addr
// ----------------------------------
class Ulw : public Instruction {
   public:
      Ulw(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ulw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         int value = s->getMemoryByte((*m_args)[1]->getValue(s)+3) << 24 |
                                       s->getMemoryByte((*m_args)[1]->getValue(s)+2) << 16 |
                                       s->getMemoryByte((*m_args)[1]->getValue(s)+1) << 8 |
                                       s->getMemoryByte((*m_args)[1]->getValue(s));
         
         s->setRegister((*m_args)[0]->getRegister(), value);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "ulw"; }
      virtual const char *getSyntax() const { return "$dest, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Load the 32-bit word at the possibly unaligned address &lt;addr&gt; into register $dest";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction sh $src, addr
// ----------------------------------
class Sh : public Instruction {
   public:
      Sh(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sh(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryByte(m_args->getValue(s, 1), m_args->getValue(s, 0) & 0xFF);
         s->setMemoryByte(m_args->getValue(s, 1) + 1, (m_args->getValue(s, 0) & 0xFF00) >> 8);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sh"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the least-significant half-word of register $src into &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction sh $src, addr
// ----------------------------------
class Ush : public Sh {
   public:
      Ush(StatementArgList *args = NULL) : Sh(args) { }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Ush(args);
      }      
      
      // @overridden from Statement
      virtual const char *getName() const { return "ush"; }
      virtual const char *getDescription() const {
         return "Stores the least-significant half-word of register $src into &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction sw $src, addr
// ----------------------------------
class Sw : public Instruction {
   public:
      Sw(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 90; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sw(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryWord(m_args->getValue(s, 1), m_args->getValue(s, 0));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sw"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the word in register $src into &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction sw $src, addr
// ----------------------------------
class Usw : public Sw {
   public:
      Usw(StatementArgList *args = NULL) : Sw(args) { }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Usw(args);
      }
      
      virtual int importance() const { return 50; }

      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryByte(m_args->getValue(s, 1), m_args->getValue(s, 0) & 0xFF);
         s->setMemoryByte(m_args->getValue(s, 1) + 1, (m_args->getValue(s, 0) & 0xFF00) >> 8);
         s->setMemoryByte(m_args->getValue(s, 1) + 2, (m_args->getValue(s, 0) & 0xFF0000) >> 16);
         s->setMemoryByte(m_args->getValue(s, 1) + 3, (m_args->getValue(s, 0) & 0xFF000000) >> 24);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "usw"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the word in register $src into the possibly-unaligned address &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction sw $src, addr
// ----------------------------------
class Swl : public Instruction {
   public:
      Swl(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Swl(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int address = (*m_args)[1]->getValue(s);
         unsigned int regValue = (*m_args)[0]->getValue(s);
         unsigned int baseAddress = address - (address % 4);
         
         //cerr << "lwl: address: " << std::hex << address << ", regValue: " << regValue << ", baseAddress: " << baseAddress << "\n";
                  
         switch (address % 4) {
            case 0:
               //cerr << "lwl: case 0\n";
               
               s->setMemoryByte(baseAddress + 0, (regValue & 0xFF000000) >>  24 );               
               break;
            case 1:
               //cerr << "lwl: case 1\n";
               
               s->setMemoryByte(baseAddress + 1, (regValue & 0xFF000000) >>  24 );
               s->setMemoryByte(baseAddress + 0, (regValue & 0x00FF0000) >>  16 );               
               break;
            case 2:
               //cerr << "lwl: case 2\n";
               
               s->setMemoryByte(baseAddress + 2, (regValue & 0xFF000000) >>  24 );
               s->setMemoryByte(baseAddress + 1, (regValue & 0x00FF0000) >>  16 );
               s->setMemoryByte(baseAddress + 0, (regValue & 0x0000FF00) >>  8 );
               break;
            case 3:
               //cerr << "lwl: case 3\n";
               s->setMemoryWord(baseAddress, regValue);
               break;
         }
         
         //cerr << "lwl: newRegValue: " << std::hex << regValue << "\n";
         
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "swl"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the left (most significant) bytes from the word in $dest into the possibly unalligned &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction swr $src, addr
// ----------------------------------
class Swr : public Instruction {
   public:
      Swr(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Swr(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         
         unsigned int address = (*m_args)[1]->getValue(s);
         unsigned int regValue = (*m_args)[0]->getValue(s);
         unsigned int baseAddress = address - (address % 4);
             
         switch (address % 4) {
            case 3:
               
               s->setMemoryByte(baseAddress + 3, (regValue & 0x000000FF));               
               break;
            case 2:
               
               s->setMemoryByte(baseAddress + 2, (regValue & 0x000000FF));
               s->setMemoryByte(baseAddress + 3, (regValue & 0x0000FF00) >>  8 ); 
                            
               break;
            case 1:               
               s->setMemoryByte(baseAddress + 1, (regValue & 0x000000FF));
               s->setMemoryByte(baseAddress + 2, (regValue & 0x0000FF00) >>  8 ); 
               s->setMemoryByte(baseAddress + 3, (regValue & 0x00FF0000) >>  16 );
               
               break;
            case 0:
               s->setMemoryWord(baseAddress, regValue);
               break;
         }
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "swr"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the right (least significant) bytes from the word in $dest into the possibly unalligned &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction sd $src, addr
// ----------------------------------
class Sd : public Instruction {
   public:
      Sd(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Sd(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryWord(m_args->getValue(s, 1), m_args->getValue(s, 0));
         s->setMemoryWord(m_args->getValue(s, 1) + 4,  s->getRegister((*m_args)[0]->getRegister() + 1));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sd"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the 64-bit quantity in registers $src and $src + 1 at &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};



// ----------------------------------
// Instruction sb $src, addr
// ----------------------------------
class Sb : public Instruction {
   public:
      Sb(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, ADDRESS | IMMEDIATE);
      }
      
      virtual Instruction *create(StatementArgList *args) const {
         return new Sb(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setMemoryByte(m_args->getValue(s, 1), (char) (m_args->getValue(s, 0) & 255) );
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "sb"; }
      virtual const char *getSyntax() const { return "$src, &lt;addr&gt;"; }
      virtual const char *getDescription() const {
         return "Stores the least-significant byte of register $src into &lt;addr&gt;";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction move $dest, $src
// ----------------------------------
class Move : public Instruction {
   public:
      Move(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 80; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Move(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "move"; }
      virtual const char *getSyntax() const { return "$dest, $src"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in register $src";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction movn $dest, $src
// ----------------------------------
class Movn : public Instruction {
   public:
      Movn(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Movn(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         if ((*m_args)[2]->getValue(s) != 0)
            s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "movn"; }
      virtual const char *getSyntax() const { return "$dest, $src, $condition"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in register $src if and only if the value in the register $condition is NOT zero";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction movz $dest, $src
// ----------------------------------
class Movz : public Instruction {
   public:
      Movz(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER, PLAIN_REGISTER, PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Movz(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         if ((*m_args)[2]->getValue(s) == 0)
            s->setRegister((*m_args)[0]->getRegister(), (*m_args)[1]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "movz"; }
      virtual const char *getSyntax() const { return "$dest, $src, $condition"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in register $src if and only if the value in the register $condition is zero";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mflo : public Instruction {
   public:
      Mflo(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Mflo(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getRegister(lo));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mflo"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in the special register LO";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction mfhi $dest, $src
// ----------------------------------
class Mfhi : public Instruction {
   public:
      Mfhi(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Mfhi(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister((*m_args)[0]->getRegister(), s->getRegister(hi));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mfhi"; }
      virtual const char *getSyntax() const { return "$dest"; }
      virtual const char *getDescription() const {
         return "Sets the register $dest to the value in the special register HI";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mtlo : public Instruction {
   public:
      Mtlo(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Mtlo(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister(lo, (*m_args)[0]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mtlo"; }
      virtual const char *getSyntax() const { return "$src"; }
      virtual const char *getDescription() const {
         return "Sets the special LO register to the value in the register (or immediate value) $src";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};


// ----------------------------------
// Instruction mflo $dest, $src
// ----------------------------------
class Mthi : public Instruction {
   public:
      Mthi(StatementArgList *args = NULL) : Instruction(args) { }
      
      // @overridden from Instruction
      virtual bool isSyntacticallyValid(StatementArgList *args) const {
         return args->matchesSyntax(PLAIN_REGISTER | IMMEDIATE);
      }
      
      virtual int importance() const { return 50; }

      virtual Instruction *create(StatementArgList *args) const {
         return new Mthi(args);
      }
      
      virtual void execute(State *s, ParseList* parseList) const {
         s->setRegister(hi, (*m_args)[0]->getValue(s));
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return "mthi"; }
      virtual const char *getSyntax() const { return "$src"; }
      virtual const char *getDescription() const {
         return "Sets the special HI register to the value in the register (or immediate value) $src";
      }

      virtual int getType() const { return DATA_TRANSFER; }
};

#endif // __DATATRANSFER_H__

