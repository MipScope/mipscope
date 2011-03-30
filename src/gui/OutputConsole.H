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
   file: OutputConsole.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __OUTPUT_CONSOLE_H__
#define __OUTPUT_CONSOLE_H__

#include <QObject>

#include "Console.H"
#include "IOConsole.H"

class Gui;
class EditorPane;
class OutputAction;
class OutputHelper;

class OutputConsole : public Console, public IOConsole {
public:
	friend class NewOutputAction;
	friend class ClearOutputAction;

	OutputConsole(Gui *parent, EditorPane *editorPane);
      
	void update();
	void updateBatch();
      
	// @overridden from Console
	void push(const QString &newOutput);
	void pop();

	// @overridden from IOConsole
	virtual std::string	prompt_for_string (State*, int, std::string prompt_message, size_t max_length);
	virtual int		prompt_for_int (State*, int, std::string prompt_message, int min, int max);
	virtual double		prompt_for_double (State*, int, std::string prompt_message, double min, double max);
	virtual void		output_string (State*, int, std::string message);
	virtual void		clear_output (State*, int);

	virtual void		reset ();
	virtual void		undo_output ();
	virtual bool		can_undo_output () const { return true; }


   
private:
	QVector<OutputAction*> m_outputActions;
	OutputHelper *m_outputHelper;
	int m_noUpdates; // pseudo-stack to only update once-per-batch

	void update_output ();
};

class OutputHelper : public QObject {
   
   Q_OBJECT
   
   public:
      OutputHelper(OutputConsole *outputConsole);
 
   public slots:
      void updateBatch();
      
   private:
      OutputConsole *m_output;
};

class OutputAction {
   public:
      virtual ~OutputAction() { }
      
      virtual void undo(OutputConsole *output) = 0;
};

class NewOutputAction : public OutputAction {
   public:
      NewOutputAction(OutputConsole *out, const QString &s);

      virtual void undo(OutputConsole *output);
      
   protected:
      QString m_string;
};

class ClearOutputAction : public OutputAction {
   public:
      ClearOutputAction(OutputConsole *out, const QVector<QString>& strings);

      virtual void undo(OutputConsole *output);

   protected:
      QVector<QString> m_strings;
};

#endif // __OUTPUT_CONSOLE_H__

