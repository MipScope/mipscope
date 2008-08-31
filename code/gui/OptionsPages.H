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
   file: OptionsPages.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */
#ifndef PAGES_H
#define PAGES_H

#include <QWidget>
#include <QFont>
#include "Options.H"

class QPushButton;
class QString;
/*class QFontComboBox;
class QComboBox;
class QString;
class QLabel;*/

class GeneralOptionsPage : public QWidget
{
   Q_OBJECT

   public:
      GeneralOptionsPage(QWidget *parent = 0);

   protected slots:
      void verbosityChanged(int);
      void registerAliasesChanged(int);
      void displayBaseChanged(int);
      void showAllInstructionsChanged(bool);
      void templatePathChanged(const QString&);
      void xspimPathChanged(const QString&);
};

class EditingOptionsPage : public QWidget
{
   Q_OBJECT

   public:
      EditingOptionsPage(QWidget *parent = 0);
   
   signals:
      void fontChanged(const QFont&);

   protected slots:
/*      void fontFamilyChanged(const QFont&);
      void sizeChanged(const QString&);
      void findSizes(const QFont&);*/

      void syntaxPopupsChanged(int);
      void errorHighlightingChanged(int);
      void autoIndentationChanged(int);
      void tabWidthChanged(int);
		void autoSaveToPrompt(bool doIfTrue);
		void autoSaveToAlways(bool doIfTrue);
		void autoSaveToNever(bool doIfTrue);
      void changeFont();

   protected:
      QFont m_font;
      QPushButton *m_fontButton;

/*      QFontComboBox *m_fontCombo;
      QComboBox *m_sizeCombo;
      QLabel *m_exampleText;*/
};

class DebuggingOptionsPage : public QWidget
{
   Q_OBJECT

   public:
      DebuggingOptionsPage(QWidget *parent = 0);

   protected slots:
      void readOnlyDebuggingChanged(int);
      void implicitRollbackChanged(int);
};

#endif

