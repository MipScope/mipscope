#ifndef PARSENODE_H_
#define PARSENODE_H_

#include <QTextBlockUserData>
#include "typedefs.H"

class AddressIdentifier;
class Statement;
class State;

class ParseNode : public QTextBlockUserData {
   public:
      ParseNode(QTextBlock* textBlock, Statement* statement = NULL, AddressIdentifier *label = NULL);
      
      static ParseNode *Node(const QTextBlock &b); // utility method
      
      Statement* getStatement(void) const;
      AddressIdentifier *getLabel() const;
      
      void setAddress(unsigned int address);
      unsigned int getAddress() const;  // inclusive
      unsigned int getSize() const;
      unsigned int getEndAddress() const;  // exclusive
      
      TIMESTAMP getFirstExecuted(void) const;
      QTextBlock *getTextBlock() const;
      
      ParseNode* getNext(void) const;
      ParseNode* getPrevious(void) const;

      // Returns whether or not this ParseNode contains a valid Instruction
      bool isExecutable() const;
      void execute(State* state);
      bool isValid() const;
      void setSemanticValidity(bool isValid);
      
   private:
      TIMESTAMP m_firstExecuted;
      AddressIdentifier *m_label;
      
      Statement* m_statement;
      QTextBlock* m_textBlock;

      unsigned int m_address;
      
      // Set in ParseList::insert() upon successful insertion
      bool m_isSemanticallyValid;
};

#endif // PARSENODE_H_

