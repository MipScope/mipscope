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

