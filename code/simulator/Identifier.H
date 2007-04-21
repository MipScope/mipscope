/* ---------------------------------------------- *\
   file: Identifier.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/20/2007
\* ---------------------------------------------- */
#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

class ParseNode;

class Identifier {
   public:
      Identifier(QString id);
      
      QString getID() const;
      int getValue() const = 0;
      
   protected:
      QString m_id;
};

class AddressIdentifier : Identifier {
   public:
      AddressIdentifier(const QString &id, ParseNode*);
      
      // Returns the address of the ParseNode corresponding to this:
      // Label or data label
      int getValue() const;
      
      void setLabelParseNode(ParseNode *p);
      
   protected:
      ParseNode *m_parseNode;
};

// for something like li $r0, 0x1e   -- storing the 0x1e -> 31 mapping
class ImmediateIdentifier : Identifier {
   public:
      ImmediateIdentifier(const QString &id, int value);
      
      // Returns the value of this constant
      int getValue() const;
      
   protected:
      int m_value;
};


#endif // __IDENTIFIER_H__

