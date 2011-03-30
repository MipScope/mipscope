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

#ifndef TEXTGUI_H_
#define TEXTGUI_H_

#include <QObject>
#include "UI.H"
#include "SyscallHandler.H"
#include "SPIMSyscalls.H"
#include "TextIOConsole.H"

class QStringList;
class Debugger;
class QApplication;

#define PROGRAM_TIMEOUT 30 // seconds

class TextGui : public QObject, public UI {
   
   Q_OBJECT
   
   public:
      TextGui(QStringList args);
      void executeProgram(QApplication* app);
		SyscallListener *getSyscallListener();
		void stopCurrentProgram();
      
   public slots:
      void syscallReceived(int no, int valueOfa0);
      void programTerminated(int reason);
		
      
   private:
      Debugger* m_debugger;
      SyscallListener m_syscallListener;
      TextIOConsole m_console;
      SPIMSyscalls m_spim_syscalls;
   
      void setupPlugins();
};

#endif /*TEXTGUI_H_*/
