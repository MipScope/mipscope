/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

#ifndef STATEEXCEPTION_H_
#define STATEEXCEPTION_H_

#include <QString>
#include <QObject>
#include "typedefs.H"
#include "Debugger.H"

class InvalidAddress : public StateException {
	public:
		InvalidAddress(unsigned int attemptedAddress, QTextBlock *b) : 
			StateException(QString("Attempt to access invalid address: %1").arg(QString::number(attemptedAddress, 16)), b), m_attemptedAddress(attemptedAddress) {}
		unsigned int m_attemptedAddress;
};

class BusError : public StateException {
	public:
		BusError(unsigned int attemptedAddress, QTextBlock *b) : 
			StateException("Attempt to read/write a word from an address not divisible by 4", b),m_attemptedAddress(attemptedAddress) {}
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

class ExitSyscallHandlerCauseIAMTooLazyToWriteThisProperly : public StateException {
   public:
      ExitSyscallHandlerCauseIAMTooLazyToWriteThisProperly(QTextBlock *pc)
         : StateException("", pc) { }
};

class BreakError : public StateException {
   public:
      BreakError(const QString &message, QTextBlock *pc)
         : StateException(message, pc) { }
};

// This is the only exception that can safely be called from proxied syscalls:
class SyscallError : public StateException {
public:
	SyscallError (const QString& message, QTextBlock* pc) : StateException(message, pc) { }
};

#endif // STATEEXCEPTION_H_

