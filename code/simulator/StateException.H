#ifndef STATEEXCEPTION_H_
#define STATEEXCEPTION_H_

#include <QString>
#include <QObject>
#include "typedefs.H"
#include "Debugger.H"

class InvalidAddress : public StateException {
	public:
		InvalidAddress(unsigned int attemptedAddress, QTextBlock *b) : 
			StateException(QString("Attempt to access invalid address: %1").arg(attemptedAddress), b), m_attemptedAddress(attemptedAddress) {}
		unsigned int m_attemptedAddress;
};

class BusError : public StateException {
	public:
		BusError(unsigned int attemptedAddress, QTextBlock *b) : 
			StateException("Attempt to read/write a word from an address not divisible by 4", b),
			m_attemptedAddress(attemptedAddress) {}
		unsigned int m_attemptedAddress;
};

class InvalidRegister : public StateException {
	public:
		InvalidRegister(int attemptedRegister, QTextBlock *pc) : 
			StateException("Attempt to access invalid register", pc),
			m_attemptedRegister(attemptedRegister) {}
		int m_attemptedRegister;
};

class AssertionFailure : public StateException {
   public:
      AssertionFailure(int val1, int val2, QTextBlock *pc) : 
            StateException("Assertion failure.", pc),
            m_val1(val1), m_val2(val2) {}
      int m_val1, m_val2;
};

class DivideByZero : public StateException {
   public:
      DivideByZero(QTextBlock *pc) : StateException("Divide by Zero Exception", pc) { }
};

class ArithmeticOverflow : public StateException {
   public:
      ArithmeticOverflow(QTextBlock *pc) : StateException("Overflow Exception", pc) { }
};



#endif // STATEEXCEPTION_H_

