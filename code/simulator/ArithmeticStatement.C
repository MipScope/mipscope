#include "ArithmeticStatement.H"

#include <QString>

/* Add */
QString Add::prefix("Add");
QString Add::description("adds two registers, extends sign to width of register");

// static
bool Add::isValid(QString line, ParseNode* before) {
	return true;
}

//static 
Statement* Add::factory(QString line, ParseNode* before) {
	return new Add(1, 2, 3);
}

//constructor
Add::Add(REGISTER src1, REGISTER src2, REGISTER dest) {	

}
void Add::execute(State* state) {
}
