#ifndef PARSENODE_H_
#define PARSENODE_H_

#include <QObject>
#include <QString>
#include <QTextBlockUserData>

#include "typedefs.H"
#include "State.H"
#include "Statement.H"
#include "ParseNode.H"

class ParseNode : private QTextBlockUserData {
	
	Q_OBJECT
	
	public:
		ParseNode(QTextBlock* textBlock, Statement* statement);
		~ParseNode(void);
		
		TIMESTAMP getFirstExecuted(void);
		Statement* getStatement(void);
		
		ParseNode* getNext(void);
		ParseNode* getPrevious(void);
				
		void execute(State* state);		
		
	private:
		TIMESTAMP m_firstExecuted;
		QString m_label;
		Statement* m_statement;
		QTextBlock* m_textBlock;
		
			
};

#endif /*PARSENODE_H_*/
