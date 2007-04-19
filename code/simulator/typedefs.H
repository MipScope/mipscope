#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <QString>

#include "Statement.H"
#include "ParseNode.H"

typedef int TIMESTAMP;
typedef unsigned int ADDRESS;
typedef unsigned int REGISTER;
typedef unsigned int WORD;
typedef unsigned char BYTE;
typedef Statement*(*STATEMENT_FACTORY)(const QString&, ParseNode*);
typedef bool(*STATEMENT_IS_VALID)(const QString&, ParseNode*);  

#endif /*TYPEDEFS_H_*/
