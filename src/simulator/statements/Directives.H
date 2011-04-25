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
#include <memory>
#include <assert.h>
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

class IntegerDirectiveInitializer {
public:
	virtual ~IntegerDirectiveInitializer() { }

	virtual size_t get_count () const = 0;

	virtual void init_byte (State*, unsigned int address) = 0;
	virtual void init_half (State*, unsigned int address) = 0;
	virtual void init_word (State*, unsigned int address) = 0;

	virtual void get_referenced_labels (std::vector<AddressIdentifier*>&) = 0;
};

class SingleIntegerDirectiveInitializer : public IntegerDirectiveInitializer {
	std::vector<Identifier*>	m_values;

	template<class Value> void init (State* s, unsigned int address);

	// Disallow copying and assignment, since we own the memory in m_values
	SingleIntegerDirectiveInitializer (const SingleIntegerDirectiveInitializer&) { }
	SingleIntegerDirectiveInitializer& operator= (const SingleIntegerDirectiveInitializer&) { return *this; }
public:
	explicit SingleIntegerDirectiveInitializer (Identifier* value) { m_values.push_back(value); }
	explicit SingleIntegerDirectiveInitializer (const std::vector<Identifier*>& values) : m_values(values) { }
	~SingleIntegerDirectiveInitializer ();

	virtual size_t get_count () const { return m_values.size(); }

	virtual void init_byte (State*, unsigned int address);
	virtual void init_half (State*, unsigned int address);
	virtual void init_word (State*, unsigned int address);
	virtual void get_referenced_labels (std::vector<AddressIdentifier*>&);
};

class RepeatIntegerDirectiveInitializer : public IntegerDirectiveInitializer {
	Identifier*	m_value;
	size_t		m_count;

	template<class Value> void init (State* s, unsigned int address);

	// Disallow copying and assignment, since we own the memory for m_value
	RepeatIntegerDirectiveInitializer (const RepeatIntegerDirectiveInitializer&) { }
	RepeatIntegerDirectiveInitializer& operator= (const RepeatIntegerDirectiveInitializer&) { return *this; }
public:
	RepeatIntegerDirectiveInitializer (Identifier* value, size_t count) : m_value(value), m_count(count) { }
	~RepeatIntegerDirectiveInitializer ();

	virtual size_t get_count () const { return m_count; }

	virtual void init_byte (State*, unsigned int address);
	virtual void init_half (State*, unsigned int address);
	virtual void init_word (State*, unsigned int address);
	virtual void get_referenced_labels (std::vector<AddressIdentifier*>&);
};

// --------------------------------------------
// Directive .byte   value
// Directive .byte   value:size
// Directive .byte   value1, value2, value3 ...
// --------------------------------------------
class ByteDirective : public Directive {
   public:
      ByteDirective () { }

      explicit ByteDirective(std::auto_ptr<IntegerDirectiveInitializer> init)
      : Directive(init->get_count() * sizeof(char)), m_init(init) { }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
	 assert(m_init.get() != NULL);
         m_init->init_byte(s, p->getAddress());
      }
      virtual void get_referenced_labels (std::vector<AddressIdentifier*>& l) {
	 assert(m_init.get() != NULL);
	 m_init->get_referenced_labels(l);
      }
      
      // @overridden from Statement
      virtual const char *getName() const { return ".byte"; }
      virtual const char *getSyntax() const { return "value  OR  value:size  OR  value1, value2, ..."; }
      virtual const char *getDescription() const {
         return "Initializes a block of one or more consecutive bytes in memory";
      }
      
   private:
      std::auto_ptr<IntegerDirectiveInitializer>	m_init;
};

// --------------------------------------------
// Directive .half   value
// Directive .half   value:size
// Directive .half   value1, value2, value3 ...
// --------------------------------------------
class HalfDirective : public Directive {
   public:
      HalfDirective () { }

      explicit HalfDirective(std::auto_ptr<IntegerDirectiveInitializer> init)
      : Directive(init->get_count() * sizeof(short)), m_init(init) { }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
	 assert(m_init.get() != NULL);
         m_init->init_half(s, p->getAddress());
      }
      virtual void get_referenced_labels (std::vector<AddressIdentifier*>& l) {
	 assert(m_init.get() != NULL);
	 m_init->get_referenced_labels(l);
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
      std::auto_ptr<IntegerDirectiveInitializer>	m_init;
};

// --------------------------------------------
// Directive .word   value
// Directive .word   value:size
// Directive .word   value1, value2, value3 ...
// --------------------------------------------
class WordDirective : public Directive {
   public:
      WordDirective () { }

      explicit WordDirective(std::auto_ptr<IntegerDirectiveInitializer> init)
      : Directive(init->get_count() * sizeof(int)), m_init(init) { }
      
      // @overridden from Directive
      virtual void initialize(ParseNode *p, State *s) {
	 assert(m_init.get() != NULL);
         m_init->init_word(s, p->getAddress());
      }
      virtual void get_referenced_labels (std::vector<AddressIdentifier*>& l) {
	 assert(m_init.get() != NULL);
	 m_init->get_referenced_labels(l);
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
      std::auto_ptr<IntegerDirectiveInitializer>	m_init;
};

// ----------------------------------
// Directive CONSTANT = imm
// ----------------------------------
inline ConstStatement::ConstStatement(AddressIdentifier *name, ImmediateIdentifier *value) : Statement(0), m_id(name), m_value(value)
{ }

inline bool ConstStatement::isInstruction() { return false; }
inline bool ConstStatement::isDirective() { return false; }

inline AddressIdentifier *ConstStatement::getID() const {
   return m_id;
}

inline ImmediateIdentifier *ConstStatement::getImmediate() const {
   return m_value;
}

// @overridden from Statement
inline const char *ConstStatement::getName() const { return ""; }
inline const char *ConstStatement::getSyntax() const { return ""; }
inline const char *ConstStatement::getDescription() const { return ""; }

#endif // __DIRECTIVES_H__

