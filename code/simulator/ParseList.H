/* ---------------------------------------------- *\
   file: ParserList.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef PARSELIST_H_
#define PARSELIST_H_
#include "typedefs.H"

class QTextDocument;
class parseNode;

class ParseList {

   public:
      ParseList(QTextDocument *document);
      
      ParseNode *first();
      ParseNode *last();
      
      // no need to tell ParseList where to insert new node; it just knows
      // cause it's da shit
      void insert(ParseNode *newNode);
      
   private:
      QTextDocument *m_source;
};

#endif // PARSELIST_H_

