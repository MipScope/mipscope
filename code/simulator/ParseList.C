#include <QObject>
#include <QTextBlock>

#include "ParseNode.H"
#include "ParseList.H"
#include "../gui/Gui.H"
#include "../gui/TextEditor.H"
#include "../gui/Utilities.H"

ParseList::ParseList(TextEditor* textEditor) :
	m_textEditor(textEditor) { }

ParseNode* ParseList::getFirst(void) {
	return static_cast<ParseNode*> (m_textEditor->getBlockForLine(0)->userData());	
}
