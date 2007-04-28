#ifndef STATEEXCEPTION_H_
#define STATEEXCEPTION_H_

#include <QString>
#include "typedefs.H"

class StateException
{
	public:
		StateException(QString message) : m_message(message) {}
		QString getMessage(void) { return m_message; }
	
	private:
		QString m_message;
};

class InvalidAddress : public StateException {
	public:
		InvalidAddress(unsigned int attemptedAddress) : 
			StateException("Attempt to access invalid address"),
			m_attemptedAddress(attemptedAddress) {}
		unsigned int m_attemptedAddress;
};

class BusError : public StateException {
	public:
		BusError(unsigned int attemptedAddress) : 
			StateException("Attempt to read/write a word from an address not divisible by 4"),
			m_attemptedAddress(attemptedAddress) {}
		unsigned int m_attemptedAddress;
};

class InvalidRegister : public StateException {
	public:
		InvalidRegister(int attemptedRegister) : 
			StateException("Attempt to access invalid register"),
			m_attemptedRegister(attemptedRegister) {}
		int m_attemptedRegister;
};

class AssertionFailure : public StateException {
   public:
      AssertionFailure(int val1, int val2) : 
            StateException("Assertion failure."),
            m_val1(val1), m_val2(val2) {}
      int m_val1, m_val2;
};

class DivideByZero : public StateException {
   public:
      DivideByZero() : StateException("Divide by Zero Exception") { }
};



#endif // STATEEXCEPTION_H_

