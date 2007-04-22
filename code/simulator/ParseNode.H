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
      
      Statement* getStatement(void);
      AddressIdentifier *getLabel();
      
      int getAddress();
      TIMESTAMP getFirstExecuted(void);
      
      ParseNode* getNext(void);
      ParseNode* getPrevious(void);
      
      void execute(State* state);
      bool isValid();
      
   private:
      TIMESTAMP m_firstExecuted;
      AddressIdentifier *m_label;
      
      Statement* m_statement;
      QTextBlock* m_textBlock;

      int m_address;
};

#endif // PARSENODE_H_

