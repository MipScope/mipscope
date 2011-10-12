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
   file: OutputConsole.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#include "OutputConsole.H"
#include "Utilities.H"
#include "EditorPane.H"
#include "Gui.H"
#include "../simulator/State.H"
#include <QtGui>

// Pseudo-Output terminal, capable of printing/undoing output
// ----------------------------------------------------------
OutputConsole::OutputConsole(Gui *gui, EditorPane *editorPane)
   : Console(gui, editorPane, "Output Console"), 
     m_outputHelper(new OutputHelper(this)), m_noUpdates(0)
{
   // initial loadup message displayed in output console
   m_display->setPlainText("Welcome to "PROJECT_NAME" version "PROJECT_VERSION"\n"
	"Copyright (C) 2007 Travis Fischer and Tim O'Donnell\n"
	PROJECT_NAME" comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome "
	"to redistribute it under certain conditions.\nSee the file GPL.txt in the source package for details.");
}

void OutputConsole::push(const QString &newOutput) {
   // TODO:  OPTIMIZE this!!  Bottleneck!  (infinite-loop printout example)
   m_strings.append(newOutput);
/*   QTextCursor c = m_display->textCursor();
   c.movePosition(QTextCursor::End);
   c.insertText(newOutput);*/
   
   //updateDisplay();
}

void OutputConsole::update() {
   updateDisplay();
   
   if (!m_visible)
      m_gui->ensureVisibility(this);
}

void OutputConsole::pop() {
   m_strings.pop_back();
   
   ++m_noUpdates;
   updateBatchSignal();
//   update();
}

void OutputConsole::update_output ()
{
	++m_noUpdates;
	updateBatchSignal();
}


std::string	OutputConsole::prompt_for_string (State* state, int status, std::string prompt_message, size_t max_length)
{
	m_gui->getOutputConsole()->update();
	bool okay = false;
	QString result = QInputDialog::getText(NULL, "Syscall", prompt_message.c_str(), QLineEdit::Normal, "", &okay);
	if (!okay)
		throw InputError();
	return result.toStdString().substr(0, max_length);
}

int		OutputConsole::prompt_for_int (State* state, int status, std::string prompt_message, int min, int max)
{
	m_gui->getOutputConsole()->update();
	bool okay = false;
	int result = QInputDialog::getInteger(NULL, "Syscall", prompt_message.c_str(), 0, min, max, 1, &okay);
	if (!okay)
		throw InputError();

	return result;
}

double		OutputConsole::prompt_for_double (State* state, int status, std::string prompt_message, double min, double max)
{
	m_gui->getOutputConsole()->update();
	bool okay = false;
	double result = QInputDialog::getDouble(NULL, "Syscall", prompt_message.c_str(), 0, min, max, 1, &okay);
	if (!okay)
		throw InputError();

	return result;
}


void		OutputConsole::output_string (State* state, int status, std::string message)
{
	m_outputActions.push_back(new NewOutputAction(this, QString(message.c_str())));
	if (status == PAUSED)
		update_output();
   
}

void		OutputConsole::clear_output (State* state, int status)
{
	m_outputActions.push_back(new ClearOutputAction(this, m_strings));
	if (status == PAUSED)
		update_output();
}

void		OutputConsole::reset ()
{
   flush();
   m_noUpdates = 0;

   foreach(OutputAction *a, m_outputActions) {
      if (a != NULL)
         delete a;
   }

   m_outputActions.clear();
}

void		OutputConsole::undo_output ()
{
   if (!m_outputActions.isEmpty()) {
      OutputAction *a = m_outputActions.back();
      if (a == NULL)
         return;
      
      a->undo(this);
      m_outputActions.pop_back();
      delete a;
   }
}


// should always be called in Gui thread
void OutputConsole::updateBatch() {
   if (--m_noUpdates <= 0) {
      updateDisplay();
      m_noUpdates = 0;
      
      if (!m_visible)
         m_gui->ensureVisibility(this);
   }
}



NewOutputAction::NewOutputAction(OutputConsole *out, const QString &s)
   : m_string(s)
{
   out->push(s);
}

void NewOutputAction::undo(OutputConsole *output) {
   output->pop();
}

ClearOutputAction::ClearOutputAction(OutputConsole *out, const QVector<QString>& strings) 
   : m_strings(strings)
{
   out->flush();
}

void ClearOutputAction::undo(OutputConsole *output) {
   output->setStrings(m_strings);
}



OutputHelper::OutputHelper(OutputConsole *outputConsole) : QObject(outputConsole), 
   m_output(outputConsole)
{
   connect(m_output, SIGNAL(updateBatchSignal()), this, SLOT(updateBatch()));
}

// should always be called in Gui thread
void OutputHelper::updateBatch() {
//   cerr << "OutputHelper::updateBatch\n";
   m_output->updateBatch();
}


