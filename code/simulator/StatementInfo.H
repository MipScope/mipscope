#ifndef STATEMENTINFO_H_
#define STATEMENTINFO_H_

#include <QString>

#include "typedefs.H"
#include "State.H"
#include "Statement.H"
#include "ParseNode.H"

class StatementInfo : public QObject {
	Q_OBJECT
	
	public:
		StatementInfo(QString description, STATEMENT_FACTORY factory, STATEMENT_IS_VALID isValid);
		QString getDescription(void);
		STATEMENT_FACTORY getFactory(void);
		STATEMENT_IS_VALID getIsValid(void);

	private:
		QString m_description;
		STATEMENT_FACTORY m_factory;
		STATEMENT_IS_VALID m_isValid;	
};

#endif /*STATEMENTINFO_H_*/
