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

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QRegExp>
#include <QtCore>
#include <string>

string _tab;

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
         cerr << "Error line  " << lineNo << ": " << e.toStdString() << endl;
         e.setTextBlock(actual);
         e.setLineNo(lineNo);
         errors.push_back(e);
         
         continue;
      }
      
      assert (n != NULL);
      // Setup n in dis hizhouse
      parseNodes.push_back(n);
   }
   
   if (errors.size() >= 1) {
      delete parseList;
      throw errors;  // (vector<ParseError>)
   }
   
   // do this afterwards so labels defined later above are already in parseList's maps
   foreach(ParseNode *cur, parseNodes)
      parseList->insert(cur);
   
   return parseList;
}

// Returns NULL if b does not contain a syntactically-valid line
ParseNode *Parser::parseLine(QTextBlock *b, ParseList *list) {
   QString text = b->text().simplified(); // removed leading/trailing whitespace
   
   cerr << "Parsing line: " << text.toStdString() << endl;
   string orig = _tab;
   _tab += "   ";
   
   // remove commented parts
   // we want the first hash(#) after the LAST quote(")
   int lastQuote = text.lastIndexOf('"'), firstQ = -1;
   if (lastQuote == -1) lastQuote = 0; // not found
   else firstQ = text.lastIndexOf('"');
   
   int commentIndex = text.indexOf('#', lastQuote), index;
   int comment2Index = text.indexOf('#');
   if (comment2Index < firstQ || firstQ == -1)
      commentIndex = comment2Index;
   
   bool comment;//, breakPoint = ((b->userState() & B_BREAKPOINT) != 0);
   
   if ((comment = (commentIndex >= 0))) {
      text.truncate(commentIndex);
      text = text.simplified();
      
      cerr << _tab << "Removed comment: '" << text.toStdString() << "' is left\n";
   }
   
   if (text == "") {
      cerr << _tab << "Recognized as blank or comment\n";
      _tab = orig;
      return new ParseNode(b);
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
         
         const QString &id = label->getID();
         if (list->m_labelMap.contains(id))
            PARSE_ERRORL(QString("redeclaration of label '%1'").arg(id), labelStr, labelStr.length());
         list->m_labelMap.insert(id, label);
         
         if (text == "") {
            _tab = orig;
            
            return new ParseNode(b, NULL, label);
         }
      } else { // possibly colon w/out identifier
         // may also be a directive (.byte 0:100  or .ascii "colon:"
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
   
   cerr << _tab << "Parsing instr: " << text.toStdString() << endl;

   index = text.indexOf(' ');
   QString instr = text.section(' ', 0, 0);
   bool noArgs;
   if ((noArgs = (index < 0 || instr == "")))
      instr = text; // account for instructions w/out args such as 'syscall'
   // remove instr from text
   else text = text.remove(0, index + 1);
   
   cerr << _tab << "Found instr: " << instr.toStdString() << "  '" << text.toStdString() << "'  is left" << endl;

   InstructionMapIterator i = instructionMap.find(instr);

   if (i == instructionMap.end()) {
      cerr << "instr " << instr.toStdString() << " not found in map\n";
      PARSE_ERRORL(QString("'") + instr + QString("' is not a valid instruction."), text, instr.length());
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
         if (arg == "")
            break;
         
         try {
            args[i] = parseArg(arg, list);
         } catch(ParseError &e) {
            e += QString("(in argument '%1' to instruction '%2')").arg(QString(i), instr);
            throw;
         }
//         if ((args[i] = parseArg(arg)) == NULL) {
//            _tab = orig;
//            return NULL; // invalid argument
//         }
      } while(++i < 3);
   }
   
   argList = new StatementArgList(args[0], args[1], args[2]);
   cerr << _tab << "NoArgs: " << argList->noArgs() << endl;
   Instruction *instrFactory = *i;
   
   if (instrFactory == NULL) {
      cerr << _tab << ">>> Error: instruction '" << instr.toStdString() << "' has not been implemented yet! <<<\n";
      PARSE_ERRORL(QString("instruction '%1' has not been implemented yet!").arg(instr), text, instr.length());
   }
   
   // --------------------------------------------------------------
   // Ensure instruction and arguments found are Syntactically Valid
   // --------------------------------------------------------------
   if (!instrFactory->isSyntacticallyValid(argList)) {
      cerr << _tab << "Error: arguments to instruction '" << instrFactory->getName() << "' are invalid!\n";
      PARSE_ERRORL(QString("invalid arguments to instruction '%1'.\n"
                  "Syntax: '%2'").arg(instr, instrFactory->getName() + QString(" ") + instrFactory->getSyntax()), text, text.length());
   }
   
   Instruction *instruction = instrFactory->create(argList);
   assert (instruction != NULL);
   
   // Setup ParseNode to return and associate w/ QTextBlock *b
   // incorporate AddressIdentifier *label parsed above into ParseNode
   _tab = orig;
   return new ParseNode(b, instruction, label);
}

// Parses given text for one argument of an instruction
StatementArg *Parser::parseArg(QString text, ParseList *list) {
   string orig = _tab;
   _tab += "   ";

   QString copy = text;
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
            PARSE_ERRORL(QString("unrecognized argument: '%1'").arg(copy), copy, copy.length());
      }
      
      _tab = orig;
      return new StatementArg(id);
   }
   
   if (regIndex + 1 >= len)
      PARSE_ERRORL(QString("misplaced '$' token."), copy, copy.length());
   
   QString reg;
   QRegExp endOfReg("\\$[a-z0-9]+\\b");
   int endIndex = text.indexOf(endOfReg, regIndex);
   
   if (regIndex + 2 >= len || (endIndex > 0 && endIndex + endOfReg.matchedLength() == regIndex + 2 && endIndex == regIndex))
      reg = Parser::substring(text, regIndex + 0, 2);
   else reg = Parser::substring(text, regIndex + 0, 3);

   cerr << _tab << "Len: " << len << ", regIndex = " << regIndex << ", endIndex = " << endIndex << ", matched = " << endOfReg.matchedLength() << endl;
   cerr << _tab << "Parsing register: '" << text.toStdString() << "'  reg: " << reg.toStdString() << endl;
   
   int registerNo = -1;
   if (reg.at(1) == QChar('r') && reg.at(2) != QChar('a')) { // determine which $r register
      reg = Parser::substring(text, 2, reg.length() - 2);
      
      bool okay = false;
      registerNo = reg.toInt(&okay, 10);
      if (!okay || registerNo < zero || registerNo >= register_count)
         PARSE_ERRORL(QString("invalid register: '%1'").arg(reg), reg, reg.length());

   } else { // determine between aliases
      bool found = false;

      // Don't count last 3 aliases (PC, HI, LO) -- unavailable to user
      for(int i = 0; i < register_count - 3; i++) {
         QRegExp r(QString("\\$") + QString(registerAliases[i]));

         if (r.exactMatch(reg)) {
            found = true;
            registerNo = i;
            break;
         }
      }
      
      if (!found)
         PARSE_ERRORL(QString("invalid register: '%1'").arg(reg), reg, reg.length());
   }
   
   // at this point, should know register number
   cerr << _tab << "Register: " << registerNo << " \t $" << registerAliases[registerNo] << "\n";
   
   // --------------------
   // Test for dereference
   // --------------------
   int parenStart = text.indexOf('(');
   bool validParens = (parenStart >= 0);
   if (validParens) {
      int parenEnd = text.indexOf(')');

      if (parenStart > regIndex || parenEnd <= parenStart + 3 || parenEnd < regIndex)
         PARSE_ERROR(QString("parenthesis mismatch."), text);
      
      validParens = true;
      
      
      // TODO:  check to ensure characters inbetween (  $r0  )  parenthesis and $reg are all whitespace or empty
      
      
      if (parenEnd != len - 1) {
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

   cerr << _tab << "Parsing Labelw/Offset: label = '" << labelStr.toStdString() << "' \timm = '" << immediate.toStdString() << "'\n";
   
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
      label->setOffset(imm->getValue());
   
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
   
   cerr << _tab << "Found Label: " << text.toStdString() << endl;

   _tab = orig;
   // don't yet know corresponding ParseNode*
   return new AddressIdentifier(text, NULL);
}

bool Parser::simplify(QString &text) {
   return ((text = text.simplified()) != "");
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
  
   bool okay = false;
   int value = text.toInt(&okay, 0);
   
   // includes hex and binary conversion :)
   if (okay) {// text was parsed as a valid int, according to the c-language!
      cerr << _tab << "Found Immediate: " << value;
      if (pasted)
         cerr << "  (from const '" << copy.toStdString() << "')";
      cerr << endl;

      _tab = orig;
      return new ImmediateIdentifier(text, value);
   } else cerr << _tab << "Invalid immediate: '" << text.toStdString() << "'\n";
   
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
   
   cerr << _tab << "Parsing directive: '" << text.toStdString() << "'\n";
   if (text[0] != QChar('.'))
      PARSE_ERROR(QString("error parsing directive"), text);
   
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
               cerr << _tab << "Found global symbol '" << symbol->getID().toStdString() << "'\n";

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
            
            cerr << _tab << "Found directive '" << directives[type];
            if (offset >= 0)
               cerr << "' with offset/value of '" << offset;
            cerr << "'\n";
            
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

            text = text.remove(0, 1).trimmed();
           
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
            cerr << _tab << "Found String: '" << text.toStdString() << "'\n";
            
            if (type == ASCII)
               directive = new AsciiDirective(text);
            else directive = new AsciizDirective(text); // type == ASCIIZ
         }
         break;
         
      case BYTE: // ------------------------------
      case HALF: // Hardest cases; several formats
      case WORD: // ------------------------------
         {
            int sizeInd  = text.indexOf(':'); // for value:size
            int commaInd = text.indexOf(','); // for value1, value2, ...
            ImmediateIdentifier *imm = NULL;

            if (sizeInd < 0 || commaInd < 0) {
               if (sizeInd < 0) {
                  if (commaInd < 0) {  // value
                     if ((imm = parseImmediate(text, list)) == NULL) {
                        PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                    "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                    directives[type]), text, text.length());
                     }
                     
                     int value = imm->getValue();
                     
                     cerr << _tab << "Reserving " << "1" << " '" << directives[type] << "' with initial value '" << value << "'\n";
                     
                     if (type == BYTE)
                        directive = new ByteDirective(value);
                     else if (type == HALF)
                        directive = new HalfDirective(value);
                     else if (type == WORD)
                        directive = new WordDirective(value);
                  } else {    // value1, value2, value3, ...
                     int noValues = 1;
                     int j = 0;
                     while((j = text.indexOf(',', j)) >= 0) { ++j; ++noValues; }
                     cerr << _tab << noValues << " values found:\n";
                     vector<int> values;
                     int i = 0;
                     
                     // Parse a list of arguments
                     for(;;) {
                        QString val = text.section(',', i, i).simplified();
                        if (val == "")
                           break;
                        
                        if ((imm = parseImmediate(val, list)) == NULL) {
                           PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                       "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                       directives[type]), val, val.length());
                        }
                        
                        values.push_back(imm->getValue());
                        ++i;
                     }
                     
                     if (values.size() <= 0) {
                        PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                    "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                    directives[type]), text, text.length());
                     }
                     
                     cerr << _tab << "Reserving " << values.size() << " '" << directives[type] << "'s with values listed\n";
                     
                     if (type == BYTE)
                        directive = new ByteDirective(values);
                     else if (type == HALF)
                        directive = new HalfDirective(values);
                     else if (type == WORD)
                        directive = new WordDirective(values);
                  }
               } else {       // value:size
                  QString valueStr = text.left(sizeInd).simplified();
                  if ((imm = parseImmediate(valueStr, list)) == NULL) {
                     PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                 "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]), 
                                 directives[type]), valueStr, valueStr.length());
                  }
                  int value = imm->getValue(), size = 0;
                  QString sizeStr = text.right(text.length() - sizeInd - 1).simplified();
                  if ((imm = parseImmediate(sizeStr, list)) == NULL || ((size = imm->getValue()) <= 0)) {
                     PARSE_ERRORL(QString("invalid arguments to directive '%1'\n"
                                 "Syntax: '%2' value OR value:size OR val1, val2, ...").arg(QString(directives[type]),
                                 directives[type]), sizeStr, sizeStr.length());
                  }
                  
                  cerr << _tab << "Reserving " << size << " '" << directives[type] << "'s with value '" << value << "'\n";
                  
                  if (type == BYTE)
                     directive = new ByteDirective(value, size);
                  else if (type == HALF)
                     directive = new HalfDirective(value, size);
                  else if (type == WORD)
                     directive = new WordDirective(value, size);
               }
            }
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
   return new ParseNode(b, directive, label);
}

ParseNode *Parser::parseConstant(QTextBlock *b, QString &text, AddressIdentifier *label, int equalsIndex, ParseList *list) {
    string orig = _tab;
   _tab += "   ";
   
   cerr << _tab << "Parsing constant: '" << text.toStdString() << "'\n";
   
   // TODO!
   QString labelStr = substring(text, 0, equalsIndex).simplified();
   
   AddressIdentifier *name = parseLabel(labelStr);
   if (name == NULL)
      PARSE_ERRORL(QString("invalid identifier for constant: '%1'").arg(labelStr), labelStr, labelStr.length());
   
   QString valueStr = text.remove(0, equalsIndex + 1).simplified();
   ImmediateIdentifier *imm = parseImmediate(valueStr, list);
   if (imm == NULL)
      PARSE_ERRORL(QString("invalid value assigned to '%1'").arg(labelStr), valueStr, valueStr.length());
   
   const QString &id = name->getID();
   cerr << _tab << "Const found: " << id.toStdString() << " = " << imm->getValue() << endl;
   if (list->m_preProcessorMap.contains(id))
      PARSE_ERRORL(QString("multiple declarations of const '%1'").arg(name->getID()), labelStr, labelStr.length());
   
   // from here on out, string name will get replaced by string value during parsing :)
   list->m_preProcessorMap.insert(id, imm->getID());
   
   _tab = orig;
   return new ParseNode(b, new ConstStatement(name, imm), label);
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
      
      if (m_position >= 0)
         m_position += b->position();
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
   int i = 0;
   while(directives[i] != NULL)
      directiveMap.insert(directives[i++], NULL);
}

// Default is no initialization (setting to NULL to avoid warnings)
void Directive::initialize(ParseNode *p, State *s) { p = NULL; s = NULL; }

unsigned int Directive::getPreferredAlignment() const {
   return 0; // turn off automatic alignment
}

bool Directive::isAlign() const {
   return false;
}

