/* MipScope <http://mipscope.sourceforge.net>
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
 */

/* ---------------------------------------------- *\
   file: Options.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/31/2007
\* ---------------------------------------------- */
#ifndef __OPTIONS_H_
#define __OPTIONS_H_

#include "Utilities.H"
#include <QObject>
#include <QFont>
#include <QString>

class Gui;
class EditorPane;
class QSettings;

class Options : public QObject
{
   Q_OBJECT

   public:
      Options(Gui *gui);
      virtual ~Options();
      
      static Options *m_options;
      static QSettings *m_settings; // stored by Qt, huzzah!

      void resetDefaults();
      void setupConnections(EditorPane *pane);

      // General
      static bool verbosity();
      static bool registerAliases();
      static int dislayBase();
      static bool showAllInstructions();
      static QString getTemplatePath();
      static QString getXSpimPath();
      
      bool m_verbosity;
      bool m_registerAliases;
      int m_displayBase;
      bool m_showAllInstructions;

      QString m_templatePath;
      QString m_xspimPath;
      
   signals:
      void verbosityChanged(bool);
      void registerAliasesChanged(bool);
      void displayBaseChanged(int);
      void showAllInstructionsChanged(bool);
      void templatePathChanged(const QString&);
      void xspimPathChanged(const QString&);

   public slots:
      void setVerbosity(bool);
      void setRegisterAliases(bool);
      void setDislayBase(unsigned int);
      void setShowAllInstructions(bool);

      void setTemplatePath(const QString&);
      void setXSpimPath(const QString&);


      // Editing
   public:
      static bool syntaxPopupsEnabled();
      static bool errorHighlightingEnabled();
      static bool autoIndentationEnabled();
      static unsigned int tabWidth();
      static QFont getFont();
      
      bool m_syntaxPopupsEnabled;
      bool m_errorHighlightingEnabled;
      bool m_autoIndentationEnabled;
      unsigned int m_tabWidth;
      QFont m_font;
      
   signals:
      void syntaxPopupsEnabledChanged(bool);
      void errorHighlightingEnabledChanged(bool);
      void autoIndentationEnabledChanged(bool);
      void tabWidthChanged(unsigned int);
      void fontChanged(const QFont&);

   public slots:
      void setSyntaxPopupsEnabled(bool);
      void setErrorHighlightingEnabled(bool);
      void setAutoIndentationEnabled(bool);
      void setTabWidth(unsigned int);
      void setFont(const QFont&);
		
		 // Auto-save
   public:
		
		enum AutoSaveOptions {PROMPT, ALWAYS, NEVER};
		
      static AutoSaveOptions autoSaveOptions();
            
      AutoSaveOptions m_autoSaveOptions;
      
   signals:
      void autoSaveOptionsChanged(AutoSaveOptions);

   public slots:
      void setAutoSaveOptions(AutoSaveOptions);
		
       
       // Debugging
   public:
      static bool readOnlyDebuggingEnabled();
      static bool implicitRollbackEnabled();
      static unsigned int noPreviousXInstructions();
      
      bool m_readOnlyDebuggingEnabled;
      bool m_implicitRollbackEnabled;
      unsigned int m_noPreviousXInstructions;
      
   signals:
      void readOnlyDebuggingEnabledChanged(bool);
      void implicitRollbackEnabledChanged(bool);
      void noPreviousXInstructionsChanged(unsigned int);

   public slots:
      void setReadOnlyDebuggingEnabled(bool);
      void setImplicitRollbackEnabled(bool);
      void setNoPreviousXInstructions(int);

   protected:
      Gui *m_gui;
};

#endif // __OPTIONS_H_

