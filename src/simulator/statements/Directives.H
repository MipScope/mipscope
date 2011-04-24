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
   file: Directives.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/23/2007
\* ---------------------------------------------- */
#ifndef __DIRECTIVES_H__
#define __DIRECTIVES_H__

#include "../Statement.H"
#include "../ParseNode.H"
#include "../State.H"
#include <stdlib.h>
#include <QString>
#include <vector>
using namespace std;

// ----------------------------------
// Directive .ascii  "string"
// ----------------------------------
class AsciiDirective : public Directive {
   public:
      AsciiDirective(const QString &string = "") : Directive(string.length()), 
         m_string(string) { }
      
      virtual void initialize(ParseNode *p, State *s) {
         unsigned int address = (unsigned) p->getAddress();
         
         int length = m_string.length();
         s->memcpy(address, m_string.toAscii().constData(), length);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".ascii"; }
      virtual const char *getSyntax() const { return "\"string\""; }
      virtual const char *getDescription() const {
         return "Declares a non-null-terminated ASCII string";
      }

   protected:
      QString m_string;
};

// ----------------------------------
// Directive .asciiz  "string"
// ----------------------------------
class AsciizDirective : public Directive {
   public:
      AsciizDirective(const QString &string = "") : Directive(string.length() + 1), 
         m_string(string) { }
      
      virtual void initialize(ParseNode *p, State *s) {
         unsigned int address = (unsigned) p->getAddress();
         
         int length = m_string.length() + 1;
         s->memcpy(address, m_string.toAscii().constData(), length);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".asciiz"; }
      virtual const char *getSyntax() const { return "\"string\""; }
      virtual const char *getDescription() const {
         return "Declares a null-terminated ASCII string";
      }

   protected:
      QString m_string;
};

// ----------------------------------
// Directive .data <address>
// ----------------------------------
class DataDirective : public Directive {
   public:
      DataDirective(int address = -1) : Directive(), m_address(address) { }
     
      virtual unsigned int getPreferredAlignment() const {
         return 2;
      }

      virtual bool isData() const {
         return true;
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".data"; }
      virtual const char *getSyntax() const { return " &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Declares the beginning of a data segment, optionally beginning at &lt;address&gt;";
      }

   protected:
      int m_address;
};

// ----------------------------------
// Directive .text <address>
// ----------------------------------
class TextDirective : public Directive {
   public:
      TextDirective(int address = -1) : Directive(), m_address(address) { }
     
      virtual unsigned int getPreferredAlignment() const {
         return 2;
      }

      virtual bool isText() const {
         return true;
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".text"; }
      virtual const char *getSyntax() const { return " &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Declares the beginning of a text segment (code), optionally beginning at &lt;address&gt;";
      }
      
   protected:
      int m_address;
};

// ----------------------------------
// Directive .ktext <address>
// ----------------------------------
class KTextDirective : public Directive {
   public:
      KTextDirective(int address = -1) : Directive(), m_address(address) { }
     
      virtual unsigned int getPreferredAlignment() const {
         return 2;
      }

      virtual int importance() const { return 50; }

      virtual bool isText() const {
         return true;
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".ktext"; }
      virtual const char *getSyntax() const { return " &lt;address&gt;"; }
      virtual const char *getDescription() const {
         return "Warning: not fully implemented; no different than .text";
      }
      
   protected:
      int m_address;
};

// ----------------------------------
// Directive .align offset
// ----------------------------------
class AlignDirective : public Directive {
   public:
      AlignDirective(int alignment = 0) : Directive(), m_alignment(alignment) { }
      virtual unsigned int getPreferredAlignment() const {
         return m_alignment;
      }

      virtual int importance() const { return 50; }

      virtual bool isAlign() const {
         return true;
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".align"; }
      virtual const char *getSyntax() const { return "offset"; }
      virtual const char *getDescription() const {
         return "Aligns the next datum on a 2^offset byte boundary";
      }

   protected:
      int m_alignment;
};

// ----------------------------------
// Directive .space noBytes
// ----------------------------------
class SpaceDirective : public Directive {
   public:
      SpaceDirective(int noBytes = 0) : Directive(noBytes) { }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".space"; }
      virtual const char *getSyntax() const { return "noBytes"; }
      virtual const char *getDescription() const {
         return "Reserves the specified number of bytes in the current data segment";
      }
};

// ----------------------------------
// Directive .global symbol
// ----------------------------------
class GlobalDirective : public Directive {
   public:
      GlobalDirective(AddressIdentifier *label = NULL) : Directive(), m_label(label) { }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".global"; }
      virtual const char *getSyntax() const { return "label"; }
      virtual const char *getDescription() const {
         return "Assigns a global scope to the given symbol (can be used in other files).";
      }
      
   protected:
      AddressIdentifier *m_label;
};

// --------------------------------------------
// Directive .byte   value
// Directive .byte   value:size
// Directive .byte   value1, value2, value3 ...
// --------------------------------------------
class ByteDirective : public Directive {
   public:
      ByteDirective(unsigned char value = 0, unsigned int no = 1) : Directive(no * sizeof(char)), m_values(NULL) {
         if ((m_values = (unsigned char *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }
         
         memset(m_values, value, m_size);
      }

      ByteDirective(const vector<int> &values) : Directive(values.size() * sizeof(char)), m_values(NULL) {
         if ((m_values = (unsigned char *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }

         unsigned char *ptr = m_values;
         
         for(unsigned int i = 0; i < values.size(); i++)
            *ptr++ = (unsigned char)values[i];
      }
      
      virtual ~ByteDirective() {
         safeFree(m_values);
      }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
         unsigned int address = (unsigned) p->getAddress();
         
         if (m_values != NULL)
            s->memcpy(address, m_values, m_size);
//         else s->memset(address, m_value, m_size);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".byte"; }
      virtual const char *getSyntax() const { return "value  OR  value:size  OR  value1, value2, ..."; }
      virtual const char *getDescription() const {
         return "Initializes a block of one or more consecutive bytes in memory";
      }
      
   private:
      unsigned char *m_values; // non-null if we have a list
};

// --------------------------------------------
// Directive .half   value
// Directive .half   value:size
// Directive .half   value1, value2, value3 ...
// --------------------------------------------
class HalfDirective : public Directive {
   public:
      HalfDirective(unsigned short value = 0, unsigned short no = 1) : Directive(no * sizeof(short)), m_values(NULL) {
         if ((m_values = (unsigned short *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }
         
         unsigned short *ptr = m_values;
         for(unsigned int i = no; i--;)
            *ptr++ = value;
//         memset(m_values, value, no);
      }
 
      HalfDirective(const vector<int> &values) : Directive(values.size() * sizeof(short)), m_values(NULL) {
         if ((m_values = (unsigned short *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }
         
         unsigned short *ptr = m_values;
         
         for(unsigned int i = 0; i < values.size(); i++)
            *ptr++ = (unsigned short)values[i];
      }
      
      virtual ~HalfDirective() {
         safeFree(m_values);
      }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
         unsigned int address = (unsigned) p->getAddress();
//         assert(!(address & 1));
         
         if (m_values != NULL)
            s->memcpy(address, m_values, m_size);
      }
      
      virtual unsigned int getPreferredAlignment() const {
         return 1; // 2 byte boundary
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".half"; }
      virtual const char *getSyntax() const { return "value  OR  value:size  OR  value1, value2, ..."; }
      virtual const char *getDescription() const {
         return "Initializes a block of one or more consecutive half-words in memory";
      }
      
   private:
      unsigned short *m_values;
};

// --------------------------------------------
// Directive .word   value
// Directive .word   value:size
// Directive .word   value1, value2, value3 ...
// --------------------------------------------
class WordDirective : public Directive {
   public:
      WordDirective(unsigned int value = 0, unsigned int no = 1) : Directive(no * sizeof(int)), m_values(NULL) {
         if ((m_values = (unsigned int *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }
         
         unsigned int *ptr = m_values;
         for(unsigned int i = no; i--;)
            *ptr++ = value;
         
//         memset(m_values, value, no);
      }
      
      WordDirective(const vector<int> &values) : Directive(values.size() * sizeof(int)), m_values(NULL) {
         if ((m_values = (unsigned int *)malloc(m_size)) == NULL) {
            perror("malloc");
            cerr << "\nERROR:  Ran out of memory!\n";
            exit(1);
         }
         
         unsigned int *ptr = m_values;
         
         for(unsigned int i = 0; i < values.size(); i++)
            *ptr++ = (unsigned int)values[i];
      }
      
      virtual ~WordDirective() {
         safeFree(m_values);
      }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
         unsigned int address = (unsigned) p->getAddress();
//         assert(!(address & 3));
         
         if (m_values != NULL)
            s->memcpy(address, m_values, m_size);
      }
      
      virtual unsigned int getPreferredAlignment() const {
         return 2; // 4 byte boundary
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".word"; }
      virtual const char *getSyntax() const { return "value  OR  value:size  OR  value1, value2, ..."; }
      virtual const char *getDescription() const {
         return "Initializes a block of one or more consecutive words in memory";
      }
      
   private:
      unsigned int *m_values;
};

// ----------------------------------
// Directive CONSTANT = imm
// ----------------------------------
ConstStatement::ConstStatement(AddressIdentifier *name, ImmediateIdentifier *value) : Statement(0), m_id(name), m_value(value)
{ }

bool ConstStatement::isInstruction() { return false; }
bool ConstStatement::isDirective() { return false; }

AddressIdentifier *ConstStatement::getID() const {
   return m_id;
}

ImmediateIdentifier *ConstStatement::getImmediate() const {
   return m_value;
}

// @overridden from Statement
const char *ConstStatement::getName() const { return ""; }
const char *ConstStatement::getSyntax() const { return ""; }
const char *ConstStatement::getDescription() const { return ""; }

#endif // __DIRECTIVES_H__

