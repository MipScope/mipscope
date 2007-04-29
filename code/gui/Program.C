/* ---------------------------------------------- *\
   file: Program.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#include "Program.H"
#include "Utilities.H"
#include "EditorPane.H"
#include "TextEditor.H"
#include "Gui.H"
#include "SyscallHandler.H"
#include "RegisterView.H"
#include "ErrorConsole.H"
#include "../simulator/Debugger.H"
#include "../simulator/ParseList.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Parser.H"
#include "../simulator/State.H"
#include <QTextBlock>


// TODO:  Test how it looks w/ pc/registers/memory constantly updating; not only when paused


// Proxy between Gui and Debugger and vice-versa
// ---------------------------------------------
Program::Program(Gui *gui, EditorPane *editorPane, TextEditor *parent)
   : QObject(parent), m_current(true), m_runnable(false), m_gui(gui), m_editorPane(editorPane), m_parent(parent), m_syntaxErrors(NULL), m_parseList(NULL), m_debugger(new Debugger()) 
{
   State *s = m_debugger->getState();

   // Initialize relationship between Proxy and State
   connect(s, SIGNAL(syscall(int,int)), this, SLOT(syscallReceived(int,int)));
   connect(s, SIGNAL(undoSyscall(int)), this, SLOT(undoSyscallReceived(int)));
   connect(s, SIGNAL(undoAvailabilityChanged(bool)), this, SLOT(undoAvailabilityChangeReceived(bool)));
   connect(s, SIGNAL(memoryChanged(unsigned int, unsigned int)), this, SLOT(memoryChangeReceived(unsigned int, unsigned int)));
   connect(s, SIGNAL(registerChanged(unsigned int, unsigned int)), this, SLOT(registerChangeReceived(unsigned int, unsigned int)));
   connect(s, SIGNAL(pcChanged(ParseNode*)), this, SLOT(pcChangeReceived(ParseNode*)));
   
   // Initialize relationship between Proxy and Debugger
   connect(m_debugger, SIGNAL(programStatusChanged(int)), this, SLOT(programStatusChangeReceived(int)));
   connect(m_debugger, SIGNAL(programTerminated(int)), this, SLOT(programTerminated(int)));
   
   // Initialize relationship between Gui and Proxy
   connect(m_gui, SIGNAL(stop()), this, SLOT(stop()));
   connect(m_gui, SIGNAL(pause()), this, SLOT(pause()));
   connect(m_gui, SIGNAL(run()), this, SLOT(run()));
   connect(m_gui, SIGNAL(stepForward()), this, SLOT(stepForward()));
   connect(m_gui, SIGNAL(stepBackward()), this, SLOT(stepBackward()));
//   connect(m_parent, SIGNAL(stepBackwardToTimestamp(TIMESTAMP)), this, SLOT(stepBackwardToTimestamp(TIMESTAMP)));
   
   // Proxy -> Gui
   connect(this, SIGNAL(programStatusChanged(int)), m_gui, SLOT(programStatusChanged(int)));
   connect(this, SIGNAL(syscall(State*,int,int,int)), m_gui->getSyscallListener(), SLOT(syscall(State*,int,int,int)));
   connect(this, SIGNAL(undoSyscall(int)), m_gui->getSyscallListener(), SLOT(undoSyscall(int)));
   connect(this, SIGNAL(undoAvailabilityChanged(bool)), m_gui, SLOT(programUndoAvailabilityChanged(bool)));

   // Initialize relationship between parent TextEditor and Proxy
   connect(m_parent, SIGNAL(jumpTo(const QTextBlock&)), this, SLOT(jumpTo(const QTextBlock&)));
   connect(this, SIGNAL(pcChanged(ParseNode*)), m_parent, SLOT(pcChanged(ParseNode*)));
   connect(this, SIGNAL(programStatusChanged(int)), m_parent, SLOT(programStatusChanged(int)));
   connect(this, SIGNAL(registerChanged(unsigned int, unsigned int, int)), m_gui, SLOT(registerChanged(unsigned int, unsigned int, int)));

   // TODO:  content changed during Pause textEditor->notifies Proxy
   
   // Initialize relationship between EditorPane and Proxy
   connect(m_editorPane, SIGNAL(activeEditorChanged(TextEditor*)), this, SLOT(currentChanged(TextEditor*)));
}

bool Program::isRunnable() const {
   return m_runnable;
}

void Program::setRunnable(bool isRunnable) {
   m_runnable = isRunnable;
   validityChanged(m_runnable);
}

SyntaxErrors *Program::getSyntaxErrors() const {
   return m_syntaxErrors;
}

int Program::getStatus() const { // gets status of Debugger
   return m_debugger->getStatus();
}

ParseNode *Program::getPC() const {
   return getState()->getPC();
}

State *Program::getState() const {
   return m_debugger->getState();
}

/*void Program::customEvent(QEvent *e) {
//   if (e->type() == )
}*/

// Slots outside of Debugger/Gui Relationship
// ------------------------------------------
void Program::currentChanged(TextEditor *cur) {
   if ((m_current = (cur == m_parent)))
      m_gui->getErrorConsole()->updateSyntaxErrors(m_syntaxErrors, m_parent, false);
}

// TODO:  add contentChanged thing


// --------------------------
// Slots from Debugger -> Gui
// --------------------------
void Program::syscallReceived(int no, int valueOfa0) {
//   if (no == 1)
//      m_gui->m_output->push(QString(getState()->getRegister(a0)));
   
//   if (m_current);
      syscall(getState(), getStatus(), no, valueOfa0);
}

void Program::undoSyscallReceived(int no) {
   undoSyscall(no);
}

void Program::undoAvailabilityChangeReceived(bool isAvailable) {
   undoAvailabilityChanged(isAvailable);
}

// only when debugger is paused
void Program::pcChangeReceived(ParseNode *pc) {
//   if (pc != NULL)
//      cerr << pc << ", " << getStatus() << endl;
   
   if (m_current && getStatus() == PAUSED)
      pcChanged(pc);
}

void Program::registerChangeReceived(unsigned int reg, unsigned int current) {
//   cerr << "reg " << reg << " = " << current << endl;

   if (m_current)// && getStatus() == PAUSED)
      registerChanged(reg, current, getStatus());
}

void Program::memoryChangeReceived(unsigned int address, unsigned int value) {
   if (m_current && getStatus() == PAUSED)
      memoryChanged(address, value);
}

void Program::programStatusChangeReceived(int s) {
   if (m_current)
      emit programStatusChanged(s);
}

void Program::programTerminated(int reason) {
   if (STATUS_BAR == NULL)
      return;
   
   if (m_gui->handleProgramTerminated(reason))
      return;

   const int delay = STATUS_DELAY + 2000;
   const QString &name = m_parent->fileName();

   switch(reason) {
      case T_COMPLETED:
         STATUS_BAR->showMessage(QString("Program %1 completed successfully.").arg(name), delay);
         break;
      case T_TERMINATED:
         STATUS_BAR->showMessage(QString("Program %1 terminated.").arg(name), delay);
         break;
      case T_INVALID_PROGRAM:
         STATUS_BAR->showMessage(QString("Program %1 contains errors.").arg(name), delay);
         break;
      case T_ABNORMAL:
      default:
         STATUS_BAR->showMessage(QString("Warning: Program %1 terminated abnormally.").arg(name), delay);
         break;
   }
}


// --------------------------
// Slots from Gui -> Debugger
// --------------------------
void Program::stop() {
   if (m_current)
      m_debugger->programStop();
}

void Program::pause() {
   if (m_current)
      m_debugger->programPause();
}

void Program::updateSyntaxErrors(SyntaxErrors *newS) {
   m_syntaxErrors = newS;
   
   m_gui->getErrorConsole()->updateSyntaxErrors(newS, m_parent);
//   m_parent->
}

void Program::run() {
   if (!m_current)
      return;
   
   if (getStatus() == STOPPED) {
      // reparse program; ensure it was successful
      
      try {
         m_parseList = Parser::parseDocument(m_parent->document());
      } catch(SyntaxErrors &e) {
         updateSyntaxErrors(new SyntaxErrors(e));
         return;
      }

      if (!m_parseList->isValid()) {
         updateSyntaxErrors(m_parseList->getSyntaxErrors());
         return;
      }
      
      updateSyntaxErrors(NULL);
      m_debugger->setParseList(m_parseList);
   }
   
   m_debugger->programRun();
   
//   if (m_current)
//      programStatusChanged(RUNNING);
}

void Program::stepForward() {
   if (m_current)
      m_debugger->programStepForward();
}

void Program::stepBackward() {
   if (m_current)
      m_debugger->programStepBackward();
}

void Program::stepBackwardToTimestamp(TIMESTAMP stamp) {
   if (m_current)
      m_debugger->programStepBackwardToTimestamp(stamp);
}

void Program::jumpTo(const QTextBlock &b) {
//   if (m_current)
//      m_debugger->jumpTo(b);
}

