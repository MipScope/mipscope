/* ---------------------------------------------- *\
   file: Identifier.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/20/2007
\* ---------------------------------------------- */
#include "Identifier.H"
#include "ParseNode.H"
#include <iostream>
#include <exception>
#include <QString>
using namespace std;

Identifier::Identifier(const QString &id) : m_id(id) { }

QString Identifier::getID() const {
   return m_id;
}

bool Identifier::isImmediate() const {
   ImmediateIdentifier *imm = NULL;
   
   try {
      Identifier *i = const_cast<Identifier*>(this);
      imm = dynamic_cast<ImmediateIdentifier*>(i);
   } catch(bad_alloc&) { }

   return (imm != NULL);
}

bool Identifier::isAddress() const {
   return (!isImmediate());
}

AddressIdentifier::AddressIdentifier(const QString &id, ParseNode *p) 
   : Identifier(id), m_parseNode(p), m_offset(0) { }

int AddressIdentifier::getValue() const {
   return m_parseNode->getAddress() + m_offset;
}

int AddressIdentifier::getOffset() const {
   return m_offset;
}

void AddressIdentifier::setOffset(int val) {
   m_offset = val;
}

void AddressIdentifier::setLabelParseNode(ParseNode *p) {
   m_parseNode = p;
}

// Returns the ParseNode containing the label which corresponds to this identifier
ParseNode *AddressIdentifier::getParseNode() const {
   return m_parseNode;
}

ImmediateIdentifier::ImmediateIdentifier(const QString &id, int value) 
   : Identifier(id), m_value(value) {}

int ImmediateIdentifier::getValue() const {
   return m_value;
}

