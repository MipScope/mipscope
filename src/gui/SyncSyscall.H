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
    file: SyncSyscall.H
    auth: Brian Moore
    acct: bjmoore
    date: 6/1/2007
 \* ---------------------------------------------- */

#ifndef _SYNCSYSCALL_H_
#define _SYNCSYSCALL_H_

#include <QObject>
#include <memory>

class State;
class SyscallHandler;
class SyscallError;

// this class acts as a signal emitter in the current thread
// its intended to be created, used once, then destroyed
// it's only purpose in life is to get around the fact that
//    the SyscallListener is a) not always called in the same thread
//                       and b) not always called from the thread it belongs to
class LocalSyscallProxy : public QObject
{
    Q_OBJECT
    public:
        LocalSyscallProxy(SyscallHandler *inHandler, QObject *mainProxy = NULL);
        
        void syscall(State *s, int status, int syscallNo, int valueOfa0);
        void undoSyscall(int syscallNo);
        void reset();
        
        void connectTo(QObject *mainProxy);
        
    signals:
        void sig_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0, std::auto_ptr<SyscallError>*);
        void sig_undoSyscall(SyscallHandler *inHandler, int syscallNo);
        void sig_reset(SyscallHandler *inHandler);
        
    private:
        SyscallHandler *m_handler;
};

// all instances of this class will push themselves into the main thread
// otherwise it is simply an adaptor to run syscall, undoSyscall and reset on the given handler
class MainSyscallProxy : public QObject
{
    Q_OBJECT
    
    public:
        MainSyscallProxy();
        
    public slots:
        void slot_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0, std::auto_ptr<SyscallError>*);
        void slot_undoSyscall(SyscallHandler *inHandler, int syscallNo);
        void slot_reset(SyscallHandler *inHandler);
};

#endif /* _SYNCSYSCALL_H_ */

