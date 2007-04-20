#include "Statement.H"
#include "StatementInfo.H"

Statement::Statement(void) {}

QHash<QString, StatementInfo>* Statement::s_statementInfoHash = NULL;

// static
QHash<QString, StatementInfo>* Statement::getStatementInfoHash(void) {
	
	if (s_statementInfoHash == NULL) {
		s_statementInfoHash = new QHash<QString, StatementInfo>();
		
		//s_statementInfoHash[Add::s_prefix] =
	}
	return s_statementInfoHash;
}
