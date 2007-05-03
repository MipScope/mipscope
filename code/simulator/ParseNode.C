/* ---------------------------------------------- *\
   file: ParseNode.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#include "ParseNode.H"
#include "ParseList.H"
#include "Identifier.H"
#include "Statement.H"
#include "State.H"
#include "../gui/Utilities.H"
#include <QTextBlock>


PlaceHolder::PlaceHolder(ParseNode *parent, QTextBlock *textBlock) 
   : QTextBlockUserData(), m_parent(parent), m_textBlock(textBlock)
{
   if (m_textBlock != NULL)
      m_textBlock->setUserData(this);
}

PlaceHolder::~PlaceHolder() {
   if (m_parent != NULL)
      m_parent->notifyDeleted();
}

ParseNode::ParseNode(ParseList *parent, QTextBlock* textBlock, Statement* statement, AddressIdentifier *label)
   : m_firstExecuted(CLEAN_TIMESTAMP), m_label(label), m_parseList(parent), 
   m_statement(statement), m_textBlock(textBlock), 
   m_placeHolder(new PlaceHolder(this, textBlock)), m_isSemanticallyValid(true)
{
   if (textBlock != NULL) {
      m_text = textBlock->text();
      Parser::trimCommentsAndWhitespace(m_text);
   } else m_text = "";
   
//   cerr << "<<< " << m_statement << ", '" << m_textBlock->text().toStdString() << "'\n";
   
   if (m_label != NULL)
      m_label->setLabelParseNode(this);
   
      //cerr << m_label->getID().toStdString() << " set (in constructor) to " << this << endl;
}

void ParseNode::notifyDeleted(bool alreadyKnown) {
   m_placeHolder = NULL;  // keep track that we were deleted
   
   // for editing-on-the-fly, have to know when a ParseNode gets deleted.. messy
   if (!alreadyKnown)
      m_parseList->notifyParseNodeDeleted(this);
}

ParseNode *ParseNode::Node(const QTextBlock &b) {
   if (!b.isValid() || b.userData() == NULL)
      return NULL;
   
   PlaceHolder *placeHolder = static_cast<PlaceHolder*>(b.userData());
   
   if (placeHolder == NULL || placeHolder->m_parent->m_placeHolder == NULL)
      return NULL; // ensure it hasn't been deleted
   
   return placeHolder->m_parent;
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
   /*if (m_textBlock->text() == QString("main:"))
      cerr << "equal\n\n\n\n";
   cerr << "\t'" << m_textBlock->text().toStdString() << "' address is: " << m_address << endl;*/
   
   m_address = address;
}

unsigned int ParseNode::getAddress() const {  // inclusive
   //cerr << "\t'" << m_textBlock->text().toStdString() << "' address is: " << m_address << endl;
   
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

void ParseNode::setFirstExecuted(TIMESTAMP timestamp) {
   m_firstExecuted = timestamp;
}

TIMESTAMP ParseNode::getFirstExecuted(void) const {
   return m_firstExecuted;
}

QTextBlock *ParseNode::getTextBlock() const {
   return m_textBlock;
}

bool ParseNode::containsBreakPoint() const {
   int blockState;
   
   return (m_textBlock != NULL && ((blockState = m_textBlock->userState()) >= 0) && (blockState & B_BREAKPOINT));
}

ParseNode* ParseNode::getNext(void) const {
//   cerr << m_textBlock << endl;
//   cerr << ", " << (*m_textBlock == QTextBlock()) << endl;
//   cerr << ", " << (m_textBlock->next() == QTextBlock()) << endl;
//   cerr << m_textBlock->isValid() << endl;
   if (m_placeHolder == NULL || m_textBlock->next() == QTextBlock() || !m_textBlock->next().isValid())
      return NULL;
   
   return ParseNode::Node(m_textBlock->next());
}

ParseNode* ParseNode::getPrevious(void) const {
   if (m_placeHolder == NULL)
      return NULL;

   return ParseNode::Node(m_textBlock->previous());
}

// Returns whether or not this ParseNode contains a valid Instruction
bool ParseNode::isExecutable() const {
   return (m_statement == NULL ? false : m_statement->isInstruction());
}

void ParseNode::execute(State* state, ParseList* parseList) {
   bool increment = true, executed;
   
   if ((executed = isExecutable())) {
      // set firstexecuted
      if (m_firstExecuted == CLEAN_TIMESTAMP)
         m_firstExecuted = state->getCurrentTimestamp();

      // actually execute
      Instruction *instr = static_cast<Instruction*>(m_statement);
      instr->execute(state, parseList);

      // ensure PC gets updated properly (some branching instructions take care of this themselves)
      increment = instr->autoIncrementPC();
   }

   if (increment)
      state->incrementPC();
   
   // update state's timestamp
   if (executed)
      state->newTimestamp();
}

bool ParseNode::isValid() const {
//   cerr << m_textBlock << ", " << m_isSemanticallyValid << ", " << m_textBlock->isValid() << endl;
   return (m_placeHolder != NULL && m_textBlock != NULL && m_isSemanticallyValid && m_textBlock->isValid());
}

void ParseNode::setSemanticValidity(bool isValid) {
   m_isSemanticallyValid = isValid;
}

void ParseNode::setText(const QString &text) {
   m_text = text;
}

QString ParseNode::getText() const {
   return m_text;
}

