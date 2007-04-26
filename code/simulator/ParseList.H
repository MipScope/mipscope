/* ---------------------------------------------- *\
   file: ParserList.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef PARSELIST_H_
#define PARSELIST_H_
#include "typedefs.H"
#include "Parser.H"
#include <QHash>
#include <list>

class QTextDocument;
class QTextBlock;
class parseNode;
class AddressIdentifier;
class State;

// Two valid entry points for program
#define __START   "__start"
#define MAIN      "main"

// map of SemanticErrors; QString -> list<ParseNode*>;
// mark those ParseNodes invalid.
//    upon successful insert(), update list of SemanticErrors for which 
// any AddressIdentifiers in that ParseNode that was inserted were declared.
//    upon unsuccessful insert(), add ParseNode* to list of SemanticErrors
// under the key of the AddressID that caused the semantic error.
//
//    Only valid ParseList if m_semanticErrors map.size() is zero.

class ParseList {

   public:
      friend class Parser;
      
      ParseList(QTextDocument *document);
      
      ParseNode *first() const;
      ParseNode *last() const;
      
      // Used for jumping to a specific address
      ParseNode *getNodeForAddress(unsigned int address) const;
      
      // (static) Returns the closest executable ParseNode at or after the given ParseNode 
      static ParseNode *getClosestInstruction(ParseNode *p);

      // Returns whether or not this ParseList contains a fully-valid, runnable program (free of Syntactic and Semantic errors)
      bool isValid() const;
      
      // Returns the ParseNode* containing the program's entry point (__start label or main)
      ParseNode *getEntryPoint() const;
      
      // ensures ParseList is valid and ready to run, and then initializes all 
      // static memory given the State.  returns whether or not initialization was successful.
      bool initialize(State *state);
      
      // no need to tell ParseList where to insert new node; it just knows
      // cause it's da shit!
      // throws SemanticError upon failure
      bool insert(ParseNode *newNode);
      
      // ensures all maps get updated
      void remove(ParseNode *node);
      void remove(QTextBlock &block);
      
   private:
      QTextDocument *m_source;
      SemanticErrors m_semanticErrors;
      
      // LabelMap and PreProcessorMap used by Parser::parseLine
      LabelMap m_labelMap;
      PreProcessorMap m_preProcessorMap;
      
      unsigned int m_nextTextAddress;
      unsigned int m_nextDataAddress;
};

class SemanticError : public ParseError {
// TODO:
   // AddressIdentifier used but not found
   // AddressIdentifier declared twice -> here is previous declaration
   // program entry point not found ( __start: or main: )
   // Using .data AddressIdentifier as destination in branch/jump (warning?)
   
   public:
      SemanticError(const QString &description, const QString &unrecognized, ParseNode *parseNode = NULL);
      
      ParseNode *getParseNode() const;
      
   protected:
      // invalid parseNode until this SemanticError is deleted :)
      ParseNode *m_parseNode;
};


#endif // PARSELIST_H_

