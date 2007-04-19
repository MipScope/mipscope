#ifndef ARITHMETICSTATEMENT_H_
#define ARITHMETICSTATEMENT_H_

#include <QString>

class State;
class ParseNode;

#include "Statement.H"

class ArithmeticStatement : public Statement {
	virtual void execute(State* state) const = 0;
};

class Add : public Statement {

	public:
		static QString prefix;
		static QString description;
		
		void execute(State* state) const;
		Add(REGISTER src1, REGISTER src2, REGISTER dest);
		
		static Statement* factory(QString line, ParseNode* before);
		static bool isValid(QString line, ParseNode* before);

};


//class Addu : public ArithmeticStatement {}



#endif /*ARITHMETICSTATEMENT_H_*/
