#include "StatementInfo.H"
#include "Statement.H"

class Add;
//class Addu;
//class Sub;
//class Subu;

/* Add */
static Add::prefix = "add"
static Add::description = "adds two registers, extends sign to width of register"

static bool Add::isValid(QString line, ParseNode* before) {
	return true;
}

static Statement* Add::factory(QString line, ParseNode* before) {
	return new Statement(1, 2, 3);
}
				
Add::Add(REGISTER src1, REGISTER src2, REGISTER dest) {		
}
void Add::execute(State* state) {
}

