#ifndef _SYNCSYSCALL_H_
#define _SYNCSYSCALL_H_

#include <QObject>

class State;
class SyscallHandler;

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
        void sig_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0);
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
        void slot_syscall(SyscallHandler *inHandler, State *s, int status, int syscallNo, int valueOfa0);
        void slot_undoSyscall(SyscallHandler *inHandler, int syscallNo);
        void slot_reset(SyscallHandler *inHandler);
};

#endif /* _SYNCSYSCALL_H_ */