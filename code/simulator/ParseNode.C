#include "ParseNode.H"
#include "Identifier.H"
#include "Statement.H"
#include "State.H"
#include <QTextBlock>

ParseNode::ParseNode(QTextBlock* textBlock, Statement* statement, AddressIdentifier *label)
   : m_firstExecuted(CLEAN_TIMESTAMP), m_label(label), m_statement(statement), 
   m_textBlock(textBlock), m_isSemanticallyValid(false)
{
   if (m_label != NULL)
      m_label->setLabelParseNode(this);

   if (m_textBlock != NULL)
      m_textBlock->setUserData(this);
}

ParseNode *ParseNode::Node(const QTextBlock &b) {
   return static_cast<ParseNode*>(b.userData());
}

// initializes this ParseNode at the specified address, returning how many 
// consecutive bytes it will take up in the address space
/*unsigned int ParseNode::initialize(unsigned int address){
   m_address = address;
   
   if (m_statement == NULL)
      return 0;
   
   return m_statement->getSizeInBytes();
}*/

Statement* ParseNode::getStatement(void) const {
   return m_statement;
}

void ParseNode::setAddress(unsigned int address) {
   m_address = address;
}

unsigned int ParseNode::getAddress() const {  // inclusive
   return m_address;
}

unsigned int ParseNode::getSize() const {
   return (m_statement == NULL ? 0 : m_statement->getSizeInBytes());
}

unsigned int ParseNode::getEndAddress() const {  // exclusive
   return getAddress() + getSize();
}

AddressIdentifier *ParseNode::getLabel() const {
   return m_label;
}

TIMESTAMP ParseNode::getFirstExecuted(void) const {
   return m_firstExecuted;
}

QTextBlock *ParseNode::getTextBlock() const {
   return m_textBlock;
}

ParseNode* ParseNode::getNext(void) const {
   return ParseNode::Node(m_textBlock->next());
}

ParseNode* ParseNode::getPrevious(void) const {
   return ParseNode::Node(m_textBlock->previous());
}

// Returns whether or not this ParseNode contains a valid Instruction
bool ParseNode::isExecutable() const {
   return (m_statement == NULL ? false : m_statement->isInstruction());
}

void ParseNode::execute(State* state) {
   if (!isExecutable())
      return;
   
   // set firstexecuted
   if (m_firstExecuted == CLEAN_TIMESTAMP)
      m_firstExecuted = state->getCurrentTimestamp();
   
   // actually execute
   Instruction *instr = static_cast<Instruction*>(m_statement);
   instr->execute(state);
   
   // ensure PC gets updated properly (some branching instructions take care of this themselves)
   if (instr->autoIncrementPC())
      state->incrementPC();
   
   // update state's timestamp
   state->newTimestamp();
}

bool ParseNode::isValid() const {
   return (m_textBlock != NULL && m_isSemanticallyValid && m_textBlock->isValid());
}

void ParseNode::setSemanticValidity(bool isValid) {
   m_isSemanticallyValid = isValid;
}

