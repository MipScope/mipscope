#ifndef PARSELIST_H_
#define PARSELIST_H_

#include <QObject>

#include "typedefs.H"

class QTextDocument;
class StatementArg;
//class TextEditor;
extern const char *const Registers[];

class ParseList : public QObject {

   Q_OBJECT

   public:
      ParseList(QTextDocument *document);//TextEditor* textEditor);

      ParseNode* getFirst(void);
      
      static ParseList *parseDocument(QTextDocument *document);
      static ParseNode *parseLine(QTextBlock *b);
      static StatementArg *parseArg(QString text, int no);
      static Identifier *parseIdentifier(QString text);
      
      // Why doesn't QString have this functinality???
      static QString substring(QString s, int startIndex, int endIndex);
      static QString substring(QString s, int start, int length);
      
   private:
      QTextDocument *m_source;
      //		TextEditor* m_textEditor;

};

#endif /*PARSELIST_H_*/
