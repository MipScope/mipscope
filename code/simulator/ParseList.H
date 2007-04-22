#ifndef PARSELIST_H_
#define PARSELIST_H_

#include "typedefs.H"
#include <QString>

class QTextDocument;
class QTextBlock;
class StatementArg;
class Identifier;
class AddressIdentifier;
class ImmediateIdentifier;
extern const char *const Registers[];

class ParseList {

   public:
      ParseList(QTextDocument *document);
      
      ParseNode* getFirst(void);
      
      // Repeatedly calls parseLine until either the entire document 
      // has been parsed, in which case a new (completely initialized)
      // ParseList is returned, or until one or more syntax errors are
      // found, in which case NULL is returned.
      //
      // TODO:  Have SyntaxError class and make vector<SyntaxError> 
      // available to show user where the error(s) occurred!
      static ParseList *parseDocument(QTextDocument *document);
      
      // Parses one line of a QTextDocument
      // Returns an initialized ParseNode* if parsing was successful
      // Returns NULL upon syntax error
      static ParseNode *parseLine(QTextBlock *b);
      
      // Parses given text for one argument of an instruction
      static StatementArg *parseArg(QString text);
      
      // Searches for label+-imm($reg)
      static AddressIdentifier *parseLabelWithOffset(QString text);
      
      // Parses the given text for an immediate number  or label identifier
      static Identifier *parseIdentifier(QString text);
      
      // Parses for an immediate number (in hex, binary, or decimal)
      static ImmediateIdentifier *parseImmediate(QString text);
      // Parses for valid label names (or identifiers for global data, consts, etc)
      static AddressIdentifier *parseLabel(QString text);
      static bool simplify(QString &text);

      // Parses the directive found in the text given
      // (assuming its (possibly optional) label has already been parsed)
      static ParseNode *parseDirective(QTextBlock *b, QString &text, AddressIdentifier *label);
      
      static ParseNode *parseConstant(QTextBlock *b, QString &text, AddressIdentifier *label, int equalsIndex);

      // Why doesn't QString have this functinality???
      //static QString substring(QString s, int startIndex, int endIndex);
      static QString substring(QString s, int start, int length);
      
   private:
      QTextDocument *m_source;
};

class ParseError : QString {
   public:
      ParseError(const char *description, QTextBlock *b = NULL, int lineNo = -1);
      
      int getLineNo() const;
      QTextBlock *getTextBlock() const;
      
   protected:
      QTextBlock *m_textBlock;
      int m_lineNo;
};

#endif // PARSELIST_H_

