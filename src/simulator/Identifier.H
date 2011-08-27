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
   file: Identifier.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/20/2007
\* ---------------------------------------------- */
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__
#include <QString>

class ParseNode;
class AddressIdentifier;

class Identifier {
   public:
      Identifier(const QString &id);
      virtual ~Identifier() { }
      
      QString getID() const;
      virtual int getValue() const = 0;
      
      bool isImmediate() const;
      bool isAddress() const;
      const AddressIdentifier* getAddressIdentifier() const;
      AddressIdentifier* getAddressIdentifier();
      
   private:
      QString m_id;
};

class AddressIdentifier : public Identifier {
   public:
      AddressIdentifier(const QString &id, ParseNode*);
      
      // Returns the address of the ParseNode corresponding to this:
      // Label or data label  plus an optional offset (to handle label + imm)
      int getValue() const;
      int getOffset() const;
      void setOffset(int);
      
      void setLabelParseNode(ParseNode *p);

      // Returns the ParseNode containing the label which corresponds to this identifier
      ParseNode *getParseNode() const;
      
   private:
      ParseNode *m_parseNode;
      int m_offset;
};

// for something like li $r0, 0x1e   -- storing the 0x1e -> 31 mapping
class ImmediateIdentifier : public Identifier {
   public:
      ImmediateIdentifier(const QString &id, int value);
      
      // Returns the value of this constant
      int getValue() const;
      
   private:
      int m_value;
};


#endif // __IDENTIFIER_H__

