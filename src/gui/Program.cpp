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
#include "MemoryView.H"
#include "ErrorConsole.H"
#include "../simulator/Debugger.H"
#include "../simulator/ParseList.H"
#include "../simulator/ParseNode.H"
#include "../simulator/Parser.H"
#include "../simulator/State.H"
#include "../simulator/Identifier.H"
#include <QTextBlock>
#include <QTimer>


// Proxy between Gui and Debugger and vice-versa
// ---------------------------------------------
Program::Program(Gui *gui, EditorPane *editorPane, TextEditor *parent)
   : QObject(parent), m_current(true), m_runnable(false), m_rollingBack(R_NORMAL), m_gui(gui), m_editorPane(editorPane), m_parent(parent), m_syntaxErrors(NULL), m_parseList(NULL), m_debugger(new Debugger(gui->getSyscallListener()))
{
   State *s = m_debugger->getState();
   
   // Initialize relationship between Proxy and State
   //connect(s, SIGNAL(syscall(int,int)), this, SLOT(syscallReceived(int,int)));
//   connect(s, SIGNAL(undoSyscall(int)), this, SLOT(undoSyscallReceived(int)));
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
   connect(m_gui, SIGNAL(stepForward(int)), this, SLOT(stepForward(int)));
   connect(m_gui, SIGNAL(stepBackward(int)), this, SLOT(stepBackward(int)));
//   connect(m_parent, SIGNAL(stepBackwardToTimestamp(TIMESTAMP)), this, SLOT(stepBackwardToTimestamp(TIMESTAMP)));
   
   // Proxy -> Gui
   connect(this, SIGNAL(programStatusChanged(int)), m_gui, SLOT(programStatusChanged(int)));
//   connect(this, SIGNAL(syscall(State*,int,int,int)), m_gui->getSyscallListener(), SLOT(syscall(State*,int,int,int)));
//   connect(this, SIGNAL(undoSyscall(int)), m_gui->getSyscallListener(), SLOT(undoSyscall(int)));
   connect(this, SIGNAL(undoAvailabilityChanged(bool)), m_gui, SLOT(programUndoAvailabilityChanged(bool)));
   connect(this, SIGNAL(validityChanged(bool)), m_gui, SLOT(validityChanged(bool)));

   // Initialize relationship between parent TextEditor and Proxy
   connect(m_parent, SIGNAL(jumpTo(const QTextBlock&)), this, SLOT(jumpTo(const QTextBlock&)));
   connect(this, SIGNAL(pcChanged(ParseNode*, bool)), m_parent, SLOT(pcChanged(ParseNode*, bool)));
   connect(this, SIGNAL(programStatusChanged(int)), m_parent, SLOT(programStatusChanged(int)));
   connect(this, SIGNAL(registerChanged(unsigned int, unsigned int, int, ParseNode*)), m_gui, SLOT(registerChanged(unsigned int, unsigned int, int, ParseNode*)));
   connect(this, SIGNAL(memoryChanged(unsigned int, unsigned int, ParseNode*)), m_gui, SLOT(memoryChanged(unsigned int, unsigned int, ParseNode*)));
   connect(this, SIGNAL(updateMemory(Program*,State*,int)), m_gui, SLOT(updateMemory(Program*,State*,int)));

   // Initialize relationship between EditorPane and Proxy
   connect(m_editorPane, SIGNAL(activeEditorChanged(TextEditor*)), this, SLOT(currentChanged(TextEditor*)));
}

bool Program::isRunnable() {
   m_runnable = (m_syntaxErrors == NULL || m_syntaxErrors->size() <= 0);
   
   return m_runnable;
}

void Program::setRunnable(bool isRunnable) {
   m_runnable = isRunnable;
   emit validityChanged(m_runnable);
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
   bool wasCurrent = m_current;
   
   if ((m_current = (cur == m_parent))) { // am current
      if ((err = m_gui->getErrorConsole()) != NULL)
         err->updateSyntaxErrors(m_syntaxErrors, m_parent, false);
   } else if (wasCurrent && getStatus() == RUNNING) {
      // automatically pause program if user switches tabs
      pause();
      
      if (STATUS_BAR != NULL)
         STATUS_BAR->showMessage(QString("Program '%1' paused").arg(m_parent->fileName()));
   }
}


// --------------------------
// Slots from Debugger -> Gui
// --------------------------
/*void Program::syscallReceived(int no, int valueOfa0) {
//   if (no == 1)
//      m_gui->m_output->push(QString(getState()->getRegister(a0)));
   
//   if (m_current);
      syscall(getState(), getStatus(), no, valueOfa0);
}*/

/*void Program::undoSyscallReceived(int no) {
   undoSyscall(no);
}*/

void Program::undoAvailabilityChangeReceived(bool isAvailable) {
   undoAvailabilityChanged(isAvailable);
}

// only when debugger is paused
void Program::pcChangeReceived(ParseNode *pc) {
//   if (pc != NULL)
//      cerr << pc << ", " << getStatus() << endl;
   
   if (m_current && getStatus() == PAUSED && m_rollingBack != R_ROLLING && pc == getPC()) {
      pcChanged(pc, (m_rollingBack == R_JUST_ROLLED));

      updateMemory(this, getState(), PAUSED);
      m_gui->getRegisterView()->pcChanged(pc, false);

//      m_gui->getStackView()->updateDisplay(getState(), PAUSED);
//      m_gui->updateMemoryView(this);
   }
}

void Program::registerChangeReceived(unsigned int reg, unsigned int current, ParseNode *pc) {
//   cerr << "reg " << reg << " = " << current << endl;

   if (m_current)// && getStatus() == PAUSED)
      registerChanged(reg, current, getStatus(), pc);
}

void Program::memoryChangeReceived(unsigned int address, unsigned int value, ParseNode *pc) {
   if (m_current && getStatus() == PAUSED && m_rollingBack != R_ROLLING)
      memoryChanged(address, value, pc);
}

void Program::programStatusChangeReceived(int s) {
   if (m_current) {
      if (s == STOPPED) {
         //disconnect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
         setRunnable(true);
      }
      
      emit programStatusChanged(s);
      
      if (s != RUNNING) {
         //m_gui->getStackView()->updateDisplay(getState(), s);
         RegisterView *regView = m_gui->getRegisterView();
         regView->updateDisplay(s);
         regView->pcChanged(getPC(), false);
         
         updateMemory(this, getState(), s);
//         if (s == PAUSED && !m_debugger->getReasonforPause().isEmpty())
//            notifyPause(m_debugger->getReasonforPause());

         //m_gui->updateMemoryView(this);
      }
   }
}

void Program::programTerminated(int reason) {
   updateSyntaxErrors(NULL);
   if (m_gui->handleProgramTerminated(reason))
      return;
   
   if (STATUS_BAR == NULL)
      return;
   
//   const int delay = STATUS_DELAY + 40000;
   const QString &name = m_parent->fileName();
   
   switch(reason) {
      case T_COMPLETED:
         STATUS_BAR->showMessage(QString("Program %1 completed successfully. %2").arg(name, m_debugger->getDuration()));
         break;
      case T_TERMINATED:
         STATUS_BAR->showMessage(QString("Program %1 terminated. %2").arg(name, m_debugger->getDuration()));
         break;
      case T_INVALID_PROGRAM:
         STATUS_BAR->showMessage(QString("Program %1 contains errors.").arg(name));
         break;
      case T_FORCE_ROLLBACK:
         STATUS_BAR->showMessage(QString("Execution halted:  Program %1 rolled back past entry point!").arg(name));
         break;
      case T_UNCAUGHT_EXCEPTION:
         {
            const ParseError &err = m_debugger->getException();
            STATUS_BAR->showMessage(err);
            ErrorConsole *console = m_gui->getErrorConsole();
            
            if (console != NULL) {
               SyntaxErrors *errors = new SyntaxErrors();
               errors->push_back(err);
               
               console->updateSyntaxErrors(errors, m_parent, true);
            }
         }
         break;
      case T_ABNORMAL:
      default:
         STATUS_BAR->showMessage(QString("Warning: Program %1 terminated abnormally. %2").arg(name, m_debugger->getDuration()));
         break;
   }
}

// Display a message notifying user of reason for pause in program execution in the status bar
void Program::notifyPause(const QString &reason) {
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
   unsigned int size = 0;
   bool noChange;
   
   if (newS == m_syntaxErrors)
      return;
   
   if ((noChange = (m_syntaxErrors != NULL && newS != NULL && 
        (size = m_syntaxErrors->size()) == newS->size()))) {
      
      for(unsigned int i = 0; i < size; i++) {
         if (newS->at(i) != m_syntaxErrors->at(i)) {
            noChange = false;
            break;
         }
      }
   }
   
   m_syntaxErrors = newS;
   
   if (!noChange)
      m_parent->updateSyntaxErrors(m_syntaxErrors);

   if (m_syntaxErrors == NULL) {
      //cerr << "NO Syntax Errors: forcing update!\n";
      forceUpdateOfSyntaxErrors(false);
      //if (!noChange)
      //   STATUS_BAR->clearMessage();
   } else {
      m_syntaxErrorStack.push(noChange);
      QTimer::singleShot(250, this, SLOT(singleShotSyntaxErrors()));
   }
}

void Program::singleShotSyntaxErrors() {
   if (m_syntaxErrorStack.isEmpty())
      return;

   bool noChange = m_syntaxErrorStack.pop();

   if (m_syntaxErrorStack.isEmpty())
      forceUpdateOfSyntaxErrors(noChange);
   //else cerr << "aggregate: " << m_syntaxErrorStack.size() << endl;
}

void Program::forceUpdateOfSyntaxErrors(bool noChange) {
//   cerr << "forcingUpdateOfSyntaxErrors: noChange=" << noChange << endl;
   
   ErrorConsole *err; // ErrorConsole has its own change-checking
   // (can't only check strings since error line numbers might have changed as well)
   if ((err = m_gui->getErrorConsole()) != NULL)
      err->updateSyntaxErrors(m_syntaxErrors, m_parent);
}

// cannot just have default paramaters (SLOTS get messed up w/ them sometimes)
void Program::loadProgram() {
   loadProgram(false);
}

// Parse the program and initialize the Debugger
void Program::loadProgram(bool forceLoad) {
   int status = getStatus();
   
   // content changed during Pause textEditor->notifies Proxy
   // disconnect first to assure only one connection at a time!
   disconnect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
   connect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
   
//   cerr << "Loading\n\n";
   if (status == STOPPED) {
      // reparse program; ensure it was successful
      if (m_parseList == NULL || forceLoad || !m_parseList->isValid()) {
         //safeDelete(m_parseList);
         
         try {
            m_parseList = Parser::parseDocument(m_parent->document());
         } catch(SyntaxErrors &e) {
				if (m_parseList != NULL) {
            	m_parseList->cleanup();
				}
            updateSyntaxErrors(new SyntaxErrors(e));
            return;
         }
         
         m_parseList->setInteractiveProgram(this);
         if (!m_parseList->isValid()) {
            updateSyntaxErrors(m_parseList->getSyntaxErrors());
            delete m_parseList;
            m_parseList = NULL;
            return;
         }
      }

      m_parent->clearLastInstructions();
      updateSyntaxErrors(NULL);
      m_debugger->setParseList(m_parseList);
   }
}

QString Program::getName() {
   QString name = m_parent->fileName();
   if (name.endsWith(".s") && name.length() != 2)
      name = name.left(name.length() - 2);
   
   return name;
}

void Program::run() {
   if (!m_current)
      return;
   
   int status = getStatus();
   if (status == STOPPED) {
      // reparse program; ensure it was successful
      loadProgram(true);
      if (m_parseList == NULL || !m_parseList->isValid())
         return; // errors remain
      
      m_debugger->setParseList(m_parseList);
      m_debugger->setWatchpoints(getWatchpoints());
      m_debugger->setMemoryWatchpoints(m_gui->getMemoryWatchpoints());

      updateMemory(this, getState(), RUNNING);//View(this);
      if (STATUS_BAR != NULL)
         STATUS_BAR->showMessage(QString("Running program %1...").arg(getName()));
      
      // content changed during Pause textEditor->notifies Proxy
//      connect(m_parent->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(contentsChange(int,int,int)));
   } else if (status == RUNNING) {
      if (STATUS_BAR != NULL)
         STATUS_BAR->clearMessage();
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

void Program::stepForward(int noInstructions) {
   if (m_current) {
      m_debugger->programStepForward(noInstructions);
      //pcChanged(getState()->getPC());
   }
}

void Program::stepBackward(int noInstructions) {
   if (m_current)
      m_debugger->programStepBackward(noInstructions);
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
   if (m_parseList == NULL) {
      loadProgram(false);
      if (m_parseList == NULL)
         return;
   }
   
   m_parseList->cleanup();
   if (getStatus() != RUNNING) {
      QTextDocument *doc = m_parent->getTextDocument();
      
      QTextCursor c = m_parent->textCursor();
//      int originalPosition = c.position();
      c.setPosition(position);
      //c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
      int affected = position + (charsRemoved > charsAdded ? charsRemoved : charsAdded);
      int oldPosition;
      TIMESTAMP newlyInsertedTimestamp = MAX_TIMESTAMP;
      
      //cerr << "\n>>> CONTENTS CHANGED: " << charsRemoved << " removed; " << charsAdded << " added" << endl;
      
      //cerr << "Pos: " << position << "; " << doc->findBlock(position).text().toStdString() << endl;//",   after pos: " << c.selectedText().toStdString() << endl;
      
      do {
         const QTextBlock &block = doc->findBlock(position);
         ParseNode *modified = ParseNode::Node(block);
         QString newText = block.text();
         Parser::trimCommentsAndWhitespace(newText);
         
         if (modified != NULL) {
            // TODO: add an equality operator to Statement*
            
            if (modified->getText() != newText) {
               modified->setText(newText);
               m_modified.push_back(modified);
            }
         } else if (newText != "") {
            //cerr << "SPECIAL CASE: '" << block.text().toStdString() << "'\n";
            
            // Inserting a completely new text block; must roll back to the 
            // minimum timestamp of the previous and next blocks
            ParseNode *prev = ParseList::getClosestInstructionBefore(ParseNode::Node(block.previous()));
            
            if (prev != NULL) {
               TIMESTAMP timestamp = prev->getFirstExecuted();
               //cerr << "prev timestamp = " << timestamp << endl;
               
               if (timestamp != CLEAN_TIMESTAMP && timestamp < newlyInsertedTimestamp)
                  newlyInsertedTimestamp = timestamp + 1;
            }
            
            ParseNode *next = ParseList::getClosestInstruction(ParseNode::Node(block.next()));
            
            if (next != NULL) {
               TIMESTAMP timestamp = next->getFirstExecuted();
               //cerr << "next timestamp = " << timestamp << endl;
               
               //cerr << "next: " << ParseNode::Node(block.next()) << ", segment = " << ParseList::getSegment(ParseNode::Node(block.next())) << endl;
               
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
      
      bool shouldUpdate = (!m_modified.empty() || newlyInsertedTimestamp < MAX_TIMESTAMP);
      
      // Update program State
      if (shouldUpdate)
         rollBackToEarliest(newlyInsertedTimestamp);
      
      // Update validity of program
      try {
         m_parseList->updateSyntacticValidity(getState(), m_parent->textCursor().block());
      } catch(SyntaxErrors &e) {
         if (VERBOSE)
            cerr << "\tProgram is invalid; " << e.size() << " syntax errors\n";
         //c.setPosition(originalPosition);
         //m_parent->setTextCursor(c);
         setRunnable(false);
         updateSyntaxErrors(new SyntaxErrors(e));
         return;
      }
      
      SyntaxErrors *semanticErrors = m_parseList->getSyntaxErrors();
      if (semanticErrors != NULL && !semanticErrors->empty()) {
         if (VERBOSE)
            cerr << "\tProgram is invalid; " << semanticErrors->size() << " semantic errors\n";
         //c.setPosition(originalPosition);
         //m_parent->setTextCursor(c);
         setRunnable(false);
         updateSyntaxErrors(semanticErrors);
         return;
      }
      
//      c.setPosition(originalPosition);
//      m_parent->setTextCursor(c);
      setRunnable(true);
      updateSyntaxErrors(NULL);
      //cerr << "Program is valid! Runnable = true\n";
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
   
   if (VERBOSE)
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
   
   if (getStatus() != PAUSED)
      return;

   if (earliest <= original) {
      if (VERBOSE)
         cerr << "<<<ROLLING BACK to " << earliest << endl;
      
      setRunnable(false);
      if (earliest > 1) {
         m_rollingBack = R_ROLLING;
         m_debugger->programStepBackwardToTimestamp(earliest - 1);
         m_rollingBack = R_JUST_ROLLED;
         
         // only one set of updates after rolling back
         pcChangeReceived(getPC());
         memoryChanged(DATA_BASE_ADDRESS, 0, getPC());

         m_rollingBack = R_NORMAL;
      } else forceStop(); // rolled back past program entry point
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

bool Program::undoIsAvailable() const {
   return (m_debugger->getStatusWithoutLocking() == PAUSED && getState()->undoIsAvailable());
}

ParseNode *Program::getDeclaration(const QString &identifier) {
   if (m_parseList != NULL)
      return m_parseList->getDeclaration(identifier);

   return NULL;
}

TIMESTAMP Program::getCurrentTimestamp() const {
   return getState()->getCurrentTimestamp();
}

MemoryUseMap *Program::getMemoryUseMap() const {
   return getState()->getMemoryUseMap();
}

unsigned int Program::getDataSize() const {
   if (m_parseList == NULL)
      return 0;
   
   return m_parseList->getDataSize();
}

LabelMap *Program::getLabelMap() const {
   if (m_parseList == NULL)
      return NULL;

   return m_parseList->getLabelMap();
}

void Program::gotoDeclaration(unsigned int address) {
   if (m_parseList == NULL)
      return;
   
   ParseNode *target = m_parseList->getNodeForDataAddress(address);
   if (target == NULL)
      return;
   
   m_parent->gotoNode(target);
}

bool Program::getLabelForAddress(unsigned int address, QString &label) {
    if (m_parseList == NULL)
      return false;
   
   ParseNode *target = m_parseList->getNodeForDataAddress(address);
   if (target == NULL)
      return false;
   
   AddressIdentifier *addr = target->getLabel();
   if (addr == NULL)
      return false;

   label = addr->getID();
   return true;
}

