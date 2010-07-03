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
   file: FindDialog.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/29/2007
\* ---------------------------------------------- */
#ifndef __FINDDIALOG_H__
#define __FINDDIALOG_H__

#include <QDialog>
#include <QGroupBox>
#include <QTextCursor>

class ReplaceWidget;
class EditorPane;
class QLabel;
class QStyle;
class QCheckBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QTextCursor;
class QString;

class FindDialog : public QDialog
{
   Q_OBJECT

   public:
      FindDialog(EditorPane *parent = NULL, bool replace = false, bool hasSelection = false);

      // Returns true if this FindDialog is in replace mode
      bool isReplace() const;

      // Returns true if this FindDialog is in find mode
      bool isFind() const;
      
      // Accessors for the the QLineEdits (find and replace)
      QString getSearchString() const;
      QString getReplaceString() const;
      
      // Accessors for the user-defined properties of this FindDialog
      bool regularExpression() const;
      bool caseSensitive() const;
      bool wholeWordsOnly() const;
      bool fromCursor() const;
      bool findBackwards() const;
      bool searchSelection() const;
      //bool promptOnReplace() const;
      
   public slots:
      // @overriden from QDialog
      virtual void accept();
      
      // main functionality
      QTextCursor find(const QTextCursor &start, const QTextDocument *doc);
      QTextCursor replace(const QTextCursor &start, QTextDocument *doc, int startPos = -1, int endPos = -1);
      void replaceAll();
      
      void show(bool replace, const QTextCursor&);
      void updateReplace(bool enabled);
      
      // when active TextEditor is read-only, don't display option for replace
      void setModifiable(bool);
      
   private:
      EditorPane *m_parent;
      ReplaceWidget *m_replaceWidget;
      QPushButton *m_invoked; // find or replace pressed
      QPushButton *m_cancel;
      QPushButton *m_replaceAll;
      QLineEdit *m_lineEdit;
      
      // true after one successful occurrence of find -> find next
      bool m_searching;
      bool m_modifiable;
      
      // Options
      QCheckBox *m_regularExpression, 
                *m_caseSensitive, 
                *m_wholeWordsOnly, 
                *m_fromCursor, 
                *m_findBackwards, 
                *m_searchSelection;
                //*m_promptOnReplace;
};

class ReplaceWidget : public QGroupBox {

   Q_OBJECT
   
   public:
      ReplaceWidget(FindDialog *parent, QStyle *style);
   
      void setContentVisible(bool visible);
      QString getReplaceString() const;

   protected:
      FindDialog *m_parent;
      QLabel *m_label;
      QLineEdit *m_lineEdit;
};

#endif // __FINDDIALOG_H__

