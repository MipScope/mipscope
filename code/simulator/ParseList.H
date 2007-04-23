#ifndef PARSELIST_H_
#define PARSELIST_H_

#include "typedefs.H"

class QTextDocument;

class ParseList {

   public:
      ParseList(QTextDocument *document);
      
      ParseNode* getFirst(void);
      
   private:
      QTextDocument *m_source;
};

#endif // PARSELIST_H_

