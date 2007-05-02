/* ---------------------------------------------- *\
   file: ParserList.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#include "ParseList.H"
#include "Parser.H"
#include "ParseNode.H"
#include "StatementArgList.H"
#include "Identifier.H"
#include "Statement.H"
#include "State.H"

#include "../gui/Program.H"

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>

ParseList::ParseList(QTextDocument *document) :
   m_source(document), m_nextTextAddress(TEXT_BASE_ADDRESS), 
   m_nextDataAddress(DATA_BASE_ADDRESS), m_program(NULL)
{ }

ParseList::~ParseList() { }

ParseNode *ParseList::first() const {
   return ParseNode::Node(m_source->begin());
   //	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}

ParseNode *ParseList::last() const {
   return ParseNode::Node(m_source->end().previous());
}

ParseNode *ParseList::getNodeForAddress(unsigned int address) const {
   if (address < TEXT_BASE_ADDRESS || address >= m_nextTextAddress)
      return NULL;
   
   ParseNode *cur = getEntryPoint();

   
   // TODO:  start at better address point
   // and/or binary-search for ParseNode at address
   // and/or maintain Map or text addresses -> ParseNode*
   

   while(cur != NULL) {
      unsigned int startAddress = cur->getAddress();
      unsigned int endAddress = cur->getEndAddress();
      
      if (address >= startAddress && address < endAddress)
         return getClosestInstruction(cur); // closest executable instr

      cur = cur->getNext();
   }
   
   // invalid addess
   return NULL;
}

// (static) Returns the closest executable ParseNode at or After the given ParseNode 
ParseNode *ParseList::getClosestInstruction(ParseNode *p) {
   if (p == NULL)
      return NULL;
   
   while(!p->isExecutable()) {
      if ((p = p->getNext()) == NULL)
         return NULL;  // end of program (tested and works :)
   }
   
   return p;
}

// (static) Returns the closest executable ParseNode at or Before the given ParseNode 
ParseNode *ParseList::getClosestInstructionBefore(ParseNode *p) {
   if (p == NULL)
      return NULL;
   
   while(!p->isExecutable()) {
      if ((p = p->getPrevious()) == NULL)
         return NULL;  // start of source, I believe
   }
   
   return p;
}


// Returns whether or not this ParseList contains a fully-valid, runnable program (free of Syntactic and Semantic errors)
bool ParseList::isValid() const {
   /*if (m_semanticErrors.size() > 0) {
      cerr << m_semanticErrors.size() << " semantic errors remain!\n";
      foreach(ParseNodeList *l, m_semanticErrors.values()) {
         foreach(ParseNode *e, *l) {
            cerr << e << endl;
         }
      }

      return false;
   }
   
   // ensure prog contains valid entry point
   if (getEntryPoint() == NULL) {
      cerr << "error: no program entryPoint found\n";
      return false; // TODO:  notify user of this
   }
   int i = 0;
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next()) {
      ParseNode *p = ParseNode::Node(b);
      
      if (p == NULL || !p->isValid()) {
         cerr << "error: line no: " << i << " is invalid  (" << p << ")\n";
         return false;
      }
      ++i;
   }

   return true;*/
   
   return (getSyntaxErrors()->size() == 0);
}

SyntaxErrors *ParseList::getSyntaxErrors() const {
   SyntaxErrors *errors = new SyntaxErrors();
   
   if (m_semanticErrors.size() > 0) {
      foreach(QString s, m_semanticErrors.keys()) {
         foreach(ParseNode *e, *m_semanticErrors[s]) {
            ParseError p(QString("Undefined reference to label '%1'").arg(s), s, s.length(), e->getTextBlock());

            errors->push_back(p);
         }
      }
   }

   if (getEntryPoint() == NULL)
      errors->push_back(ParseError(QString("Could not locate program entry point; Must define label '"MAIN"' or '"__START"'"), ""));
   
   // Ensure only instructions are in .text and data is in .data
   enum SEGMENT { S_TEXT, S_DATA };

   SEGMENT segment = S_TEXT; // text is the default
   
   int lineNo = 0;
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next(), ++lineNo) {
      ParseNode *p = ParseNode::Node(b);
      Statement *s = NULL;
      
      if (p != NULL && p->isValid() && (s = p->getStatement()) != NULL) {
         if (s->isDirective()) {
            Directive *dir = Statement::getDirective(s);

            if (dir->isData())
               segment = S_DATA;
            else if (dir->isText())
               segment = S_TEXT;
            else if (segment == S_TEXT) { // we have data in a .text section
               ParseError parseError(QString("Data can only appear in a '.data' segment"), b.text(), b.text().length(), p->getTextBlock(), lineNo);
               
               errors->push_back(parseError);
            }
         } else if (s->isInstruction() && segment == S_DATA) {
            ParseError parseError(QString("Text can only appear in a '.text' segment"), b.text(), b.text().length(), p->getTextBlock(), lineNo);

            errors->push_back(parseError);
         }
      }
   }
   
   return errors;
}

// Returns the ParseNode* containing the program's entry point (__start label or main)
ParseNode *ParseList::getEntryPoint() const {
   ParseNode *entryPoint = NULL;
   
   //cerr << m_labelMap.size() << ", " << m_labelMap[MAIN]->getParseNode()->isValid() << endl;
   if (m_labelMap.contains(__START) && (entryPoint = m_labelMap[__START]->getParseNode()) != NULL && entryPoint->isValid())
      return entryPoint;
   if (m_labelMap.contains(MAIN) && (entryPoint = m_labelMap[MAIN]->getParseNode()) != NULL && entryPoint->isValid())
      return entryPoint;
  
   return NULL;
}

// ensures ParseList is valid and ready to run, and then initializes all 
// static memory given the State.  returns whether or not initialization was successful.
bool ParseList::initialize(State *state) {
   if (!this->isValid())
      return false;
   
   state->reset();
   ParseNode *l = last();
   for(ParseNode *cur = first(); cur != l && cur != NULL; cur = cur->getNext()) {
      if (cur == NULL) {
         cerr << "\t\tDEBUG this!  cur == NULL ParseList::initialize\n\n";
         break;
      }
      Statement *s = cur->getStatement();
      
      if (s != NULL)
         s->initialize(cur, state);
   }
   
   state->setRegister(gp, DATA_BASE_ADDRESS);
   state->setRegister(sp, STACK_BASE_ADDRESS - 4);

   // Move PC to closest executable instruction after the entry point at __start: or main:
   state->setPC(getClosestInstruction(getEntryPoint()));

   // Important:  has to go last!
   state->newTimestamp();
   return true;
}

bool ParseList::insert(ParseNode *newNode) {
   if (newNode == NULL)
      return false;
   
   // Validate any ParseNodes which were waiting on this ParseNode's label 
   // to become semantically valid
   AddressIdentifier *label = newNode->getLabel();
   if (label != NULL && m_semanticErrors.contains(label->getID())) {
      ParseNodeList *waitingNodes = m_semanticErrors[label->getID()];
      
      foreach(ParseNode *cur, *waitingNodes) {
         cur->setSemanticValidity(true);
         
         Statement *s = cur->getStatement();
         if (s != NULL && s->isInstruction()) {
            StatementArgList *args = (static_cast<Instruction*>(s))->getArguments();

            for(int i = 0; i < args->noArgs(); i++) {
               StatementArg *cur = (*args)[i];

               if (cur->hasAddressIdentifier()) {
                  Identifier *addrID = cur->getID();
                  const QString &id = addrID->getID();
                  if (id == label->getID() && addrID->isAddress()) {
                     (static_cast<AddressIdentifier*>(addrID))->setLabelParseNode(newNode);
                     break;
                  }
               }
            } // for
         } // if statement is an instruction
      } // foreach ParseNode waiting
      
      m_semanticErrors.remove(label->getID());
      delete waitingNodes;
   }
   
   Statement *s = newNode->getStatement();
   bool semanticallyCorrect = true;
   
   // ensure any labels referenced by this instruction are defined
   if (s != NULL && s->isInstruction()) {
      StatementArgList *args = (static_cast<Instruction*>(s))->getArguments();
      
      for(int i = 0; i < args->noArgs(); i++) {
         StatementArg *cur = (*args)[i];
         
         if (cur->hasAddressIdentifier()) {
            const QString &id = cur->getID()->getID();

            if (!m_labelMap.contains(id)) {
               // this node is referencing some label which currently doesn't exist
               // wait for the label to appear, and only then validate it
               newNode->setSemanticValidity(false);
               semanticallyCorrect = false;
               
               
               //cerr << "semantically Invalid: '" << newNode->getTextBlock()->text().toStdString() << "' s = " << s << endl;
               
               
               if (!m_semanticErrors.contains(id))
                  m_semanticErrors.insert(id, new ParseNodeList());
               
               m_semanticErrors[id]->push_back(newNode);
               break;
            } else { // This ParseNode references a label which has already been defined
//               cerr << id.toStdString() << " set to " << m_labelMap[id]->getParseNode() << ",   parsing: " << newNode << endl;
               
               cur->getID()->getAddressIdentifier()->setLabelParseNode(m_labelMap[id]->getParseNode());
               
//               cerr << "\t" << cur->getID()->getAddressIdentifier()->getParseNode() << endl;

            }
         }
      } // for
   }
   
   
   // ------------------------------
   // Initialize ParseNode's Address
   // ------------------------------
   unsigned int address = SENTINEL_ADDRESS;

   if (s == NULL && newNode->getLabel() != NULL) {

      // TODO:  Hack;  when to ues text, and when to use data?!
      if (m_nextTextAddress & 3)
         m_nextTextAddress += 4 - (m_nextTextAddress & 3);

      address = m_nextTextAddress;
   } else if (s != NULL) {
      unsigned int *addr = NULL;
      
      if (s->isInstruction())
         addr = &m_nextTextAddress;
      else if (s->isDirective())
         addr = &m_nextDataAddress;
      
      if (addr != NULL) {
         unsigned int alignment = s->getPreferredAlignment() << 1;
         address = *addr;
         ParseNode *prev = newNode->getPrevious();
         bool aligned = false;
         
         if (prev != NULL) { // enforce .align directive
            Statement *s = prev->getStatement();

            if (s != NULL && s->isDirective()) {
               Directive *dir = (static_cast<Directive*>(s));

//               cerr << dir->isAlign() << endl;
               if (dir->isAlign()) {
                  alignment = dir->getPreferredAlignment() << 1;
                  aligned = true;
//                  cerr << "\t\taligned at: " << alignment << ", addr = " << address << endl;
               }
            }
         }
         
         if (alignment > 0 && address & (alignment - 1)) {
            address += (alignment - (address & (alignment - 1)));

//            cerr << *addr << " vs " << address << ", " << alignment << " aligned=" << aligned << ", " << newNode << endl;
//            assert(address > *addr);
         }
         
         //cerr << "oldAddr: " << *addr;
         *addr = address + s->getSizeInBytes();
         //cerr << "  vs newAddr: " << *addr << endl;
      }
   }
   
   newNode->setAddress(address);
   
   return semanticallyCorrect;
}

// ------------------------------------
// Functionality for Editing-On-The-Fly
// ------------------------------------

void ParseList::updateSyntacticValidity() {
   SyntaxErrors errors;
   int lineNo = 0;
   
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next(), ++lineNo)
   {
      ParseNode *oldNode = ParseNode::Node(b);
      
      // Check for new blocks and update their validity by trying to insert them
      if (oldNode == NULL) {
         cerr << "(UPDATE) attempting to parse block: '" << b.text().toStdString() << "'\n";

         QTextBlock *actual = new QTextBlock(b);
         ParseNode *newNode = NULL;
         
         extern std::string _tab;
         _tab = "   ";
         
         try {
            newNode = Parser::parseLine(actual, this);
         } catch(ParseError &e) {
            cerr << "Error line  " << lineNo << ": " << e.toStdString() << endl;
            e.setTextBlock(actual);
            e.setLineNo(lineNo);
            errors.push_back(e);
            
            continue;
         }
         
         insert(newNode);
      }
   }

   if (!errors.empty())
      throw errors;
}

// Removes all references to a given ParseNode from this ParseList
void ParseList::remove(ParseNode *node) {
   if (node == NULL)
      return;
   
   cerr << "removing parseNode: '" << node << "'\n";
   
   // Remove any references to this ParseNode's label
   //    Also set semantic errors on ParseNodes which may have 
   //    been referencing that label
   AddressIdentifier *label = node->getLabel();
   if (label != NULL && m_labelMap.contains(label->getID())) {
      const QString &labelID = label->getID();
      m_labelMap.remove(labelID);
       
      bool semanticallyCorrect = true;
      for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next())
      {
         if (node->getTextBlock()->position() == b.position())
            continue; // TODO:  ??
         
         ParseNode *p = ParseNode::Node(b);
         if (p == NULL)
            continue;
         Statement *s = p->getStatement();
         
         // ensure any labels referenced by this instruction are defined
         if (s != NULL && s->isInstruction()) {
            StatementArgList *args = (static_cast<Instruction*>(s))->getArguments();
            
            for(int i = 0; i < args->noArgs(); i++) {
               StatementArg *cur = (*args)[i];

               if (cur->hasAddressIdentifier()) {
                  const QString &id = cur->getID()->getID();
                  
                  if (!m_labelMap.contains(id)) {
                     // this node is referencing some label which currently doesn't exist
                     // wait for the label to appear, and only then validate it
                     p->setSemanticValidity(false);
                     semanticallyCorrect = false;
                     cur->getID()->getAddressIdentifier()->setLabelParseNode(NULL);

                     
                     // TODO:  take out this assert
                     assert (cur->getID()->getAddressIdentifier()->getParseNode() == node); // that we're deleting.. not sure if this is a valid assertion..
                     

                     
                     //cerr << "semantically Invalid: '" << p->getTextBlock()->text().toStdString() << "' s = " << s << endl;
                     if (!m_semanticErrors.contains(id))
                        m_semanticErrors.insert(id, new ParseNodeList());
                     
                     m_semanticErrors[id]->push_back(p);
                     break;
                  }
               }
            } // for each argument of the current instruction
         }
      }  // for all text blocks
   } // if we have a label
   
   
   
   // TODO:  if node contains etwas in m_preProcessorMap...
   
   
   // if this node was in a semantic error list, remove it
   foreach(QString s, m_semanticErrors.keys()) {
      ParseNodeList *errorList = m_semanticErrors[s];
      foreach(ParseNode *e, *errorList) {
         if (e == node) {
            errorList->remove(e);
            if (errorList->empty())
               m_semanticErrors.remove(s);
            break;
         }
      }
   }

   node->notifyDeleted();
}

// Important:  ParseNodes call this upon getting deleted -- must notify 
//    everyone that it's been invalidated
void ParseList::notifyParseNodeDeleted(ParseNode *wasDeleted) {
   if (wasDeleted == NULL)
      return;
   
   if (m_program != NULL)
      m_program->notifyParseNodeDeleted(wasDeleted);
}

// for notifying program of changes to this ParseList
//    (for editing on-the-fly)
void ParseList::setInteractiveProgram(Program *program) {
   m_program = program;
}

Program *ParseList::getProgram() const {
   return m_program;
}

SemanticError::SemanticError(const QString &description, const QString &unrecognized, ParseNode *parseNode) 
   : ParseError(description, unrecognized), m_parseNode(parseNode)
{
   if (m_parseNode != NULL)
      setTextBlock(m_parseNode->getTextBlock());
}

ParseNode *SemanticError::getParseNode() const {
   return m_parseNode;
}

