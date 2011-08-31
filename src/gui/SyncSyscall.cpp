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

/* ---------------------------------------------- *\
   file: SyncSyscall.C
   auth: Brian Moore
   acct: bjmoore
   date: 6/1/2007
\* ---------------------------------------------- */

#include "SyncSyscall.H"

#include <QApplication>
#include <QThread>
#include "SyscallHandler.H"
#include "../simulator/StateException.H"

LocalSyscallProxy::LocalSyscallProxy(SyscallHandler *inHandler, QObject *mainProxy) :
   m_handler(inHandler)
{
   if (mainProxy) // auto-connect to mainProxy
   {
      connectTo(mainProxy);
   }
}

void LocalSyscallProxy::syscall(State *s, int status, int syscallNo, int valueOfa0)
{
   std::auto_ptr<SyscallError> error;
   emit sig_syscall(m_handler, s, status, syscallNo, valueOfa0, &error);
   if (error.get()) {
      throw *error;
   }
}

void LocalSyscallProxy::undoSyscall(int syscallNo)
{
   emit sig_undoSyscall(m_handler, syscallNo);
}

void LocalSyscallProxy::reset()
{
   emit sig_reset(m_handler);
}

using namespace std;
#include <iostream>
void LocalSyscallProxy::connectTo(QObject *mainProxy)
{
   Qt::ConnectionType connectionKind;

   // if we're already in the main thread, a blocking queued connection would deadlock
   // in this case use a plain old signal
//   if (QThread::currentThread() != mainProxy->thread())
   
//   cerr << "this: " << this->thread()->objectName().toStdString() << endl;
//   cerr << "this: " << QThread::currentThreadId() << endl;

   if (this->thread() != mainProxy->thread())
   {
      connectionKind = Qt::BlockingQueuedConnection;
//      cerr << "blocking\n";
   }
   else
   {
      connectionKind = Qt::DirectConnection;
//      cerr << "direct\n";
   }

   // make the connections between this and the main thread proxy
   connect(this, SIGNAL(sig_syscall(SyscallHandler*, State*, int, int, int, std::auto_ptr<SyscallError>*)),
         mainProxy, SLOT(slot_syscall(SyscallHandler*, State*, int, int, int, std::auto_ptr<SyscallError>*)),
         connectionKind);

   connect(this, SIGNAL(sig_undoSyscall(SyscallHandler*, int)),
         mainProxy, SLOT(slot_undoSyscall(SyscallHandler*, int)),
         connectionKind);

   connect(this, SIGNAL(sig_reset(SyscallHandler*)),
         mainProxy, SLOT(slot_reset(SyscallHandler*)),
         connectionKind);
}

MainSyscallProxy::MainSyscallProxy()
{
   // push myself to the main thread
   moveToThread(QApplication::instance()->thread());
}

// the following three methods just relay the call to handler
void MainSyscallProxy::slot_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0, std::auto_ptr<SyscallError>* error)
{
//   cerr << "syscall: " << syscallNo << ", " << QThread::currentThreadId() << endl;
	try {
		inHandler->syscall(s, status, syscallNo, valueOfa0);
	} catch (const SyscallError& e) {
		// Copy into the error argument so we can transfer the exception to the other thread
		error->reset(new SyscallError(e));
	}
}

void MainSyscallProxy::slot_undoSyscall(SyscallHandler *inHandler, int syscallNo)
{
   inHandler->undo_syscall(syscallNo);
}

void MainSyscallProxy::slot_reset(SyscallHandler *inHandler)
{
   inHandler->reset();
}

