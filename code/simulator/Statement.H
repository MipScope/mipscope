#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <QObject>
#include <QHash>

#include "State.H"
#include "StatementInfo.H"

class Statement : public QObject {
	
	Q_OBJECT
	
	public:
		Statement(void);
		static QHash<QString, StatementInfo>* getStatementInfoHash(void);
		
		virtual void execute(State* state) const = 0;
		
	private:
		static QHash<QString, StatementInfo>* s_statementInfoHash;
};

/*
class Add : public Statement {

	public:
		static bool isValid(QString line, ParseNode* before);
		static Statement* factory(QString line, ParseNode* before);
				
		AddStatement(REGISTER src1, REGISTER src2, REGISTER dest);
		void execute(State* state);
		
		static prefix = "add"
		static description = "adds two registers, extends sign to width of register"
		
		
	private:
		REGISTER m_src1, m_src2, m_dest;
	
}
*/




#endif /*STATEMENT_H_*/
