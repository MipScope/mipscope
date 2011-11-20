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
   file: Parser.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/21/2007
\* ---------------------------------------------- */
#ifndef __PARSER_H__
#define __PARSER_H__

#include "typedefs.H"
#include <QString>

class ParseList;
class QTextDocument;
class QTextBlock;
class StatementArg;
class Identifier;
class AddressIdentifier;
class ImmediateIdentifier;

// Controls how much debugging output gets sent to cerr (set via command-line)
extern bool VERBOSE;

#define PARSE_ERROR(d, u)        PARSE_ERRORL((d), (u), -1)
#define PARSE_ERRORL(d, u, l)    {_tab = orig; throw ParseError((d), (u), (l)); return NULL; }

class Parser {

   public:
      // Repeatedly calls parseLine until either the entire document 
      // has been parsed, in which case a new (completely initialized)
      // ParseList is returned, or until one or more syntax errors are
      // found, in which case NULL is returned.
      //
      // @throws SyntaxError (vector containing one or more ParseErrors)
      // to show user where the error(s) occurred.
      static ParseList *parseDocument(QTextDocument *document);
      
      // Parses one line of a QTextDocument
      // Returns an initialized ParseNode* if parsing was successful
      // Returns NULL upon syntax error
      static ParseNode *parseLine(QTextBlock *b, ParseList *list);
      
      // Parses given text for one argument of an instruction
      static StatementArg *parseArg(QString text, ParseList *list);
      
      // Searches for label+-imm($reg)
      static AddressIdentifier *parseLabelWithOffset(QString text, ParseList *list);
      
      // Parses the given text for an immediate number  or label identifier
      static Identifier *parseIdentifier(QString text, ParseList *list);
      
      // Parses for an immediate number (in hex, binary, or decimal)
      static ImmediateIdentifier *parseImmediate(QString text, ParseList *list);
      // Parses for valid label names (or identifiers for global data, consts, etc)
      static AddressIdentifier *parseLabel(QString text);
      static bool simplify(QString &text);
      static void trimCommentsAndWhitespace(QString &text);

      // Parses the directive found in the text given
      // (assuming its (possibly optional) label has already been parsed)
      static ParseNode *parseDirective(QTextBlock *b, QString &text, AddressIdentifier *label, ParseList *list);
      
      static ParseNode *parseConstant(QTextBlock *b, QString &text, AddressIdentifier *label, int equalsIndex, ParseList *list);

      // Why doesn't QString have this functinality???
      //static QString substring(QString s, int startIndex, int endIndex);
      static QString substring(QString s, int start, int length);
};

class ParseError : public QString {
   public:
      ParseError(const QString &description, const QString &unrecognized, int length = -1, QTextBlock *b = NULL, int lineNo = -1);
      virtual ~ParseError();
      
      QString getUnrecognized() const;
      QTextBlock *getTextBlock() const;
      void setTextBlock(QTextBlock *b);
      void setUnrecognized(const QString &text);
      
      int getPosition() const;
      int getLength() const;
      int getLineNo() const;
      
      void setPosition(int position);
      void setLineNo(int lineNo);

   protected:
      // string which caused problem (optional)
      QString m_unrecognized;
      // line where problem occurred
      QTextBlock *m_textBlock;
      
      int m_position; // position of parse error
      int m_length;   // length (if available) of unrecognized string
      // string which couldn't be recognized by parser (probably a subset of m_textBlock->text())
      int m_lineNo;

};

#endif // __PARSER_H__

