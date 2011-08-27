/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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
#include <assert.h>
using namespace std;

Identifier::Identifier(const QString &id) : m_id(id) { }

QString Identifier::getID() const {
   return m_id;
}

bool Identifier::isImmediate() const {
   return dynamic_cast<const ImmediateIdentifier*>(this);
}

bool Identifier::isAddress() const {
   return (!isImmediate());
}

const AddressIdentifier* Identifier::getAddressIdentifier() const {
   return dynamic_cast<const AddressIdentifier*>(this);
}

AddressIdentifier* Identifier::getAddressIdentifier() {
   return dynamic_cast<AddressIdentifier*>(this);
}

AddressIdentifier::AddressIdentifier(const QString &id, ParseNode *p) 
   : Identifier(id), m_parseNode(p), m_offset(0) { }

int AddressIdentifier::getValue() const {
   assert (m_parseNode != NULL);
   
//   cerr << "\tAddressIdentifier::getValue() of " << getID().toStdString() << ", " << m_parseNode << ", " << m_parseNode->getAddress() << endl;
   
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

