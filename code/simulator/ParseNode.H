#ifndef PARSENODE_H_
#define PARSENODE_H_

#include <QObject>
#include <QString>
#include <QTextBlockUserData>

#include "typedefs.H"
class State;
class Statement;

class ParseNode : public QTextBlockUserData {

   public:
      ParseNode(QTextBlock* textBlock, Statement* statement, QString label);

      TIMESTAMP getFirstExecuted(void);
      Statement* getStatement(void);

      ParseNode* getNext(void);
      ParseNode* getPrevious(void);
      
      int getAddress();
      
      void execute(State* state);		

   private:
      TIMESTAMP m_firstExecuted;
      QString m_label;
      Statement* m_statement;
      QTextBlock* m_textBlock;
      
      int m_address;
};

#endif /*PARSENODE_H_*/
