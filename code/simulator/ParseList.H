#ifndef PARSELIST_H_
#define PARSELIST_H_

#include <QObject>

#include "typedefs.H"

#include "../gui/TextEditor.H"

class ParseList : public QObject {
	
	Q_OBJECT
	
	public:
		ParseList(TextEditor* textEditor);
		
		ParseNode* getFirst(void);				
		
	private:
		TextEditor* m_textEditor;
	
};

#endif /*PARSELIST_H_*/
