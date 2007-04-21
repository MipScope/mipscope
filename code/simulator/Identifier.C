/* ---------------------------------------------- *\
   file: Identifier.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/20/2007
\* ---------------------------------------------- */
#include "Identifier.H"
#include "ParseNode.H"
#include <QString>

Identifier::Identifier(const QString &id) : m_id(id) { }

QString Identifier::getID() const {
   return m_id;
}

AddressIdentifier::AddressIdentifier(const QString &id, ParseNode *p) 
   : Identifier(id), m_parseNode(p) { }

int AddressIdentifier::getValue() const {
   return m_parseNode->getAddress();
}

void AddressIdentifier::setLabelParseNode(ParseNode *p) {
   m_parseNode = p;
}

ImmediateIdentifier::ImmediateIdentifier(const QString &id, int value) 
   : Identifier(id), m_value(value) {}

int ImmediateIdentifier::getValue() const {
   return m_value;
}

