#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QRegExp>
#include <QtCore>

#include "ParseList.H"
#include "ParseNode.H"
#include "StatementArgList.H"
#include "Identifier.H"
#include "Statement.H"
#include "../gui/Utilities.H"
#include <string>

const char *const registerAliases[] = {
   "r0", "at",       // $r0-1
   "v0", "v1",       // $r2-3
   "a0", "a1", "a2", "a3", // $r4-r7
   "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", // $r8-15
   "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", // $r16-23
   "t8", "t9",       // $r24-25
   "k0", "k1", "gp", // $r26-28
   "sp",             // $r29
   "s8",             // $r30
   "ra",             // $r31
   
   // Special -- not available to user
   "PC",             // 32
   "HI",             // 33
   "LO",             // 34
};

ParseList::ParseList(QTextDocument *document) :
   m_source(document)
{
   
}

ParseNode* ParseList::getFirst(void) {
   return NULL;
   //	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}

string _tab;

ParseList *ParseList::parseDocument(QTextDocument *document) {
   //ParseList *p = new ParseList(document);
   int lineNo   = 0;
   
   _tab = "   ";
   for(QTextBlock cur = document->begin(); cur != document->end(); cur = cur.next(), lineNo++) {
      ParseNode *n = ParseList::parseLine(&cur);
      
      if (n == NULL) { // ParseError on current line
         cerr << "ParseError occurred on line: " << lineNo << endl;
      }
      // TODO:  add if valid; return NULL otherwise
   }

   return NULL;
}

// Returns NULL if b does not contain a syntactically-valid line
ParseNode *ParseList::parseLine(QTextBlock *b) {
   QString text = b->text().simplified(); // removed leading/trailing whitespace

   cerr << "Parsing line: " << text.toStdString() << endl;
   string orig = _tab;
   _tab += "   ";

   // remove commented parts
   int commentIndex = text.indexOf('#'), index;
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
      if (index <= 0) {
         _tab = orig;
         return NULL;
      }
      
      QString labelStr = text.section(':', 0, 0);
      if ((label = parseLabel(labelStr)) == NULL) {
         _tab = orig;
         
         return NULL; // colon w/out identifier
      }
      
      // remove label from text and continue parsing
      text = text.remove(0, index + 1).simplified();
      //cerr << "after label: '" << text.toStdString() << "'\n";
      
      if (text == "") {
         _tab = orig;

         return new ParseNode(b, NULL);
      }
   }

   // -------------------
   // Check for directive
   // -------------------
   if (text.contains('.')) {
      ParseNode *p = parseDirective(b, text, label);

      _tab = orig;
      return p;
   }
   
   int equalsIndex = text.indexOf('=');
   if (equalsIndex == 0) {
      _tab = orig;
      return NULL;
   }
   
   // ------------------
   // Check for Constant
   // ------------------
   if (equalsIndex > 0) {
      ParseNode *p = parseConstant(b, text, label, equalsIndex);

      _tab = orig;
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

      _tab = orig;
      return NULL; // instr not found in map
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
         
         if ((args[i] = parseArg(arg)) == NULL) {
            _tab = orig;
            return NULL; // invalid argument
         }
      } while(++i < 3);
   }
   
   argList = new StatementArgList(args[0], args[1], args[2]);
   cerr << _tab << "NoArgs: " << argList->noArgs() << endl;
   Instruction *instrFactory = *i;
   
   if (instrFactory == NULL) {
      cerr << _tab << ">>> Error: instruction '" << instr.toStdString() << "' has not been implemented yet! <<<\n";
      
      _tab = orig;
      return NULL;
   }
   
   // --------------------------------------------------------------
   // Ensure instruction and arguments found are Syntactically Valid
   // --------------------------------------------------------------
   if (!instrFactory->isSyntacticallyValid(argList)) {
      cerr << _tab << "Error: arguments to instruction '" << instrFactory->getName() << "' are invalid!\n";
      
      _tab = orig;
      return NULL;
   }
   
   Instruction *instruction = instrFactory->create(argList);
   assert (instruction != NULL);
   
   // Setup ParseNode to return and associate w/ QTextBlock *b
   // incorporate AddressIdentifier *label parsed above into ParseNode
   _tab = orig;
   return new ParseNode(b, instruction, label);
}

// Parses given text for one argument of an instruction
StatementArg *ParseList::parseArg(QString text) {
   if (!ParseList::simplify(text))
      return NULL;
   
   string orig = _tab;
   _tab += "   ";

   int regIndex = text.indexOf('$');
   int len = text.length();
   
   if (regIndex < 0) {
      // check if we have an identifier (const, immediate, label, etc)
      // because no $registers appear in text, must be an identifier to be valid
      Identifier *id = parseIdentifier(text);
      
      if (id == NULL) {
         id = parseLabelWithOffset(text);
         if (id == NULL) {
            _tab = orig;
            return NULL;
         }
      }
      
      _tab = orig;
      return new StatementArg(id);
   }
   
   if (regIndex + 1 >= len) {
      _tab = orig;
      return NULL;
   }
   
   QString reg;
   QRegExp endOfReg("\\$[a-z0-9]+\\b");
   int endIndex = text.indexOf(endOfReg, regIndex);
   
   if (regIndex + 2 >= len || (endIndex > 0 && endIndex + endOfReg.matchedLength() == regIndex + 2 && endIndex == regIndex))
      reg = ParseList::substring(text, regIndex + 0, 2);
   else reg = ParseList::substring(text, regIndex + 0, 3);

//   cerr << _tab << "Len: " << len << ", regIndex = " << regIndex << ", endIndex = " << endIndex << ", matched = " << endOfReg.matchedLength() << endl;
//   cerr << _tab << "Parsing register: '" << text.toStdString() << "'  reg: " << reg.toStdString() << endl;
   
   int registerNo = -1;
   if (reg[1] == QChar('r')) { // determine which $r register
      reg = ParseList::substring(text, 2, reg.length() - 2);
      
      bool okay = false;
      registerNo = reg.toInt(&okay, 10);
      if (!okay || registerNo < zero || registerNo >= register_count) {
         _tab = orig;
         
         return NULL;
      }
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
      
      if (!found) {
         _tab = orig;
         return NULL;
      }
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

      if (parenStart > regIndex || parenEnd <= parenStart + 3 || parenEnd < regIndex) {
         _tab = orig;
         return NULL; // parenthesis troubles
      }
      
      validParens = true;
      
      
      // TODO:  check to ensure characters inbetween (  $r0  )  parenthesis and $reg are all whitespace or empty
      
      
      if (parenEnd != len - 1) {
         cerr << _tab << "text after right paren!\n";
         _tab = orig;
         return NULL;
      }
      
      text = ParseList::substring(text, 0, parenStart);
      
      // most general case: label +- imm($r0)
      // check if text contains label +- imm
      Identifier *id = parseLabelWithOffset(QString(text));
      if (id != NULL || (id = parseIdentifier(text)) != NULL) {
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
AddressIdentifier *ParseList::parseLabelWithOffset(QString text) {
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
   QString labelStr = text.remove(index, len - index);
   QString immediate = text.remove(0, index + 1);
   
   cerr << _tab << "Parsing Labelw/Offset: label=" << labelStr.toStdString() << " \timm = " << immediate.toStdString() << endl;
   
   ImmediateIdentifier *imm = ParseList::parseImmediate(immediate);
   if (imm == NULL) {
      _tab = orig;
      return NULL;
   }
   
   AddressIdentifier *label = ParseList::parseLabel(labelStr);

   if (label != NULL)
      label->setOffset(imm->getValue());
   
   _tab = orig;
   return label;
}

// Parses the given text for an immediate number  or label identifier
Identifier *ParseList::parseIdentifier(QString text) {
   string orig = _tab;
   _tab += "   ";

   ImmediateIdentifier *imm = parseImmediate(text);
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
AddressIdentifier *ParseList::parseLabel(QString text) {
   string orig = _tab;
   _tab += "   ";

   if (!ParseList::simplify(text)) {
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

bool ParseList::simplify(QString &text) {
   return ((text = text.simplified()) != "");
}

// Parses the given text for an immediate number (in hex, binary, or decimal)
ImmediateIdentifier *ParseList::parseImmediate(QString text) {
   string orig = _tab;
   _tab += "   ";

   if (!ParseList::simplify(text)) {
      _tab = orig;
      return NULL;
   }
   
   cerr << _tab << "Parsing immediate: '" << text.toStdString() << "'\n";
   
   bool okay = false;
   int value = text.toInt(&okay, 0);

   // includes hex and binary conversion :)
   if (okay) {// text was parsed as a valid int, according to the c-language!
      cerr << _tab << "Found Immediate: " << value << endl;

      _tab = orig;
      return new ImmediateIdentifier(text, value);
   }
   
   _tab = orig;
   return NULL;
}

/*static QString ParseList::substring(QString s, int startIndex, int endIndex) {
   return ParseList::substring(s, startIndex, endIndex - startIndex);
}*/

QString ParseList::substring(QString s, int start, int length) {
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

// Parses the directive found in the text given
// (assuming its (possibly optional) label has already been parsed)
ParseNode *ParseList::parseDirective(QTextBlock *b, QString &text, AddressIdentifier *label) {
   string orig = _tab;
   _tab += "   ";
   
   cerr << _tab << "Parsing directive: '" << text.toStdString() << "'\n";
   
   
   // TODO!
   
   // .align imm
   // .global label
   
   // .data, .text, .ktext
   
   // .ascii, .asciiz "string"
   
   
   // need to differentiate btwn const identifier and immediate -- these two are interchangeable, but AddressIdentifier should usually not be used when one of these is required?
   
   
   // .byte, .half, .space, .word 
   // followed by:
   //    value
   // OR
   //    value:size
   // OR
   //    value1, value2, value3, ... valueN  consecutive in memory
   // 
   // Ex. 'table:       .word    0:TABLE_SIZE'
   
   
   _tab = orig;
   return NULL;
}

ParseNode *ParseList::parseConstant(QTextBlock *b, QString &text, AddressIdentifier *label, int equalsIndex) {
    string orig = _tab;
   _tab += "   ";
   
   cerr << _tab << "Parsing constant: '" << text.toStdString() << "'\n";
   
   // TODO!
   QString labelStr = substring(text, 0, equalsIndex).simplified();
   
   AddressIdentifier *name = parseLabel(labelStr);
   if (name == NULL) {
      _tab = orig;
      return NULL; // invalid name for constant
   }
   
   QString valueStr = text.remove(0, equalsIndex + 1).simplified();
   ImmediateIdentifier *imm = parseImmediate(valueStr);
   if (imm == NULL) {
      _tab = orig;
      return NULL; // invalid immediate value for constant
   }
   
   cerr << _tab << "Const found: " << name->getID().toStdString() << " = " << imm->getValue() << endl;
   
   
   // TODO:  return new ParseNode( w/ label, b, name, imm as const)
//   return new ParseNode(b, new ConstantStatement(name, imm), label); ???

   
   _tab = orig;
   return NULL;
}

ParseError::ParseError(const char *description, QTextBlock *b, int lineNo)
   : QString(description), m_textBlock(b), m_lineNo(lineNo)
{ }

int ParseError::getLineNo() const {
   return m_lineNo;
}

QTextBlock *ParseError::getTextBlock() const {
   return m_textBlock;
}

