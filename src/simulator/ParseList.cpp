/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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
#include <limits.h>
#include <assert.h>

#include "../gui/Program.H"

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>

int ParseList::m_status = STOPPED;

ParseList::ParseList(QTextDocument *document) :
   m_source(document), m_nextTextAddress(TEXT_BASE_ADDRESS), 
   m_nextDataAddress(DATA_BASE_ADDRESS), 
   m_program(NULL)
{
   m_status = STOPPED;
}

ParseList::~ParseList() { }

void ParseList::setDocument(QTextDocument *document) {
   m_source = document;
}

void ParseList::cleanup() {
   std::list<QString> labels;
   
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next()) {
      ParseNode *p = ParseNode::Node(b);
      
      if (p == NULL)
         continue;
      
      AddressIdentifier *label = p->getLabel();
      if (label)
         labels.push_back(label->getID());
   }
   
   std::list<QString> toRemove;
   for(LabelMapIterator i = m_labelMap.begin(); i != m_labelMap.end(); ++i) {
      bool found = false;
      
      for(std::list<QString>::iterator j = labels.begin(); j != labels.end(); ++j) {
         if ((found = (*j == i.key())))
            break;
      }
      
      if (!found)
         toRemove.push_back(i.key());
   }
   
   for(std::list<QString>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
      m_labelMap.remove(*i);
   
   for(std::list<QString>::iterator i = labels.begin(); i != labels.end(); ++i)
      assert(m_labelMap.contains(*i));
}

ParseNode *ParseList::first() const {
   return ParseNode::Node(m_source->begin());
   //	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}

ParseNode *ParseList::last() const {
   return ParseNode::Node(m_source->end().previous());
}

void ParseList::setStatus(int status) {
   m_status = status;
}

ParseNode *ParseList::getNodeForAddress(unsigned int address) const {
   if (address < TEXT_BASE_ADDRESS || address >= m_nextTextAddress)
      return NULL;
   
   ParseNode *cur = first();

   
   // TODO:  start at better address point
   // and/or binary-search for ParseNode at address
   // and/or maintain Map or text addresses -> ParseNode*
   
   
   while(cur != NULL) {
      unsigned int startAddress = cur->getAddress();
      unsigned int endAddress = cur->getEndAddress();
      
      if (address >= startAddress && address < endAddress)
         return getClosestInstruction(cur, RUNNING); // closest executable instr

      cur = cur->getNext();
   }
   
   // invalid addess
   return NULL;
}

// returns non-executable .data ParseNode
ParseNode *ParseList::getNodeForDataAddress(unsigned int address) const {
   if (address < DATA_BASE_ADDRESS || address > m_nextDataAddress)
      return NULL;

   ParseNode *cur = first();

   while(cur != NULL) {
      unsigned int startAddress = cur->getAddress();
      unsigned int endAddress = cur->getEndAddress();
      
      if (address >= startAddress && address < endAddress)
         return cur;

      cur = cur->getNext();
   }
   
   // invalid addess
   return NULL;
}

// (static) Returns the closest executable ParseNode at or After the given ParseNode 
ParseNode *ParseList::getClosestInstruction(ParseNode *p, int status) {
   if (p == NULL)
      return NULL;
   
   // execution-protection for data segments
   if (ParseList::getSegment(p) == S_DATA)
      return NULL;
   
/*   if (status != m_status && status == PAUSED) { // default
      cerr << "STATUS getClosestInstruction: " << status << " vs " << m_status << endl;

      status = m_status; // for jump instructions to pause at breakpoints not on lines w/ instructions
   }*/
   
   while(!p->isExecutable()) {
      // this doesn't work because lines without executable instructions do not have addresses!
      
      /*if (p->containsBreakPoint() && status == RUNNING) {
         cerr << "BP found on line w/out instr!\n'" << p << "'\n";
         break;  // for breakpoints on lines without executable instructions
      // (for ex, to put a breakpoint on a label or a blank line inbetween instructions)
      }*/
      
      if ((p = p->getNext()) == NULL)
         return NULL;  // end of program (tested and works :)
   }
   
   return p;
}

// (static) Returns the closest executable ParseNode at or Before the given ParseNode 
ParseNode *ParseList::getClosestInstructionBefore(ParseNode *p) {
   if (p == NULL)
      return NULL;
   
   // run-protection for data segments
   if (ParseList::getSegment(p) == S_DATA)
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
         cerr << "error: line no: " << i + 1 << " is invalid  (" << p << ")\n";
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

SEGMENT ParseList::getSegment(ParseNode *node) {
   if (node == NULL)
      return S_TEXT;
   
   //ParseNode *original = node;
   QTextBlock block = *node->getTextBlock();
   
   while(node != NULL) {
      Statement *s = node->getStatement();
      
      if (s != NULL) {
         if (s->isInstruction())
            return S_TEXT;
         
         if (s->isDirective()) {
            Directive *dir = Statement::getDirective(s);
            
            if (dir->isText())
               return S_TEXT;
            if (dir->isData())
               return S_DATA;
         }
      }
      
      // else try the previous node
      block = block.previous();
      node  = node->getPrevious();
      
      if (node == NULL) {
         do {
            if (!block.isValid()) // backed up to beginning of document
               return S_TEXT;//ParseList::getForwardSegment(original);
            
            block = block.previous();
            node = ParseNode::Node(block);
         } while(node == NULL);
      }
   }
   
   return S_TEXT; // default
}

SEGMENT ParseList::getForwardSegment(ParseNode *node) {
   if (node == NULL)
      return S_TEXT;

   QTextBlock block = *node->getTextBlock();
   
   while(node != NULL) {
      Statement *s = node->getStatement();
      
      if (s != NULL) {
         if (s->isInstruction())
            return S_TEXT;
         
         if (s->isDirective()) {
            Directive *dir = Statement::getDirective(s);
            
            if (dir->isText())
               return S_TEXT;
            if (dir->isData())
               return S_DATA;
         }
      }
      
      // else try the next node
      block = block.next();
      node  = node->getNext();
      
      if (node == NULL) {
         do {
            if (!block.isValid()) // at end of doc
               return S_TEXT; // default
            
            block = block.next();
            node = ParseNode::Node(block);
         } while(node == NULL);
      }
   }
   
   return S_TEXT; // default

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
         //cerr << "\t\tDEBUG this!  cur == NULL ParseList::initialize\n\n";
         break;
      }
      Statement *s = cur->getStatement();
      
      if (s != NULL && cur->isValid())
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

// set initializeValid to true to initialize validated parsenodes
//  (for incremental compilation)
bool ParseList::insert(ParseNode *newNode, State *currentState) {
   if (newNode == NULL)
      return false;
   
/*   if (currentState)
      cerr << "Inserting: '" << newNode << "'\n";*/
   
   // Validate any ParseNodes which were waiting on this ParseNode's label 
   // to become semantically valid
   AddressIdentifier *label = newNode->getLabel();
   if (label != NULL && m_semanticErrors.contains(label->getID())) {
      ParseNodeList *waitingNodes = m_semanticErrors[label->getID()];
      
      // Update the semantic validity of all nodes which referenced newNode's label
      foreach(ParseNode *cur, *waitingNodes) {
         cur->setSemanticValidity(true);
         
         // Find the reference to newNode's label in cur's arguments
         Statement *s = cur->getStatement();

	 if (s != NULL) {
            std::vector<AddressIdentifier*> labels;
            s->get_referenced_labels(labels);

            for (std::vector<AddressIdentifier*>::iterator it(labels.begin()); it != labels.end(); ++it) {
                  if ((*it)->getID() == label->getID()) {
                     (*it)->setLabelParseNode(newNode);
		     // don't break out of the loop yet, because this label might be referenced more than once
		  }
	    }
	 }
         
         if (s != NULL && currentState != NULL && cur->isValid())
            s->initialize(newNode, currentState);

      } // foreach ParseNode waiting
      
      m_semanticErrors.remove(label->getID());
      delete waitingNodes;
   }
   
   Statement *s = newNode->getStatement();
   bool semanticallyCorrect = true;
   
   // ensure any labels referenced by this instruction are defined
   if (s != NULL) {
      std::vector<AddressIdentifier*> labels;
      s->get_referenced_labels(labels);

      for (std::vector<AddressIdentifier*>::iterator it(labels.begin()); it != labels.end(); ++it) {
            const QString &id = (*it)->getID();

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
               
               (*it)->setLabelParseNode(m_labelMap[id]->getParseNode());
               
//               cerr << "\t" << cur->getID()->getAddressIdentifier()->getParseNode() << endl;

            }
      } // for
   }
   
   
   // ------------------------------
   // Initialize ParseNode's Address
   // ------------------------------
   unsigned int address = SENTINEL_ADDRESS;

   if (s == NULL && newNode->getLabel() != NULL) {
      
      // TODO:  Hack;  when to use text, and when to use data?!
      if (getSegment(newNode) == S_TEXT) {
         if (m_nextTextAddress & 3)
            m_nextTextAddress += 4 - (m_nextTextAddress & 3);
         
         address = m_nextTextAddress;
      } else { // S_DATA
         if (m_nextDataAddress & 3)
            m_nextDataAddress += 4 - (m_nextDataAddress & 3);
         
         address = m_nextDataAddress;
      }
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
   
/*   if (currentState)
      cerr << "\tAddr of node: " << address << "\n";*/
   
   newNode->setAddress(address);
   
   if (s != NULL && currentState != NULL && newNode->isValid())
      s->initialize(newNode, currentState);
   
   return semanticallyCorrect;
}

// ------------------------------------
// Functionality for Editing-On-The-Fly
// ------------------------------------

void ParseList::updateSyntacticValidity(State *currentState, const QTextBlock &current) {
   SyntaxErrors errors;
   int lineNo = 0;
   
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next(), ++lineNo)
   {
      ParseNode *oldNode = ParseNode::Node(b);
      
      // Check for new blocks and update their validity by trying to insert them
      if (oldNode == NULL) {
         if (VERBOSE)
            cerr << "(UPDATE) attempting to parse block: '" << b.text().toStdString() << "'\n";
         
         QTextBlock *actual = new QTextBlock(b);
         ParseNode *newNode = NULL;
         
         extern std::string _tab;
         _tab = "   ";
         
         try {
            newNode = Parser::parseLine(actual, this);
         } catch(ParseError &e) {
            if (VERBOSE)
               cerr << "Error line  " << lineNo + 1 << ": " << e.toStdString() << endl;
            
            e.setTextBlock(actual);
            e.setLineNo(lineNo);
            errors.push_back(e);
            
            continue;
         }
         
         insert(newNode, currentState);
      }
   }
   
   if (!errors.empty())
      throw errors;
}

// Removes all references to a given ParseNode from this ParseList
TIMESTAMP ParseList::remove(ParseNode *node) {
   TIMESTAMP minTimestamp = MAX_TIMESTAMP;

   if (node == NULL)
      return minTimestamp;
   
   TIMESTAMP firstExecuted = node->getFirstExecuted();
   if (firstExecuted != CLEAN_TIMESTAMP) // only count timestamp if it's been run at least once (if it's "dirty")
      minTimestamp = firstExecuted;
   
   if (VERBOSE)
      cerr << "removing parseNode: '" << node << "'\n";

   QList<ParseNode*> removeAtEnd;
   
   // Remove any references to this ParseNode's label
   //    Also set semantic errors on ParseNodes which may have 
   //    been referencing that label
   AddressIdentifier *label = node->getLabel();
   if (label != NULL && m_labelMap.contains(label->getID())) {
      const QString &labelID = label->getID();
      m_labelMap.remove(labelID);
       
//      bool semanticallyCorrect = true;
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
                     // this node is referencing the label of the node that's 
                     // currently being deleted. it's now invalid, so remove it
                     
                    removeAtEnd.push_back(p);

                     /*p->setSemanticValidity(false);
                     semanticallyCorrect = false;
                     cur->getID()->getAddressIdentifier()->setLabelParseNode(NULL);
                     
                     //cerr << "semantically Invalid: '" << p->getTextBlock()->text().toStdString() << "' s = " << s << endl;
                     if (!m_semanticErrors.contains(id))
                        m_semanticErrors.insert(id, new ParseNodeList());
                     
                     m_semanticErrors[id]->push_back(p);
                     break;*/
                  }
               }
            } // for each argument of the current instruction
         }
      }  // for all text blocks
   } // if we have a label
   
   
   // -----------------------------------
   // Attempt to Update Preprocessor Maps
   // -----------------------------------
   
   Statement *s = node->getStatement();
   if (s != NULL && s->isPreprocessor()) { // consts and #defines
      ConstStatement *st = Statement::getConst(s);
      
      // NOTE:  TODO:  Note: going to have to include statements/Dir..H.. can't
      
      const QString &id = st->getID()->getID();
//      ImmediateIdentifier *imm = st->getImmediate();
      m_preProcessorMap.remove(id);
      
      // For each text block, check if it references this const, and throw any matches out!
      for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next())
      {
         if (node->getTextBlock()->position() == b.position())
            continue;
         
         ParseNode *p = ParseNode::Node(b);
         if (p != NULL && p->getText().indexOf(QRegExp(QString("\\b%1\\b").arg(id))) >= 0)
            removeAtEnd.push_back(p);
      }
   }
   
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
   
   node->notifyDeleted(true);
   
   // remove nodes which this one invalidated
   foreach(ParseNode *p, removeAtEnd) {
      TIMESTAMP newPotentialMin = remove(p);
      
      if (newPotentialMin < minTimestamp)
         minTimestamp = newPotentialMin;
   }
   
   return minTimestamp;
}

// Important:  ParseNodes call this upon getting deleted -- must notify 
//    everyone that it's been invalidated
void ParseList::notifyParseNodeDeleted(ParseNode *wasDeleted) {
   if (wasDeleted != NULL && m_program != NULL)
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

// for jumping to the declaration of an Identifier
ParseNode *ParseList::getDeclaration(const QString &identifier) {
   if (m_labelMap.contains(identifier)) {
      ParseNode *declaration = m_labelMap[identifier]->getParseNode();

      return declaration;
   }
   
   // if b contains a const -- not in m_labelMap
   for(QTextBlock b = m_source->begin(); b != m_source->end(); b = b.next())
   {
      ParseNode *p = ParseNode::Node(b);
      if (p == NULL)
         continue;
      Statement *s = p->getStatement();

      // ensure any labels referenced by this instruction are defined
      if (s != NULL && s->isPreprocessor()) {
         const QString &constID = (static_cast<ConstStatement*>(s))->getID()->getID();

         if (identifier == constID)
            return p;
      }
   }
   
   return NULL;
}

LabelMap *ParseList::getLabelMap() {
   return &m_labelMap;
}

unsigned int ParseList::getDataSize() const {
   return (m_nextDataAddress - DATA_BASE_ADDRESS);
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

