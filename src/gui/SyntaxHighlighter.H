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
   file: SyntaxHighlighter.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __SYNTAXHIGHLIGHTER_H__
#define __SYNTAXHIGHLIGHTER_H__
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>

// Listed in order of application
// (order is important; for ex: comments must override other formats)
extern const char *const Instructions[];
extern const char *const Directives[];
extern const char *const Registers[];
extern const char *LabelSyntax;
extern const char *const SpecialSyntax[];
extern const char *CommentSyntax;

class QTextCharFormat;
class QTextEdit;

class SyntaxHighlighter : public QSyntaxHighlighter {
   
   Q_OBJECT
   
   public:
      SyntaxHighlighter(QTextDocument *parent = 0);
      SyntaxHighlighter(QTextEdit *parent);
      bool matches(const QString &text);

   protected:
      void setupHighlightingRules();
      void highlightBlock(const QString &text);
      void addRules(const char *const list[], const char *regExpFormat, QTextCharFormat &format);

   private:
      struct HighlightingRule
      {
         QRegExp pattern;
         QTextCharFormat format;
      };
      
      QVector<HighlightingRule> m_rules;
};

#endif // __SYNTAXHIGHLIGHTER_H__

