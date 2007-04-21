#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QRegExp>

#include "ParseList.H"
#include "ParseNode.H"
#include "StatementArgList.H"
#include "Identifier.H"
//#include "../gui/Gui.H"
//#include "../gui/TextEditor.H"
#include "../gui/Utilities.H"

ParseList::ParseList(QTextDocument *document) :
   m_source(document)
{
   
}

ParseNode* ParseList::getFirst(void) {
   return NULL;
   //	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}

static ParseList *ParseList::parseDocument(QTextDocument *document) {
   ParseList *p = new ParseList(document);
   
   
   
   for(QTextBlock cur = document->begin(); cur != document->end(); cur = cur->next()) {
      ParseNode *n = ParseList::parseLine(cur);

      // TODO:  add if valid; return NULL otherwise
   }
}

// Returns NULL if b does not contain a syntactically-valid line
static ParseNode *ParseList::parseLine(QTextBlock *b) {
   QString text = b->text().simplified(); // removed leading/trailing whitespace
   QString label;

   cerr << "\tParsing text: " << text << endl;

   // remove commented parts
   int commentIndex = text.indexOf('#');
   bool comment, breakPoint = ((b->userState() & B_BREAKPOINT) != 0);

   if ((comment = (commentIndex >= 0)))
      b->truncate(comment);

   if (text == "")
      return new ParseNode(b, NULL);

   // check for directive
   if (text.contains('.')) {
      // TODO
      cerr << "\tParsing directive: " << text << endl;

      return NULL;
   } else if ((index = text.indexOf(':')) >= 0) {
      // we have a label; possibly an instruction as well
      if (index <= 0)
         return NULL;

      label = text.section(':', 0, 0);
      cerr << "\tParsing label: " << label << endl;

      if (label == "")
         cerr << "label :  " << b->text() << "\n" << text << endl;

      // remove label from text and continue parsing
      text = text.remove(0, index);

      if (text == "")
         return new ParseNode(b, NULL);
   }

   cerr << "\tParsing instr: " << text << endl;

   index = text.indexOf(' ');
   QString instr = text.section(' ', 0, 0);
   bool noArgs;
   if ((noArgs = (index < 0 || instr == "")))
      instr = text; // account for instructions w/out args such as 'syscall'
   // remove instr from text
   else text = text.remove(0, index);
   cerr << "\tFound instr: " << instr << "  '" << text << "'  is left" << endl;

   QHash<QString, StatementInfo*> *map = Statement::getStatementInfoHash();
   iterator i = map->find(instr);

   if (i == map->end()) {
      cerr << "instr " << instr << " not found in map\n";
      return NULL; // instr not found in map
   }

   StatementArgList *argList;
   StatementArg *first = NULL, *second = NULL, *third = NULL;

   if (!noArgs) {
      first  = ParseList::parseArg(text.section(',', 0, 0), 0);
      second = ParseList::parseArg(text.section(',', 1, 1), 1);
      third  = ParseList::parseArg(text.section(',', 2, 2), 2);
   }


}

static StatementArg *ParseList::parseArg(QString text, int no) {
   text = text.simplified();
   int regIndex = text.indexOf('$');
   int len = text.length();
   
   if (regIndex < 0) {
      // check if we have an identifier (const, immediate, label, etc)
      // because no $registers appear in text, must be an identifier to be valid
      Identifier *id = parseIdentifier(text);
      
      if (id == NULL)
         return NULL;
      
      return new StatementArg(id);
   }
   
   if (regIndex + 1 >= len)
      return NULL;

   QString reg;
   if (regIndex + 2 >= len)
      reg = ParseList::substring(text, regIndex + 0, 2);
   else reg = ParseList::substring(text, regIndex + 0, 3);
   
   int registerNo = -1;
   if (reg[1] == QChar('r')) {
      reg = ParseList::substring(text, 2, reg.length() - 2);
      
      bool okay = false;
      registerNo = reg.toInt(&okay, 10);
      if (!okay || registerNo < zero || registerNo >= register_count)
         return NULL;
   } else {
      bool found = false;
      // TODO:  determine between aliases
      for(int i = 0; i < register_count; i++) {
         QRegExp r(QString("\\$") + QString(registerAliases[i]));

         if (r.exactMatch(reg)) {
            found = true;
            registerNo = i;
            break;
         }
      }
      
      if (!found)
         return NULL;
   }
   
   // at this point, should know register number
   
   int parenStart = regIndex.indexOf('(');
   bool validParens = (parenStart >= 0);
   if (validParens) {
      int parenEnd = regIndex.indexOf(')');

      if (parenStart > regIndex || parenEnd <= parenStart + 3 || parenEnd < regIndex)
         return NULL; // parenthesis troubles
      
      validParens = true;

      // TODO:  check to ensure characters inbetween (  $r0  )  parenthesis and $reg are all whitespace or empty
   } else { // must have a generic register w/out addressing
      


      return new StatementArg(registerNo);
   }
   
}

static Identifier *ParseNode::parseIdentifier(QString text) {
   if (text == "")
      return NULL;
   
   bool okay = false;
   int value = text.toInt(&okay, 0);
   
   // includes hex and binary conversion :)
   if (okay) // text was parsed as a valid int, according to the c-language!
      return new ImmediateIdentifier(text, value);
   
   // We have an identifier
   QRegExp invalidChars("[^0-9a-zA-Z_]+");
   if (text.indexOf(invalidChars) >= 0)
      return NULL; // invalid char found
   
   QRegExp immediate("\\d"); // matches any digit
   if (text.indexOf(immediate) == 0) // identifier begins w/ digit
      return NULL;
   
   return new Identifier(text, NULL); // don't yet know corresponding ParseNode*
}

static QString ParseList::substring(QString s, int startIndex, int endIndex) {
   return ParseList::substring(s, startIndex, endIndex - startIndex);
}

static QString ParseList::substring(QString s, int start, int length) {
   int max = s.length();
   if (length = -1)
      length = max - start;

   if (length < s.length()) {
      int end = start + length;
      int stripLeft = start - 0, stripRight = max - end;
      
      end -= stripLeft;
      stripRight -= stripLeft;
      s = s.remove(0, stripLeft);
      s = s.remove(end, stripRight);
   }
   
   return s;
}

