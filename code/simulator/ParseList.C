/* ---------------------------------------------- *\
   file: ParserList.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#include "ParseList.H"
#include "Parser.H"
#include "ParseNode.H"
#include "StatementArgList.H"
#include "Identifier.H"
#include "Statement.H"

#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>

ParseList::ParseList(QTextDocument *document) :
   m_source(document)
{ }

ParseNode *ParseList::first() {
   return NULL;
   //	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}

ParseNode *ParseList::last() {
   return NULL;
}


