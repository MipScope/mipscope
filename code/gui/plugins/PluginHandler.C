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
   file: PluginHandler.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#include "PluginHandler.H"
#include "../Utilities.H"
#include "../UI.H"
#include "../Gui.H"
#include "../TextGui.H"
#include "../../simulator/State.H"


// ADD your plugins HERE:
#include "maze/MazePlugin.H"

// Handles all extensions to the program.
// All non-mipscope specific functionality
// should be initialized here and the 
// code should be located in a subdirectory.
// -----------------------------------------

PluginHandler::PluginHandler(Gui *ui) : m_ui(ui), m_textOnly(false)
{
   initializePlugins();
}

PluginHandler::PluginHandler(TextGui *ui) : m_ui(ui), m_textOnly(true)
{
	 initializePlugins();
}


void PluginHandler::initializePlugins() {
   m_plugins.clear();
   
   // Instantiate/initialize your plugins HERE:
   m_plugins.append(new MazePlugin(m_ui, m_textOnly));
}

void PluginHandler::reset() {
   foreach(Plugin *p, m_plugins)
      p->resetPlugin();
}


Plugin::Plugin(UI *ui) : m_ui(ui)
{ }

Plugin::~Plugin()
{ }

void Plugin::resetPlugin() { }

