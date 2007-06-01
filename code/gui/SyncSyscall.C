#include "SyncSyscall.H"

#include <QApplication>
#include "SyscallHandler.H"

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
    emit sig_syscall(m_handler, s, status, syscallNo, valueOfa0);
}

void LocalSyscallProxy::undoSyscall(int syscallNo)
{
    emit sig_undoSyscall(m_handler, syscallNo);
}

void LocalSyscallProxy::reset()
{
    emit sig_reset(m_handler);
}

void LocalSyscallProxy::connectTo(QObject *mainProxy)
{   
    Qt::ConnectionType connectionKind;
    
    // if we're already in the main thread, a blocking queued connection would deadlock
    // in this case use a plain old signal
    if (this->thread() != mainProxy->thread())
    {
        connectionKind = Qt::BlockingQueuedConnection;
    }
    else
    {
        connectionKind = Qt::DirectConnection;
    }
    
    // make the connections between this and the main thread proxy
    connect(this, SIGNAL(sig_syscall(SyscallHandler*, State*, int, int, int)),
            mainProxy, SLOT(slot_syscall(SyscallHandler*, State*, int, int, int)),
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
void MainSyscallProxy::slot_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0)
{
    inHandler->syscall(s, status, syscallNo, valueOfa0);
}

void MainSyscallProxy::slot_undoSyscall(SyscallHandler *inHandler, int syscallNo)
{
    inHandler->undoSyscall(syscallNo);
}

void MainSyscallProxy::slot_reset(SyscallHandler *inHandler)
{
    inHandler->reset();
}