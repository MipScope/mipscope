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
   file: ParserList.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef PARSELIST_H_
#define PARSELIST_H_
#include "typedefs.H"
#include "Parser.H"
#include <QObject>
#include <QHash>
#include <list>

class QTextDocument;
class QTextBlock;
class parseNode;
class AddressIdentifier;
class State;
class Program; // in gui/

enum SEGMENT { S_TEXT, S_DATA };


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

class ParseList : public QObject {

   Q_OBJECT

   public:
      friend class Parser;
      
      static int m_status; // hooked w/ debugger's status only when executing instructions
      
      ParseList(QTextDocument *document);
      virtual ~ParseList();
      
      void cleanup();
      
      void setDocument(QTextDocument *document);
      
      ParseNode *first() const;
      ParseNode *last() const;
      
      // Used for jumping to a specific address
      ParseNode *getNodeForAddress(unsigned int address) const;
      // Useful for getting the ParseNode associated with a specific 
      // address, regardless of its executability (.data, for example)
      ParseNode *getNodeForDataAddress(unsigned int address) const;
      
      // (static) Returns the closest executable ParseNode at or after the given ParseNode 
      static ParseNode *getClosestInstruction(ParseNode *p, int status = PAUSED);
      
      // (static) Returns the closest executable ParseNode at or before the given ParseNode 
      static ParseNode *getClosestInstructionBefore(ParseNode *p);
      
      static SEGMENT getSegment(ParseNode *node);
      static SEGMENT getForwardSegment(ParseNode *node);

      // Returns whether or not this ParseList contains a fully-valid, runnable program (free of Syntactic and Semantic errors)
      bool isValid() const;

      SyntaxErrors *getSyntaxErrors() const;
      
      // Returns the ParseNode* containing the program's entry point (__start label or main)
      ParseNode *getEntryPoint() const;
      
      // ensures ParseList is valid and ready to run, and then initializes all 
      // static memory given the State.  returns whether or not initialization was successful
      bool initialize(State *state);
      
      // no need to tell ParseList where to insert new node; it just knows
      // cause it's da shit!
      bool insert(ParseNode *newNode, State *state = NULL);
      
      void updateSyntacticValidity(State *currentState, const QTextBlock &current);

      // ensures all maps get updated; returns earliest valid timestamp
      // of any secondary nodes which had to be removed as a result of the 
      // node passed in being removed
      TIMESTAMP remove(ParseNode *node);
      
      void notifyParseNodeDeleted(ParseNode *wasDeleted);
      
      // for notifying program of changes to this ParseList
      //    (for editing on-the-fly)
      void setInteractiveProgram(Program *program);
      
      Program *getProgram() const;
      
      // for jumping to the declaration of an Identifier
      ParseNode *getDeclaration(const QString &identifier);
      
      LabelMap *getLabelMap();
      unsigned int getDataSize() const;
      
      static void setStatus(int status);
      
   private:
      QTextDocument *m_source;
      SemanticErrors m_semanticErrors;
      
      // LabelMap and PreProcessorMap used by Parser::parseLine
      LabelMap m_labelMap;
      PreProcessorMap m_preProcessorMap;
      
      unsigned int m_nextTextAddress;
      unsigned int m_nextDataAddress;
      
      // interface/proxy between Gui and active program
      // ( located in ../gui/ )
      Program *m_program;
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

