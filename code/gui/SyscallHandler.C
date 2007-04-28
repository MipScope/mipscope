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
SyscallListener::SyscallListener(Gui *gui, OutputConsole *output)
   : QObject(gui), m_gui(gui), m_output(output)
{
   
   reset();
}

void SyscallListener::registerHandler(SyscallHandler *handler) {
   // TODO
   handler = NULL;
}

void SyscallListener::syscall(State *s, int status, int syscallNo) {
   // TODO
   s = NULL;status=0;syscallNo=0;
}

void SyscallListener::undoSyscall(int syscallNo) {
   // TODO
   syscallNo = 0;
}

void SyscallListener::reset() {
   // TODO
}

SyscallHandler::SyscallHandler(SyscallListener *listener, int mySyscallNo, bool handlesUndo) : m_syscallNo(mySyscallNo), m_handlesUndo(handlesUndo)
{
   listener->registerHandler(this);
}

int SyscallHandler::getSyscallNo() const {
   return m_syscallNo;
}

bool SyscallHandler::handlesUndo() const {
   return m_handlesUndo;
}

SyscallHandler::~SyscallHandler() { }
