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
   file: Parser.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#include "Parser.H"
#include "ParseList.H"
#include "ParseNode.H"
#include "StatementArgList.H"
#include "Identifier.H"
#include "Statement.H"
#include "../gui/Utilities.H"
#include "statements/Directives.H"
#include <assert.h>

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QRegExp>
#include <QtCore>
#include <string>

string _tab;

//extern bool VERBOSE;


ParseList *Parser::parseDocument(QTextDocument *document) {
   ParseList *parseList = new ParseList(document);
   vector<ParseNode*> parseNodes;
   SyntaxErrors errors;
   int lineNo = 0;
   
   for(QTextBlock cur = document->begin(); cur != document->end(); cur = cur.next(), lineNo++) {
      ParseNode *n = NULL;
      QTextBlock *actual = new QTextBlock(cur);
      
      _tab = "   ";
      
      try {
         n = Parser::parseLine(actual, parseList);
      } catch(ParseError &e) {
         if (VERBOSE)
            cerr << "Error line  " << lineNo + 1 << ": " << e.toStdString() << endl;
         e.setTextBlock(actual);
         e.setLineNo(lineNo);
         errors.push_back(e);
         
         continue;
      }
      
//      assert (n != NULL);
      // Setup n in dis hizhouse
      parseNodes.push_back(n);
   }
   
   parseList->cleanup();
   
   if (!errors.empty()) {
      delete parseList;
      throw errors;  // (vector<ParseError>)
   }
   
   // do this afterwards so labels defined later above are already in list's maps
   foreach(ParseNode *cur, parseNodes)
      parseList->insert(cur);
   
   return parseList;
}

// Returns NULL if b does not contain a syntactically-valid line
ParseNode *Parser::parseLine(QTextBlock *b, ParseList *list) {
   QString text = b->text();

   // Clear out the user data to make sure that a failed parsing doesn't leave any residue
   b->setUserData(NULL);
   
   // Remove any comments and extraneous leading/trailing whitespace
   Parser::trimCommentsAndWhitespace(text);
   int index;
   
   if (VERBOSE) {
      cerr << "Parsing line: " << text.toStdString() << endl;
   }
   string orig = _tab;
   _tab += "   ";
      
   //if (VERBOSE) cerr << _tab << "Removed comment: '" << text.toStdString() << "' is left\n";
   
   if (text.isEmpty()) {
      if (VERBOSE) {
         cerr << _tab << "Recognized as blank or comment\n";
      }
      
      _tab = orig;
      return new ParseNode(list, b);
   }
   
   // ----------------
   // Check for Labels
   // ----------------
   AddressIdentifier *label = NULL;

   if ((index = text.indexOf(':')) >= 0) {
      // we have a label; possibly an instruction as well
      if (index <= 0)
         PARSE_ERRORL("misplaced ':'", text, 1);
      
      QString labelStr = text.section(':', 0, 0);
      if ((label = parseLabel(labelStr)) != NULL) {

         // remove label from text and continue parsing
         text = text.remove(0, index + 1).simplified();
         //cerr << "after label: '" << text.toStdString() << "'\n";
         
         if (VERBOSE) {
            cerr << "--------------- LABELS -------------------" << endl;
            for(LabelMapIterator i = list->m_labelMap.begin(); i != list->m_labelMap.end(); ++i) {
               cerr << "(" << i.key().toAscii().data();
               cerr << ", " << i.value()->getValue() << ")" << endl;
            }
            cerr << endl;
         }
         
         const QString &id = label->getID();
         if (list->m_labelMap.contains(id))
            PARSE_ERRORL(QString("redeclaration of label '%1'").arg(id), labelStr, labelStr.length());
//         list->m_labelMap.insert(id, label);
         // Label is now inserted only when a valid ParseNode is instantiated (in ParseNode::ParseNode)
         
         if (text.isEmpty()) {
            _tab = orig;
            
            return new ParseNode(list, b, NULL, label);
         }
      } else { // possibly colon w/out identifier
         // may also be a directive ( .byte 0:100  or  .ascii "colon:" )
         if (!text.contains('.'))
            PARSE_ERRORL("missing label before ':'", text, 1);
      }
   }

   // -------------------
   // Check for directive
   // -------------------
   if (text.contains('.')) {
      ParseNode *p = NULL;
 
      try {
         p = parseDirective(b, text, label, list);
      } catch(ParseError &e) {
         _tab = orig;
         throw;
      }
      
      _tab = orig;
      return p;
   }
   
   int equalsIndex = text.indexOf('=');
   if (equalsIndex == 0)
      PARSE_ERRORL("const identifier must appear before '=' symbol", text, 1);
   
   // ------------------
   // Check for Constant
   // ------------------
   if (equalsIndex > 0) {
      ParseNode *p = NULL;
      
      try {
         p = parseConstant(b, text, label, equalsIndex, list);
      } catch(ParseError &e) {
         _tab = orig;
         throw;
      }

      return p;
   }
   
   if (VERBOSE) cerr << _tab << "Parsing instr: " << text.toStdString() << endl;

   index = text.indexOf(' ');
   QString instr = text.section(' ', 0, 0);
   bool noArgs;
   if ((noArgs = (index < 0 || instr.isEmpty())))
      instr = text; // account for instructions w/out args such as 'syscall'
   // remove instr from text
   else text = text.remove(0, index + 1);
   
   if (VERBOSE) cerr << _tab << "Found instr: " << instr.toStdString() << "  '" << text.toStdString() << "'  is left" << endl;

   InstructionMapIterator i = instructionMap.find(instr);

   if (i == instructionMap.end()) {
      if (VERBOSE) cerr << "instr " << instr.toStdString() << " not found in map\n";
      PARSE_ERRORL(QString("'") + instr + QString("' is not a valid instruction."), instr, instr.length());
   }

   StatementArgList *argList;
   StatementArg *args[3] = { NULL, NULL, NULL };
   
   // ------------------------------------------
   // Attempt to Parse Up To Three StatementArgs
   // ------------------------------------------
   if (!noArgs) {
      int i = 0;
      
      do {
         QString arg = text.section(',', i, i);
         if (arg.isEmpty())
            break;
         
         try {
            args[i] = parseArg(arg, list);
         } catch(ParseError &e) {
            e.setUnrecognized(text);
            e += QString(" (in %1 argument to instruction '%2')").arg(
                  (i == 0 ? "first" : (i == 1 ? "second" : "third")), 
                  instr);
            throw;
         }
//         if ((args[i] = parseArg(arg)) == NULL) {
//            _tab = orig;
//            return NULL; // invalid argument
//         }
      } while(++i < 3);

      int noCommas = text.count(',');
      if (noCommas >= i) {
         if (i >= 3)
            PARSE_ERROR(QString("Too many arguments to instruction '%1'").arg(instr), "");
         
         PARSE_ERROR(QString("Stray comma in arguments to instruction '%1'").arg(instr), "");
      }
   }
   
   argList = new StatementArgList(args[0], args[1], args[2]);
   if (VERBOSE) cerr << _tab << "NoArgs: " << argList->noArgs() << endl;
   Instruction *instrFactory = *i;
   
   if (instrFactory == NULL) {
      if (VERBOSE)
         cerr << _tab << ">>> Error: instruction '" << instr.toStdString() << "' has not been implemented yet! <<<\n";
      PARSE_ERRORL(QString("instruction '%1' has not been implemented yet!").arg(instr), text, instr.length());
   }
   
   // --------------------------------------------------------------
   // Ensure instruction and arguments found are Syntactically Valid
   // --------------------------------------------------------------
   if (!instrFactory->isSyntacticallyValid(argList)) {
      if (VERBOSE)
         cerr << _tab << "Error: arguments to instruction '" << instrFactory->getName() << "' are invalid!\n";
      PARSE_ERRORL(QString("invalid arguments to instruction '%1'.\n"
                  "Syntax: '%2'").arg(instr, instrFactory->getName() + QString(" ") + instrFactory->getSyntax()), text, text.length());
   }
   
   Instruction *instruction = instrFactory->create(argList);
   assert (instruction != NULL);
   
   // Setup ParseNode to return and associate w/ QTextBlock *b
   // incorporate AddressIdentifier *label parsed above into ParseNode
   _tab = orig;
   return new ParseNode(list, b, instruction, label);
}

// Parses given text for one argument of an instruction
StatementArg *Parser::parseArg(QString text, ParseList *list) {
   string orig = _tab;
   _tab += "   ";
   
   QString copy(text);

   if (!Parser::simplify(text))
      PARSE_ERRORL(QString("missing argument?  '%1'").arg(copy), copy, copy.length());
   
   int regIndex = text.indexOf('$');
   int len = text.length();
   
   if (regIndex < 0) {
      // check if we have an identifier (const, immediate, label, etc)
      // because no $registers appear in text, must be an identifier to be valid
      Identifier *id = parseIdentifier(text, list);
      
      if (id == NULL) {
         id = parseLabelWithOffset(text, list);
         if (id == NULL)
            PARSE_ERRORL(QString("unrecognized argument"), copy, copy.length());
      }
      
      _tab = orig;
      return new StatementArg(id);
   }
   
   if (regIndex + 1 >= len)
      PARSE_ERRORL(QString("misplaced '$' token."), copy, copy.length());
   
   QString reg;
   QRegExp endOfReg("\\$[a-z0-9]+\\b");
   int endIndex = text.indexOf(endOfReg, regIndex);
   int registerNo = -1;
   
   if (text == QString("$zero"))
      registerNo = 0;
   else {
      if (endIndex + endOfReg.matchedLength() > regIndex + 3)
         PARSE_ERRORL(QString("invalid register '%1'.").arg(text), copy, copy.length());
   
      if (regIndex + 2 >= len || (endIndex > 0 && endIndex + endOfReg.matchedLength() == regIndex + 2 && endIndex == regIndex))
         reg = Parser::substring(text, regIndex + 0, 2);
      else reg = Parser::substring(text, regIndex + 0, 3);

      if (VERBOSE) cerr << _tab << "Len: " << len << ", regIndex = " << regIndex << ", endIndex = " << endIndex << ", matched = " << endOfReg.matchedLength() << endl;
      if (VERBOSE) cerr << _tab << "Parsing register: '" << text.toStdString() << "'  reg: " << reg.toStdString() << endl;

      // determine which $r register
      if (reg.at(1).isDigit()) {// == QChar('r') && reg.at(2) != QChar('a')) {
         reg = Parser::substring(reg, 1, reg.length() - 1);
         //2, reg.length() - 2);  (old, for $r3 type syntax -- which doesn't exist)

         bool okay = false;
         registerNo = reg.toInt(&okay, 10);
         if (!okay || registerNo < zero || registerNo >= pc)
            PARSE_ERRORL(QString("invalid register: '%1'").arg(reg), reg, reg.length());

      } else if (reg == "$fp") { // special alias - not in the registerAliases table
         registerNo = 30;
      } else { // attempt to determine between aliases
         bool found = false;
         
         reg = reg.right(reg.length() - 1);
         for(int i = 0; i < pc ; i++) {
//            QRegExp r(QString("\\$") + QString(registerAliases[i]));
            
            if (reg == QString(registerAliases[i])) {
//            if (r.exactMatch(reg)) {
               found = true;
               registerNo = i;
               break;
            }
         }

         if (!found)
            PARSE_ERRORL(QString("invalid register: '%1'").arg(reg), reg, reg.length());
      }
   }

   // at this point, should know register number
   if (VERBOSE) cerr << _tab << "Register: " << registerNo << " \t $" << registerAliases[registerNo] << "\n";
   
   // --------------------
   // Test for dereference
   // --------------------
   int parenStart = text.indexOf('(');
   bool validParens = (parenStart >= 0);
   if (validParens) {
      int parenEnd = text.indexOf(')');
      
      //cerr << parenStart << ", " << regIndex << ", " << parenEnd << endl;
      if (parenStart > regIndex || parenEnd <= parenStart + 2 || parenEnd < regIndex)
         PARSE_ERROR(QString("parenthesis mismatch."), text);
      
      validParens = true;
      
      
      // TODO:  check to ensure characters inbetween (  $a0  )  parenthesis and $reg are all whitespace or empty
      
      
      if (parenEnd != len - 1) {
         if (VERBOSE)
            cerr << _tab << "text after right paren!\n";
         PARSE_ERROR(QString("invalid characters following right parenthesis"), text);
      }
      
      text = Parser::substring(text, 0, parenStart);
      
      // most general case: label +- imm($r0)
      // check if text contains label +- imm
      Identifier *id = parseLabelWithOffset(QString(text), list);
      if (id != NULL || (id = parseIdentifier(text, list)) != NULL) {
         _tab = orig;
        return new StatementArg(id, registerNo, true);
      }
      
      _tab = orig;
      return new StatementArg(registerNo, true);
   } else { // must have a generic register w/out addressing
      _tab = orig;
      return new StatementArg(registerNo);
   }
}

// Searches for label+-imm($reg)
AddressIdentifier *Parser::parseLabelWithOffset(QString text, ParseList *list) {
   string orig = _tab;
   _tab += "   ";

   int indexP = text.indexOf('+'), index, len = text.length();
   int indexM = text.indexOf('-');
   bool plus = (indexP > 0), minus = (indexM > 0);
   
   if (plus == minus) {
      _tab = orig;
      return NULL; // want one and only one
   }
   
   index = (plus ? indexP : indexM);
   //cerr << index << ", " << text.length() << endl;
   
   QString orig2 = text;
   QString labelStr = text.remove(index, len - index);
   QString immediate = orig2.remove(0, index + 1);

   if (VERBOSE) cerr << _tab << "Parsing Labelw/Offset: label = '" << labelStr.toStdString() << "' \timm = '" << immediate.toStdString() << "'\n";
   
   ImmediateIdentifier *imm = Parser::parseImmediate(QString(immediate), list);
   if (imm == NULL) {
      QString copy = labelStr;
      labelStr = immediate;
      immediate = copy;
      
      // test if + is other way around  (label + imm  instead of imm + label)
      if ((imm = Parser::parseImmediate(immediate, list)) == NULL) { _tab = orig;
         return NULL;
      }
   }
   
   AddressIdentifier *label = Parser::parseLabel(labelStr);

   if (label != NULL)
      label->setOffset((plus ? 1 : -1) * imm->getValue());
   
   _tab = orig;
   return label;
}

// Parses the given text for an immediate number  or label identifier
Identifier *Parser::parseIdentifier(QString text, ParseList *list) {
   string orig = _tab;
   _tab += "   ";

   ImmediateIdentifier *imm = parseImmediate(text, list);
   if (imm != NULL) {
      _tab = orig;
      return imm;
   }

   // We have an identifier
   AddressIdentifier *label = parseLabel(text);

   _tab = orig;
   // returns NULL if parseLabel failed
   return label;
}

// Parses a valid label names (or identifiers for global data, consts, etc)
AddressIdentifier *Parser::parseLabel(QString text) {
   string orig = _tab;
   _tab += "   ";

   if (!Parser::simplify(text)) {
      _tab = orig;
      return NULL;
   }
   
   QRegExp invalidChars("[^0-9a-zA-Z_]+");
   if (text.indexOf(invalidChars) >= 0) {
      _tab = orig;
      return NULL; // invalid char found
   }
   
   QRegExp digit("\\d"); // matches any digit
   if (text.indexOf(digit) == 0) { // identifier begins w/ digit
      _tab = orig;
      return NULL;
   }
   
   if (VERBOSE) cerr << _tab << "Found Label: " << text.toStdString() << endl;

   _tab = orig;
   // don't yet know corresponding ParseNode*
   return new AddressIdentifier(text, NULL);
}

bool Parser::simplify(QString &text) {
   return !(text = text.simplified()).isEmpty();
}

// Parses the given text for an immediate number (in hex, binary, or decimal)
ImmediateIdentifier *Parser::parseImmediate(QString text, ParseList *list) {
   string orig = _tab;
   _tab += "   ";

   if (!Parser::simplify(text)) {
      _tab = orig;
      return NULL;
   }
   
//   cerr << "<< size: " << list->m_preProcessorMap.size() << endl;
   
   bool pasted;
   QString copy = text;
   if ((pasted = list->m_preProcessorMap.contains(text)))
      text = list->m_preProcessorMap[text];
   
   if (text.contains('\'')) {
      // Parse ascii characters  'a' for example
      int index = text.indexOf('\'');
      int index2 = text.indexOf('\'', index + 1);
      
      //cerr << "attempting to parse ASCII: " << text.toStdString() << endl;
      
      if (index != 0 || index2 != text.length() - 1 || index2 < 0) {
         _tab = orig;
         return NULL;
      }
      
      QString origText(text);
      // remove two single-quotes
      text = text.left(text.length() - 1);
      text = text.right(text.length() - 1);
      int value = -1;
      
      switch(text.length()) {
         case 1:
            value = text.at(0).toAscii();
            break;
         case 2:
            if (text.at(0) != '\\') {
               _tab = orig;
               return NULL;
            }
            
            switch(text.at(1).toAscii()) {
               case 'n':
                  value = '\n';
                  break;
               case '\\':
                  value = '\\';
                  break;
               case '"':
                  value = '"';
                  break;
               case 't':
                  value = '\t';
                  break;
               default:
                  _tab = orig;
                  return NULL;
                  break;
            }
            break;
         default:
            _tab = orig;
            return NULL;
      }

      if (value < 0) {
         _tab = orig;
         return NULL;
      }

      if (VERBOSE) {
         cerr << _tab << "Found Immediate: " << value;
         if (pasted)
            cerr << "  (from const '" << copy.toStdString() << "')";

         cerr << endl;
      }
           
      _tab = orig;
      return new ImmediateIdentifier(origText, value);

//      cerr << "VALUE:  " << value << endl;
//      cerr << text.toStdString() << endl;
   }
   
   bool okay = false;
   qint64 value_long = text.toLongLong(&okay, 0); // includes C-like hex and binary conversion :)
   int value;
   
   if (value_long < -2147483648LL || value_long > 4294967295LL) return NULL; // out of 32-bit range
      
   // check that it parsed a number, and that this number fits in a 32 bit integer
   if (okay) {
      if (value_long < 0) // negative
         value = (int) value_long;
      else value = (int) (unsigned int) value_long;
      
      if (VERBOSE) {
         cerr << _tab << "Found Immediate: " << value;
         if (pasted)
            cerr << "  (from const '" << copy.toStdString() << "')";

         cerr << endl;
      }
           
      _tab = orig;
      return new ImmediateIdentifier(text, value);
   } else if (VERBOSE) cerr << _tab << "Invalid immediate: '" << text.toStdString() << "'\n";
   
   _tab = orig;
   return NULL;
}

/*static QString Parser::substring(QString s, int startIndex, int endIndex) {
   return Parser::substring(s, startIndex, endIndex - startIndex);
}*/

QString Parser::substring(QString s, int start, int length) {
   int max = s.length();
   if (length == -1)
      length = max - start;
   
//   cerr << _tab << "" << s.toStdString() << ", ";
//   cerr << _tab << "start: " << start << ", len " << length << ", max " << max << endl;

   if (length < s.length()) {
      int end = length;
      int stripLeft = start - 0, stripRight = max - end;
      
      stripRight -= stripLeft;
      s = s.remove(0, stripLeft);
//      cerr << "afterL: " << s.toStdString() << ", stripR = " << stripRight <<  endl;
      s = s.remove(end, stripRight);
   }
   
   //cerr << _tab << "Return: " << s.toStdString() << endl;
   return s;
}

// Strips a given string of comments and extraneous whitespace
void Parser::trimCommentsAndWhitespace(QString &text) {
   text = text.simplified(); // removed leading/trailing whitespace
   if (text.isEmpty())
      return;

   // remove commented parts
   // we want the first hash(#) after the LAST quote(")
   int lastQuote = text.lastIndexOf('"'), firstQ = -1;
   if (lastQuote == -1) lastQuote = 0; // not found
   else firstQ = text.lastIndexOf('"');
   
   int commentIndex = text.indexOf('#', lastQuote);
   int comment2Index = text.indexOf('#');
   if (comment2Index < firstQ || firstQ == -1)
      commentIndex = comment2Index;
   
   bool comment;//, breakPoint = ((b->userState() & B_BREAKPOINT) != 0);
   
   if ((comment = (commentIndex >= 0))) {
      text.truncate(commentIndex);
      text = text.simplified();
   }
}


// -----------------
// Directive Parsing
// -----------------


enum DirectiveTypes {
   GLOBAL, 
   ALIGN, DATA, TEXT, KTEXT, SPACE, 
   ASCIIZ, ASCII, 
   BYTE, HALF, WORD, 
};

// Listed by argument-types
const char *const directives[] = {
   "global", // symbol
   "align", "data", "text", "ktext", "space", // immediate value
   "asciiz", "ascii", // string
   "byte", "half", "word", // value OR value:size OR value1, value2, ...
   NULL
};


// Parses the directive found in the text given
// (assuming its (possibly optional) label has already been parsed)
ParseNode *Parser::parseDirective(QTextBlock *b, QString &text, AddressIdentifier *label, ParseList *list) {
   string orig = _tab;
   _tab += "   ";
   
   if (VERBOSE) cerr << _tab << "Parsing directive: '" << text.toStdString() << "'\n";
   if (text[0] != QChar('.'))
      PARSE_ERRORL(QString("error parsing directive"), ".", 1);
   
   text = text.right(text.length() - 1);
   
   int type = 0;
   do {
      if (text.startsWith(directives[type]))
         break;
   } while(directives[++type] != NULL);
   
   if (directives[type] == NULL) {
      QString dir = text.section(' ', 0, 0);
      PARSE_ERRORL(QString("invalid directive: '%1'").arg(dir), dir, dir.length());
   }
   
   // shorten text to remove the directive identifier
   text = text.remove(0, QString(directives[type]).length()).simplified();
   
//   cerr << "before: " << text.toStdString() << ", " << type << endl;
   Directive *directive = NULL;
   
   switch(type) {
      case GLOBAL:  // .global symbol
         {
            AddressIdentifier *symbol = parseLabel(text);

            if (symbol != NULL) {
               if (VERBOSE) cerr << _tab << "Found global symbol '" << symbol->getID().toStdString() << "'\n";

               directive = new GlobalDirective(symbol);
            }
         }
         break;
      case ALIGN:   // .align imm
      case DATA:    // .data <address>
      case TEXT:
      case KTEXT:
      case SPACE:   // .space imm
         {
            int offset = -1;
            
            if (text != QString("")) {
               ImmediateIdentifier *imm = parseImmediate(text, list);
               
               if (imm == NULL)
                  PARSE_ERRORL(QString("argument to directive '%1' is invalid: '%2'").arg(QString(directives[type]), text), text, text.length());

               offset = imm->getValue();
            }
            
            if (VERBOSE) {
               cerr << _tab << "Found directive '" << directives[type];
               if (offset >= 0)
                  cerr << "' with offset/value of '" << offset;

               cerr << "'\n";
            }
            
            if (type == DATA)
               directive = new DataDirective(offset);
            else if (type == TEXT)
               directive = new TextDirective(offset);
            else if (type == KTEXT)
               directive = new KTextDirective(offset);
            else if (offset >= 0) {
               if (type == ALIGN)
                  directive = new AlignDirective(offset);
               else if (type == SPACE)
                  directive = new SpaceDirective(offset);
            } else // invalid
               PARSE_ERRORL(QString("argument to directive '%1' is invalid: '%2'").arg(QString(directives[type]), text), text, text.length());
         }
         break;
      case ASCII:  // Parse strings
      case ASCIIZ:
         {
            int strIndex = text.indexOf('"');
            if (strIndex != 0)  // str must start w/ a quotation mark
               PARSE_ERRORL(QString("directive '%1' takes a string").arg(QString(directives[type])), text, text.length());
            
            // TODO:  possibly remove simplified() -- cause it may remove white space from strings?  No -- don't think so
            // NOTE(tim): we used to call .trimmed() on text, but that made the string " " become "", which we don't want.
            //            Therefore, removing trimmed().
            text = text.remove(0, 1);
           
            int i = 0, lastFakeQuote = -1;
            while(i < text.length() - 1) {
               int j = i;
               
               if (text.indexOf("\\\\", i) == i) {
                  text = text.left(i) + QChar('\\') + text.right(text.length() - 2 - i);
                  ++i;
               }
               if (text.indexOf("\\n", i) == i) {
                  text = text.left(i) + QChar('\n') + text.right(text.length() - 2 - i);
                  ++i;
               }
               if (text.indexOf("\\t", i) == i) {
                  text = text.left(i) + QChar('\t') + text.right(text.length() - 2 - i);
                  ++i;
               }
               if (text.indexOf("\\\"", i) == i) {
                  text = text.left(i) + QChar('"') + text.right(text.length() - 2 - i);
                  lastFakeQuote = ++i;
               }
               
               if (i <= j) // no replacements occurred
                  ++i;
            }
            
            if ((strIndex = text.indexOf('"', lastFakeQuote)) < 0) // no matching ending quotation
               PARSE_ERRORL(QString("missing or mismatched '\"' symbol at end of string"), text, text.length());

//            QString text = text.left(strIndex).replace(QString("\\n"), QString('\n'));
//            text = text.replace(QString("\\\""), QString('"'));
//            text = text.replace(QString("\\t"), QString('\t'));
            
            // TODO:  correctly handling backslashes in strings?
            
//            cerr << text.length() << " vs " << strIndex << " (last: " << lastFakeQuote << endl;

            if (strIndex < text.length() - 1) // garbage after end of string?
               PARSE_ERRORL(QString("unrecognized character(s) following string"), text, text.length());
            
            text = text.left(strIndex);
            if (VERBOSE) cerr << _tab << "Found String: '" << text.toStdString() << "'\n";
            
            if (type == ASCII)
               directive = new AsciiDirective(text);
            else directive = new AsciizDirective(text); // type == ASCIIZ
         }
         break;
         
      case BYTE: // ------------------------------
      case HALF: // Hardest cases; several formats
      case WORD: // ------------------------------
         {
	    std::auto_ptr<IntegerDirectiveInitializer> init;
            int sizeInd  = text.indexOf(':'); // for value:size
            int commaInd = text.indexOf(','); // for value1, value2, ...

            if (sizeInd < 0 || commaInd < 0) {
               if (sizeInd < 0) {
                  if (commaInd < 0) {  // value
                     Identifier* value;
                     if ((value = parseIdentifier(text, list)) == NULL) {
                        PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                    "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                    directives[type]), text, text.length());
                     }
                     
                     if (VERBOSE) cerr << _tab << "Reserving 1 '" << directives[type] << "' with given values\n";
                     
                     init.reset(new SingleIntegerDirectiveInitializer(value));
                  } else {    // value1, value2, value3, ...
                     int noValues = 1;
                     int j = 0;
                     while((j = text.indexOf(',', j)) >= 0) { ++j; ++noValues; }
                     if (VERBOSE) cerr << _tab << noValues << " values found:\n";
                     vector<Identifier*> values;
                     int i = 0;
                     
                     // Parse a list of arguments
                     for(;;) {
                        QString val = text.section(',', i, i).simplified();
                        if (val.isEmpty())
                           break;
                        
                        Identifier* value;
                        if ((value = parseIdentifier(val, list)) == NULL) {
                           PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                       "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                       directives[type]), val, val.length());
                        }
                        
                        values.push_back(value);
                        ++i;
                     }
                     
                     if (values.size() <= 0) {
                        PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                    "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                    directives[type]), text, text.length());
                     }
                     
                     if (VERBOSE) cerr << _tab << "Reserving " << values.size() << " '" << directives[type] << "'s with given values\n";

                     init.reset(new SingleIntegerDirectiveInitializer(values));
                  }
               } else {       // value:size
                  QString valueStr = text.left(sizeInd).simplified();
                  Identifier* value;
                  if ((value = parseIdentifier(valueStr, list)) == NULL) {
                     PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                 "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                 directives[type]), valueStr, valueStr.length());
                  }
                  QString sizeStr = text.right(text.length() - sizeInd - 1).simplified();

                  // ensure size is valid; allow zero-length arrays like .data x:0
                  int size = 0;
                  ImmediateIdentifier* size_imm;
                  if ((size_imm = parseImmediate(sizeStr, list)) == NULL || ((size = size_imm->getValue()) < 0)) {
                     PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                 "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]),
                                 directives[type]), sizeStr, sizeStr.length());
                  }
                  
                  if (VERBOSE) cerr << _tab << "Reserving " << size << " '" << directives[type] << "'s with given value\n";

		  init.reset(new RepeatIntegerDirectiveInitializer(value, size));
               }
            }
            if (type == BYTE)
               directive = new ByteDirective(init);
            else if (type == HALF)
               directive = new HalfDirective(init);
            else if (type == WORD)
               directive = new WordDirective(init);
         }
         
         break;
   }
   
   if (directive == NULL) // invalid directive
      PARSE_ERRORL(QString("error parsing directive '%1'").arg(QString(directives[type])), text, text.length());
   
   // .byte, .half, .word 
   // followed by:
   //    value
   // OR
   //    value:size
   // OR
   //    value1, value2, value3, ... valueN  consecutive in memory
   // 
   // Ex. 'table:       .word    0:TABLE_SIZE'
   
   _tab = orig;

   // Huzzah!  Hopefully we got here and it worked?!
   return new ParseNode(list, b, directive, label);
}

ParseNode *Parser::parseConstant(QTextBlock *b, QString &text, AddressIdentifier *label, int equalsIndex, ParseList *list) {
    string orig = _tab;
   _tab += "   ";
   
   if (VERBOSE) cerr << _tab << "Parsing constant: '" << text.toStdString() << "'\n";
   
   QString labelStr = substring(text, 0, equalsIndex).simplified();
   
   AddressIdentifier *name = parseLabel(labelStr);
   if (name == NULL)
      PARSE_ERRORL(QString("invalid identifier for constant: '%1'").arg(labelStr), labelStr, labelStr.length());
   
   QString valueStr = text.remove(0, equalsIndex + 1).simplified();
   ImmediateIdentifier *imm = parseImmediate(valueStr, list);
   if (imm == NULL)
      PARSE_ERRORL(QString("invalid value assigned to '%1'").arg(labelStr), valueStr, valueStr.length());
   
   const QString &id = name->getID();
   if (VERBOSE) cerr << _tab << "Const found: " << id.toStdString() << " = " << imm->getValue() << endl;
   if (list->m_preProcessorMap.contains(id))
      PARSE_ERRORL(QString("multiple declarations of const '%1'").arg(name->getID()), labelStr, labelStr.length());
   
   // from here on out, string name will get replaced by string value during parsing :)
   list->m_preProcessorMap.insert(id, imm->getID());
   
   _tab = orig;
   return new ParseNode(list, b, new ConstStatement(name, imm), label);
}

ParseError::ParseError(const QString &description, const QString &unrecognized, int length, QTextBlock *b, int lineNo)
   : QString(description), m_unrecognized(unrecognized), m_textBlock(b), m_position(-1), m_length(length), m_lineNo(lineNo)
{
   if (b != NULL)
      setTextBlock(b);
}

QString ParseError::getUnrecognized() const {
   return m_unrecognized;
}

void ParseError::setUnrecognized(const QString &text) {
//   cerr << "\n setUnrecognized: " << text.toStdString() << ", " << m_unrecognized.toStdString() << ", " << m_position << ", " << text.indexOf(m_unrecognized) << endl;
   if (m_position != text.indexOf(m_unrecognized) || m_position != text.lastIndexOf(m_unrecognized)) {
      m_unrecognized = text;
      m_length = text.length();

      setTextBlock(m_textBlock);
   }
}

int ParseError::getPosition() const {
   return m_position;
}

int ParseError::getLength() const {
   return m_length;
}

int ParseError::getLineNo() const {
   return m_lineNo;
}

QTextBlock *ParseError::getTextBlock() const {
   return m_textBlock;
}

void ParseError::setPosition(int position) {
   m_position = position;
}

void ParseError::setLineNo(int lineNo) {
   m_lineNo = lineNo;
}

void ParseError::setTextBlock(QTextBlock *b) {
   m_textBlock = b;
   
   // look for string in text block and set position, length accordingly
   if (b != NULL) {
      const QString &text = b->text();
      m_position = text.indexOf(m_unrecognized);
      
      //if (m_position >= 0)
      //   m_position += b->position();
   }
}

// --------------------
// Directive Superclass
// --------------------
Directive::Directive(unsigned int requiredSizeInBytes) 
   : Statement(requiredSizeInBytes)
{ }


DirectiveMap directiveMap;
void Directive::InitializeDirectiveMap() {
   directiveMap.insert("global", new GlobalDirective());
   directiveMap.insert("align", new AlignDirective());
   directiveMap.insert("data", new DataDirective());
   directiveMap.insert("text", new TextDirective());
   directiveMap.insert("ktext", new KTextDirective());
   directiveMap.insert("space", new SpaceDirective());
   directiveMap.insert("asciiz", new AsciizDirective());
   directiveMap.insert("ascii", new AsciiDirective());
   directiveMap.insert("byte", new ByteDirective());
   directiveMap.insert("half", new HalfDirective());
   directiveMap.insert("word", new WordDirective());
/*   while(directives[i] != NULL)
      directiveMap.insert(directives[i++], NULL);*/
}

// Default is no initialization (setting to NULL to avoid warnings)
void Directive::initialize(ParseNode *p, State *s) { p = NULL; s = NULL; }

unsigned int Directive::getPreferredAlignment() const {
   return 0; // turn off automatic alignment
}

// specific subclasses override accordingly
bool Directive::isAlign() const {
   return false;
}

bool Directive::isData() const {
   return false;
}

bool Directive::isText() const {
   return false;
}

ParseError::~ParseError() { }

