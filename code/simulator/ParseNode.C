#include "ParseNode.H"
#include "Identifier.H"
#include "Statement.H"
#include "State.H"
#include <QTextBlock>

ParseNode::ParseNode(QTextBlock* textBlock, Statement* statement, AddressIdentifier *label)
   : m_firstExecuted(CLEAN_TIMESTAMP), m_label(label), m_statement(statement), 
   m_textBlock(textBlock)
{
   if (m_label != NULL)
      m_label->setLabelParseNode(this);

   if (m_textBlock != NULL)
      m_textBlock->setUserData(this);
}

Statement* ParseNode::getStatement(void) {
   return m_statement;
}

int ParseNode::getAddress() {
   return m_address;
}

AddressIdentifier *ParseNode::getLabel() {
   return m_label;
}

TIMESTAMP ParseNode::getFirstExecuted(void) {
   return m_firstExecuted;
}

ParseNode* ParseNode::getNext(void) {
   // TODO

   return NULL;
}

ParseNode* ParseNode::getPrevious(void) {
   // TODO
   
   return NULL;
}

void ParseNode::execute(State* state) {
   // set firstexecuted
//   if (m_firstExecuted == CLEAN_TIMESTAMP)
//      m_firstExecuted = state->//timestamp;
   
   // actually execute
}

bool ParseNode::isValid() {
   return (m_textBlock != NULL && m_textBlock->isValid());
}

