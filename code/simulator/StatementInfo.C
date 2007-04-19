#include "StatementInfo.H"

StatementInfo::StatementInfo(QString description, STATEMENT_FACTORY factory, STATEMENT_IS_VALID isValid) :
	m_description(description), m_factory(factory), m_isValid(isValid) {}

QString StatementInfo::getDescription(void) {return m_description;}
STATEMENT_FACTORY StatementInfo::getFactory(void) {return m_factory;}
STATEMENT_IS_VALID StatementInfo::getIsValid(void) {return m_isValid;}