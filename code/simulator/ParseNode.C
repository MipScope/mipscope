#include "ParseNode.H"
#include "Statement.H"
#include <QTextBlock>

ParseNode::ParseNode(QTextBlock* textBlock, Statement* statement, QString label)
   : m_firstExecuted(CLEAN_TIMESTAMP), m_label(label), m_statement(statement), 
   m_textBlock(textBlock)
{
   
}

TIMESTAMP ParseNode::getFirstExecuted(void) {
   return m_firstExecuted;
}

Statement* ParseNode::getStatement(void) {
   
}

ParseNode* ParseNode::getNext(void) {
   
}

ParseNode* ParseNode::getPrevious(void) {
   
}

void ParseNode::execute(State* state);	 {
   // set firstexecuted
   if (m_firstExecuted == CLEAN_TIMESTAMP)
      m_firstExecuted = state->//timestamp;
   
   // actually execute
}

