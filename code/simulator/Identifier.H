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
      AddressIdentifier *getAddressIdentifier() const;
      
   protected:
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
      
   protected:
      ParseNode *m_parseNode;
      int m_offset;
      bool m_initialized; // true once setLabelParseNode() has been called
};

// for something like li $r0, 0x1e   -- storing the 0x1e -> 31 mapping
class ImmediateIdentifier : public Identifier {
   public:
      ImmediateIdentifier(const QString &id, int value);
      
      // Returns the value of this constant
      int getValue() const;
      
   protected:
      int m_value;
};


#endif // __IDENTIFIER_H__

