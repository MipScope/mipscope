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
   file: SyntaxHighlighter.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#include "SyntaxHighlighter.H"
#include "SyntaxList.H"
#include "Utilities.H"
#include <QTextDocument>
#include <QStringList>
#include <string.h>
#include <QRegExp>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
   setupHighlightingRules();
}

SyntaxHighlighter::SyntaxHighlighter(QTextEdit *parent) : QSyntaxHighlighter(parent) {
   setupHighlightingRules();
}

void SyntaxHighlighter::setupHighlightingRules() {
   HighlightingRule rule;
   QTextCharFormat instructionFormat, directiveFormat;
   QTextCharFormat registerFormat, specialFormat;
   QTextCharFormat labelFormat, commentFormat;
   
   instructionFormat.setForeground(Qt::blue);
   //instructionFormat.setFontWeight(QFont::Bold);
   directiveFormat.setForeground(Qt::darkMagenta);
   registerFormat.setForeground(QColor::fromRgb(127, 0, 0));
   specialFormat.setForeground(Qt::red);
   labelFormat.setForeground(Qt::darkCyan);
   commentFormat.setForeground(Qt::darkGreen);
   
   addRules(Instructions, "\\b%s\\b", instructionFormat);
   addRules(Directives, "\\.\\b%s\\b", directiveFormat);
   addRules(Registers, "%s", registerFormat);
   
   rule.pattern = QRegExp(LabelSyntax);
//   rule.pattern.setPatternSyntax(QRegExp::WildCard);
   rule.format = labelFormat;
   m_rules.append(rule);
   
   addRules(SpecialSyntax, "%s", specialFormat);

   rule.pattern = QRegExp(CommentSyntax);
   rule.format = commentFormat;
   m_rules.append(rule);
}

void SyntaxHighlighter::addRules(const char *const list[], const char *regExpFormat, QTextCharFormat &format) {
   HighlightingRule rule;
   QStringList patterns;
   char buf[512];
   int i = 0;
   
   while(list[i] != NULL) {
      sprintf(buf, regExpFormat, list[i++]);
      
      patterns = patterns << buf;
   }
   
   foreach (QString pattern, patterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = format;
      
      m_rules.append(rule);
   }
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
   foreach (HighlightingRule rule, m_rules) {
      QRegExp expression(rule.pattern);
      int index = text.indexOf(expression);
      
      while (index >= 0) {
         int length = expression.matchedLength();
         setFormat(index, length, rule.format);
         index = text.indexOf(expression, index + length);
      }
   }
}

// check if one word matches any of the reserved keywords
bool SyntaxHighlighter::matches(const QString &text) {
   foreach (HighlightingRule rule, m_rules) {
      QRegExp expression(rule.pattern);
      //if (expression.exactMatch(text))
      //   return true;
      int index = text.indexOf(expression);
      
      if (index >= 0)
         return true;
   }
   
   return false;
}

