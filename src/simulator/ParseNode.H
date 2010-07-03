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
   file: ParseNode.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef PARSENODE_H_
#define PARSENODE_H_

#include <QTextBlockUserData>
#include <QString>
#include "typedefs.H"
#include <ostream>

class AddressIdentifier;
class Statement;
class State;
class ParseList;
class ParseNode;

class PlaceHolder : public QTextBlockUserData {
   public:
      PlaceHolder(ParseNode *parent, QTextBlock *textBlock);
      virtual ~PlaceHolder();

      ParseNode *m_parent;
      QTextBlock *m_textBlock;
};

class ParseNode {
   public:
      ParseNode(ParseList *parent, QTextBlock* textBlock, Statement* statement = NULL, AddressIdentifier *label = NULL);
      
      static ParseNode *Node(const QTextBlock &b); // utility method
      static ParseNode *Node(const QTextBlock *b); // utility method
      
      void notifyDeleted(bool alreadyKnown = false);
      
      Statement* getStatement(void) const;
      AddressIdentifier *getLabel() const;
      
      // returns true if there is a breakpoint set on this ParseNode
      bool containsBreakPoint() const;
      
      void setAddress(unsigned int address);
      unsigned int getAddress() const;  // inclusive
      unsigned int getSize() const;
      unsigned int getEndAddress() const;  // exclusive
      
      TIMESTAMP getFirstExecuted(void) const;
      void setFirstExecuted(TIMESTAMP timestamp);
      QTextBlock *getTextBlock() const;
      
      ParseNode* getNext(void) const;
      ParseNode* getPrevious(void) const;

      // Returns whether or not this ParseNode contains a valid Instruction
      bool isExecutable() const;
      void execute(State* state, ParseList* parseList, int status);
      bool isValid() const;
      void setSemanticValidity(bool isValid);

      void setText(const QString &text);
      QString getText() const;
      
      friend std::ostream& operator<<(std::ostream &o, ParseNode *p) {
         return o << p->m_text.toStdString();
         /*if (p->m_textBlock == NULL)
            return o << "{NULL Text Block}";
         
         return o << (p->m_textBlock->text().toStdString());*/
      }
      
   private:
      TIMESTAMP m_firstExecuted;
      AddressIdentifier *m_label;
      
      // ParseList to which this ParseNode belongs
      ParseList *m_parseList;
      Statement* m_statement;
      QTextBlock* m_textBlock;
      PlaceHolder *m_placeHolder;

      unsigned int m_address;
      
      // Set in ParseList::insert() upon successful insertion
      bool m_isSemanticallyValid;
      QString m_text;
};

#endif // PARSENODE_H_

