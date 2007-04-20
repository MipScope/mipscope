#ifndef PARSELIST_H_
#define PARSELIST_H_

#include <QObject>

#include "typedefs.H"

class TextEditor;

class ParseList : public QObject {
	
	Q_OBJECT
	
	public:
		ParseList(TextEditor* textEditor);
		
		ParseNode* getFirst(void);				
		
	private:
		TextEditor* m_textEditor;
	
};

#endif /*PARSELIST_H_*/
