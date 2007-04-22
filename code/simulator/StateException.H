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
		InvalidAddress(ADDRESS attemptedAddress) : 
			StateException("Attempt to access invalid address"),
			m_attemptedAddress(attemptedAddress) {}
		ADDRESS m_attemptedAddress;
};

class BusError : public StateException {
	public:
		BusError(ADDRESS attemptedAddress) : 
			StateException("Attempt to read/write a word from an address not divisible by 4"),
			m_attemptedAddress(attemptedAddress) {}
		ADDRESS m_attemptedAddress;
};

class InvalidRegister : public StateException {
	public:
		InvalidRegister(int attemptedRegister) : 
			StateException("Attempt to access invalid register"),
			m_attemptedRegister(attemptedRegister) {}
		int m_attemptedRegister;
};

#endif // STATEEXCEPTION_H_

