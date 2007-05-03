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
#include "StackView.H"
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
   connect(s, SIGNAL(memoryChanged(unsigned int, unsigned int, ParseNode*)), this, SLOT(memoryChangeReceived(unsigned int, unsigned int, ParseNode*)));
   connect(s, SIGNAL(registerChanged(unsigned int, unsigned int, ParseNode*)), this, SLOT(registerChangeReceived(unsigned int, unsigned int, ParseNode*)));
   connect(s, SIGNAL(pcChanged(ParseNode*)), this, SLOT(pcChangeReceived(ParseNode*)));
   
   // Initialize relationship between Proxy and Debugger
   connect(m_debugger, SIGNAL(programStatusChanged(int)), this, SLOT(programStatusChangeReceived(int)));
   connect(m_debugger, SIGNAL(programTerminated(int)), this, SLOT(programTerminated(int)));
   connect(m_debugger, SIGNAL(notifyPause(const QString&)), this, SLOT(notifyPause(const QString&)));
   
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
   connect(this, SIGNAL(validityChanged(bool)), m_gui, SLOT(validityChanged(bool)));

   // Initialize relationship between parent TextEditor and Proxy
   connect(m_parent, SIGNAL(jumpTo(const QTextBlock&)), this, SLOT(jumpTo(const QTextBlock&)));
   connect(this, SIGNAL(pcChanged(ParseNode*)), m_parent, SLOT(pcChanged(ParseNode*)));
   connect(this, SIGNAL(programStatusChanged(int)), m_parent, SLOT(programStatusChanged(int)));
   connect(this, SIGNAL(registerChanged(unsigned int, unsigned int, int, ParseNode*)), m_gui, SLOT(registerChanged(unsigned int, unsigned int, int, ParseNode*)));
   connect(this, SIGNAL(memoryChanged(unsigned int, unsigned int, ParseNode*)), m_gui, SLOT(memoryChanged(unsigned int, unsigned int, ParseNode*)));

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

void Program::getLastXInstructions(int no, QVector<ParseNode*> &instrs) const
{
   getState()->getLastXInstructions(no, instrs);
}

// Slots outside of Debugger/Gui Relationship
// ------------------------------------------
void Program::currentChanged(TextEditor *cur) {
   ErrorConsole *err;

   // TODO:  make sure this m_current sheise is correct when active program
   // possibly disable changing of program??  Or possibly.. multiple programs 
   // running at once?!
   
   if ((m_current = (cur == m_parent)) && (err = m_gui->getErrorConsole()) != NULL)
      err->updateSyntaxErrors(m_syntaxErrors, m_parent, false);
}


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
   
   if (m_current && getStatus() == PAUSED && pc == getPC()) {
      pcChanged(pc);
      m_gui->getStackView()->updateDisplay(getState(), PAUSED);
   }
}

void Program::registerChangeReceived(unsigned int reg, unsigned int current, ParseNode *pc) {
//   cerr << "reg " << reg << " = " << current << endl;

   if (m_current)// && getStatus() == PAUSED)
      registerChanged(reg, current, getStatus(), pc);
}

void Program::memoryChangeReceived(unsigned int address, unsigned int value, ParseNode *pc) {
   if (m_current && getStatus() == PAUSED)
      memoryChanged(address, value, pc);
}

void Program::programStatusChangeReceived(int s) {
   if (m_current) {
      if (s == STOPPED) {
         disconnect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
         setRunnable(true);
      }
      
      emit programStatusChanged(s);
      
      if (s != RUNNING) {
         m_gui->getStackView()->updateDisplay(getState(), s);
         m_gui->getRegisterView()->updateDisplay(s);
      }
   }
}

void Program::programTerminated(int reason) {
   updateSyntaxErrors(NULL);
   if (m_gui->handleProgramTerminated(reason))
      return;
   
   if (STATUS_BAR == NULL)
      return;
   
   const int delay = STATUS_DELAY + 6000;
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
      case T_FORCE_ROLLBACK:
         STATUS_BAR->showMessage(QString("Execution halted:  Program %1 rolled back past entry point!").arg(name), delay);
         break;
      case T_ABNORMAL:
      default:
         STATUS_BAR->showMessage(QString("Warning: Program %1 terminated abnormally.").arg(name), delay);
         break;
   }
}

// Display a message notifying user of reason for pause in program execution in the status bar
void Program::notifyPause(const QString &reason) const {
   if (STATUS_BAR != NULL)
      STATUS_BAR->showMessage(reason, STATUS_DELAY + 3500);
}


// --------------------------
// Slots from Gui -> Debugger
// --------------------------
void Program::stop() {
   if (m_current)
      m_debugger->programStop();
}

// if the user rolls back before the program entry point
void Program::forceStop() {
   m_debugger->programForceStop();
}

void Program::pause() {
   if (m_current)
      m_debugger->programPause();
}

void Program::updateSyntaxErrors(SyntaxErrors *newS) {
   m_syntaxErrors = newS;
   
   m_parent->updateSyntaxErrors(newS);
   m_gui->getErrorConsole()->updateSyntaxErrors(newS, m_parent);
//   m_parent->
}

void Program::run() {
   if (!m_current)
      return;
   
   int status = getStatus();
   
   if (status == STOPPED) {
      // reparse program; ensure it was successful
      
      try {
         m_parseList = Parser::parseDocument(m_parent->document());
      } catch(SyntaxErrors &e) {
         updateSyntaxErrors(new SyntaxErrors(e));
         return;
      }
      
      m_parseList->setInteractiveProgram(this);
      if (!m_parseList->isValid()) {
         updateSyntaxErrors(m_parseList->getSyntaxErrors());
         return;
      }
      
      m_parent->clearLastInstructions();
      updateSyntaxErrors(NULL);
      m_debugger->setParseList(m_parseList);
      
      m_debugger->setWatchpoints(getWatchpoints());

      // content changed during Pause textEditor->notifies Proxy
      connect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
   } else if (status == RUNNING) {
      pause();
      return;
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
   
   
   
   // TODO!  TODO
   


   
//   if (m_current)
//      m_debugger->jumpTo(b);
}

// called whenever the status of a register watchpoint changes
void Program::watchPointModified(unsigned int reg, bool watchPoint) {
   m_debugger->watchPointModified(reg, watchPoint);
}

bool *Program::getWatchpoints() const {
   return m_gui->getWatchpoints();
}

void Program::contentsChange(int position, int charsRemoved, int charsAdded) {
   if (getStatus() == PAUSED) {
      QTextDocument *doc = m_parent->getTextDocument();
      
      QTextCursor c = m_parent->textCursor();
      int originalPosition = c.position();
      c.setPosition(position);
      //c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
      int affected = position + (charsRemoved > charsAdded ? charsRemoved : charsAdded);
      int oldPosition;
      TIMESTAMP newlyInsertedTimestamp = MAX_TIMESTAMP;
      
      // TODO:  make this affected area a tighter/exact bound!!
      //    Add so every ParseNode knows about its text() and do a 
      // diff, basically to see the changes..
      
      
      cerr << "\n>>> CONTENTS CHANGED: " << charsRemoved << " removed; " << charsAdded << " added" << endl;
      
      //cerr << "Pos: " << position << "; " << doc->findBlock(position).text().toStdString() << endl;//",   after pos: " << c.selectedText().toStdString() << endl;
      
      do {
         const QTextBlock &block = doc->findBlock(position);
         ParseNode *modified = ParseNode::Node(block);
         QString newText = block.text();
         Parser::trimCommentsAndWhitespace(newText);
         
         if (modified != NULL) {
            // TODO:  check if it actually changed?
            // add an equality operator to Statement*
            
            if (modified->getText() != newText) {
               modified->setText(newText);
               m_modified.push_back(modified);
            }
         } else if (newText != "") {
            cerr << "SPECIAL CASE: '" << block.text().toStdString() << "'\n";
            
            // Inserting a completely new text block; must roll back to the 
            // minimum timestamp of the previous and next blocks
            ParseNode *prev = ParseList::getClosestInstructionBefore(ParseNode::Node(block.previous()));
            
            if (prev != NULL) {
               TIMESTAMP timestamp = prev->getFirstExecuted();
               cerr << "prev timestamp = " << timestamp << endl;
               
               if (timestamp != CLEAN_TIMESTAMP && timestamp < newlyInsertedTimestamp)
                  newlyInsertedTimestamp = timestamp + 1;
            }
            
            ParseNode *next = ParseList::getClosestInstruction(ParseNode::Node(block.next()));
            
            if (next != NULL) {
               TIMESTAMP timestamp = next->getFirstExecuted();
               cerr << "next timestamp = " << timestamp << endl;
               
               cerr << "next: " << ParseNode::Node(block.next()) << ", segment = " << ParseList::getSegment(ParseNode::Node(block.next())) << endl;
               
               if (timestamp != CLEAN_TIMESTAMP && timestamp < newlyInsertedTimestamp)
                  newlyInsertedTimestamp = timestamp;
            }

            /*if (newlyInsertedTimestamp == MAX_TIMESTAMP) { // should never happen
               stop();
               return;
            }*/
         }
         
         //else cerr << "\n\nErr: modified = NULL for block: '" << block.text().toStdString() << "'\n";
         
         c.movePosition(QTextCursor::NextBlock);
         oldPosition = position;
         position = c.position();
      } while(position <= affected && position != oldPosition);
      
      // Update program State
      if (!m_modified.empty() || newlyInsertedTimestamp < MAX_TIMESTAMP)
         rollBackToEarliest(newlyInsertedTimestamp);
      
      // Update validity of program
      try {
         m_parseList->updateSyntacticValidity(getState());
      } catch(SyntaxErrors &e) {
         cerr << "\tProgram is invalid; " << e.size() << " syntax errors\n";
         c.setPosition(originalPosition);
         m_parent->setTextCursor(c);
         setRunnable(false);
         updateSyntaxErrors(new SyntaxErrors(e));
         
         return;
      }
      
      SyntaxErrors *semanticErrors = m_parseList->getSyntaxErrors();
      if (semanticErrors != NULL && !semanticErrors->empty()) {
         cerr << "\tProgram is invalid; " << semanticErrors->size() << " semantic errors\n";
         c.setPosition(originalPosition);
         m_parent->setTextCursor(c);
         setRunnable(false);
         updateSyntaxErrors(semanticErrors);
         return;
      }
      
      c.setPosition(originalPosition);
      m_parent->setTextCursor(c);
      setRunnable(true);
      updateSyntaxErrors(NULL);
      cerr << "Program is valid! Runnable = true\n";
      setRunnable(true);
   }
}

// called whenever a ParseNode is deleted during a pause in program execution
// ..  necessary because QTextBlocks automatically delete their associated 
// content .. must assure that ParseNode is never referenced in the Gui
void Program::notifyParseNodeDeleted(ParseNode *wasDeleted) {
   if (wasDeleted != NULL)
      m_modified.push_back(wasDeleted);
}

void Program::rollBackToEarliest(TIMESTAMP earliest) {
   TIMESTAMP original = getState()->getCurrentTimestamp();
//   TIMESTAMP earliest = original + 1;
   
   // TODO:  test this very thoroughly
   
   cerr << "rollBackToEarliest: " << m_modified.size() << " modified\n";
   
   foreach(ParseNode *modified, m_modified) {
/*      TIMESTAMP curTimestamp = modified->getFirstExecuted();
      
      if (curTimestamp != CLEAN_TIMESTAMP && curTimestamp < earliest)
         earliest = curTimestamp;*/
      
      //cerr << "Looking at mod timestamp: " << curTimestamp << endl;
      
      // remove this invalid parseNode and any secondary nodes affected 
      // by it (for instance, in the case of preprocessor directive nodes)
      //    also retrieves the minimum timestamp of all nodes removed in 
      //    this call
      TIMESTAMP curTimestamp = m_parseList->remove(modified);
      if (curTimestamp != CLEAN_TIMESTAMP && curTimestamp < earliest)
         earliest = curTimestamp;
   }
   
   m_modified.clear();

   if (earliest <= original) {
      cerr << "<<<ROLLING BACK to " << earliest << endl;
      
      setRunnable(false);
      if (earliest > 1)
         m_debugger->programStepBackwardToTimestamp(earliest - 1);
      else forceStop(); // rolled back past program entry point
   }
}

int Program::lineNumber(ParseNode *parseNode) {
   if (parseNode == NULL)
      return 0;
   
   const QTextBlock *block = parseNode->getTextBlock();
   if (block == NULL)
      return 0;
   
   return m_parent->lineNumber(*block);
}

