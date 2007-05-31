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
   file: OptionsDialog.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */

#ifndef __OPTIONSDIALOG_H__
#define __OPTIONSDIALOG_H__

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class Gui;
class EditorPane;
class GeneralOptionsPage;
class EditingOptionsPage;
class DebuggingOptionsPage;

class OptionsDialog : public QDialog
{
   Q_OBJECT

   public:
      OptionsDialog(Gui *gui, EditorPane *editorPane);

   public slots:
      void changePage(QListWidgetItem *current, QListWidgetItem *previous);

   private:
      void createIcons();

      Gui *m_gui;
      EditorPane *m_editorPane;
      
      GeneralOptionsPage *m_generalOptions;
      EditingOptionsPage *m_editingOptions;
      DebuggingOptionsPage *m_debuggingOptions;

      QListWidget *m_contents;
      QStackedWidget *m_pages;
};

#endif // __OPTIONSDIALOG_H__

