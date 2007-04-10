/* ---------------------------------------------- *\
   file: EditorPane.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __QEDITOR_PANE_H__
#define __QEDITOR_PANE_H__
#include <QTabWidget>
#include "TextEditor.H"

class QPoint;
class SyntaxHighlighter;
class TextEditor;
class Gui;

class EditorPane : public QTabWidget {
   
   Q_OBJECT
   
   public:
      friend class Gui;
      
      EditorPane(Gui *parent, const char *fileName = NULL);
	   virtual ~EditorPane(void);

      void setActiveEditor(TextEditor *newlyActive);
      void openFile(const QString &fileName);
      
   //signals:
      
      
   private slots:
      void activeEditorChanged(int index);
      void contextMenuRequested(const QPoint & pos);
      
   protected:
      TextEditor *m_activeEditor;
      SyntaxHighlighter *m_highlighter;
      Gui *m_parent;
};

#endif // __QEDITOR_PANE_H__

