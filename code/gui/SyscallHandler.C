/* ---------------------------------------------- *\
   file: SyscallHandler.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/27/2007
\* ---------------------------------------------- */
#include "SyscallHandler.H"
#include "Utilities.H"
#include "Gui.H"
#include "OutputConsole.H"
#include "../simulator/State.H"

// Handles all syscalls outputted by a Program
// -------------------------------------------
SyscallListener::SyscallListener(Gui *gui) 
   : QObject(gui), m_gui(gui)
{ }

void SyscallListener::registerHandler(SyscallHandler *handler) {
   if (handler == NULL)
      return;
   
   int syscallNo = handler->getSyscallNo();

   if (syscallNo == S_PRINT) { // convenience for OutputConsole
      m_syscallMap[S_PRINT_INT]    = handler;
      m_syscallMap[S_PRINT_FLOAT]  = handler;
      m_syscallMap[S_PRINT_DOUBLE] = handler;
      m_syscallMap[S_PRINT_STRING] = handler;
      m_syscallMap[S_PRINT_CHAR]   = handler;
   } else m_syscallMap[syscallNo] = handler;
}

void SyscallListener::syscall(State *s, int status, int syscallNo) {
   if (m_syscallMap.contains(syscallNo))
      m_syscallMap[syscallNo]->syscall(s, status, syscallNo);
}

void SyscallListener::undoSyscall(int syscallNo) {
   if (m_syscallMap.contains(syscallNo))
      m_syscallMap[syscallNo]->undoSyscall(syscallNo);
}

void SyscallListener::reset() {
   foreach(SyscallHandler *sh, m_syscallMap.values())
      sh->reset();
   
   //m_syscallMap.clear();
}

SyscallHandler::SyscallHandler(SyscallListener *listener, int mySyscallNo, bool handlesUndo) : m_syscallNo(mySyscallNo), m_handlesUndo(handlesUndo)
{
   listener->registerHandler(this);
}

SyscallHandler::~SyscallHandler() { }

int SyscallHandler::getSyscallNo() const {
   return m_syscallNo;
}

bool SyscallHandler::handlesUndo() const {
   return m_handlesUndo;
}

