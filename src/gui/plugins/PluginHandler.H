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
   file: PluginHandler.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#ifndef __PLUGIN_HANDLER_H__
#define __PLUGIN_HANDLER_H__

#include <QList>

class UI;
class Gui;
class TextGui;
class Plugin;

/* PluginHandler:
 * ---------------
 * 
 *   Central class which initializes and registers 
 * all non-mipscope specific plugins.
 */
class PluginHandler {
   
   public:
		PluginHandler(Gui *parent);
		PluginHandler(TextGui *parent);
      void reset();
      
   protected:
      void initializePlugins();

      UI *m_ui;
		bool m_textOnly;
      QList<Plugin*> m_plugins;
};

class Plugin {
   
   public:
      Plugin(UI *ui);
      virtual ~Plugin();
      
      // avoid reset() name conflict with SyscallHandler class
      virtual void resetPlugin();

   protected:
      UI *m_ui;
};

#endif // __PLUGIN_HANDLER_H__

