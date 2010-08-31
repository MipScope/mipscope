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
   file: FindDialog.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/29/2007
\* ---------------------------------------------- */
#include "FindDialog.H"
#include "EditorPane.H"
#include "Utilities.H"
#include <QtGui>

FindDialog::FindDialog(EditorPane *parent, bool replace, bool hasSelection)
   : QDialog(parent), m_parent(parent), m_searching(false), m_modifiable(true)
{
   QStyle *style = new QCleanlooksStyle();
   setFont(QFont("arial", 10));
   const QFont font("arial", 9);
   
   // assure EditorPane is notified upon invocation of find/replace
//   connect(this, SIGNAL(accepted()), m_parent, SLOT(findDialogInvoked()));
   
   setWindowTitle(tr("Find"));
   QGridLayout *mainLayout = new QGridLayout();
   QVBoxLayout *layout = new QVBoxLayout();
   
   // Find section
   QGroupBox *box = new QGroupBox(tr("Find"), this);
   box->setStyle(style);
   QLabel *label = new QLabel(tr("Text to find:"), this);
   m_lineEdit = new QLineEdit(this);
   connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(accept()));
   
   m_regularExpression = new QCheckBox(tr("Regular e&xpression"), this);
   m_regularExpression->setStyle(style);
   m_regularExpression->setFont(font);
   
   layout->addWidget(label);
   layout->addWidget(m_lineEdit);
   layout->addWidget(m_regularExpression);
   layout->addStretch(1);
   box->setLayout(layout);
   
   mainLayout->addWidget(box, 0, 0, 1, 3);
   
   // Replace section
   m_replaceWidget = new ReplaceWidget(this, style);
   m_replaceWidget->setChecked(replace);
   connect(m_replaceWidget, SIGNAL(toggled(bool)), this, SLOT(updateReplace(bool)));
   mainLayout->addWidget(m_replaceWidget, 1, 0, 1, 3);
   
   // Options section
   QGridLayout *l = new QGridLayout();
   box = new QGroupBox(tr("Options"), this);
   box->setStyle(style);
   QCheckBox *b = new QCheckBox(tr("C&ase sensitive"), this);
   m_caseSensitive = b;
   b->setStyle(style);
   b->setFont(font);
   l->addWidget(b, 0, 0);
   b = new QCheckBox(tr("&Whole words only"), this);
   m_wholeWordsOnly = b;
   b->setStyle(style);
   b->setFont(font);
   l->addWidget(b, 1, 0);
   l->setColumnMinimumWidth(0, b->sizeHint().width() + 10); // hack cause this layout manager apparently has trouble w/ multiple styles
   b = new QCheckBox(tr("From c&ursor"), this);
   m_fromCursor = b;
   b->setStyle(style);
   b->setFont(font);
   l->addWidget(b, 2, 0);
   b = new QCheckBox(tr("Find &backwards"), this);
   m_findBackwards = b;
   b->setStyle(style);
   b->setFont(font);
   l->addWidget(b, 0, 1);
   b = new QCheckBox(tr("&Search selection"), this);
   m_searchSelection = b;
   b->setStyle(style);
   b->setFont(font);
   b->setEnabled(hasSelection);
   l->setColumnMinimumWidth(1, b->sizeHint().width() + 10); // hack cause this layout manager apparently has trouble w/ multiple styles
   connect(m_parent, SIGNAL(copyAvailabile(bool)), b, SLOT(setEnabled(bool)));
   
   l->addWidget(b, 1, 1);
/*   b = new QCheckBox(tr("&Prompt on replace"), this);
   m_promptOnReplace = b;
   b->setStyle(style);
   b->setEnabled(replace);
   connect(m_replaceWidget, SIGNAL(toggled(bool)), b, SLOT(setEnabled(bool)));
   l->addWidget(b, 2, 1);*/
   
   box->setLayout(l);
   mainLayout->addWidget(box, 2, 0, 1, 3);
   
   // Bottom buttons (invoke, cancel, replaceAll)
   m_invoked = new QPushButton((replace ? "&Replace" : "&Find"));
   m_invoked->setAutoDefault(true);
   m_invoked->setStyle(style);
   connect(m_invoked, SIGNAL(clicked()), this, SLOT(accept()));
   mainLayout->addWidget(m_invoked, 3, 0);//, Qt::AlignRight);
   
   m_cancel = new QPushButton("&Cancel");
   m_cancel->setAutoDefault(false);
   m_cancel->setStyle(style);
   connect(m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
   mainLayout->addWidget(m_cancel, 3, 2);//, Qt::AlignRight);
   
   m_replaceAll = new QPushButton("Replace &All");
   m_replaceAll->setAutoDefault(false);
   m_replaceAll->setStyle(style);
   connect(m_replaceAll, SIGNAL(clicked()), this, SLOT(replaceAll()));
   mainLayout->addWidget(m_replaceAll, 3, 1);
 
   mainLayout->setSizeConstraint(QLayout::SetFixedSize);

   updateReplace(replace);
   setLayout(mainLayout);
   setModal(false);
}

void FindDialog::accept() {
   TextEditor *current = m_parent->m_activeEditor;
   QTextDocument *doc  = current->getTextDocument();
   QTextCursor start   = current->textCursor();
   QTextCursor result;
   int startPos = -1, endPos = -1;
   
   // see if we're supposed to search within a selection
   if (start.hasSelection() && searchSelection()) {
      startPos = start.selectionStart();
      endPos = start.selectionEnd();
      
      // get rid of selection
      if (findBackwards())
         start.setPosition(endPos);
      else start.setPosition(startPos);
   } else if (!fromCursor() && !m_searching)
      start.movePosition(QTextCursor::Start);
   
   if (isFind()) {
      result = find(start, doc);
      
      // test if result was outside of selection
      if (!result.isNull() && 
          ((endPos > 0 && result.selectionEnd() > endPos) || 
          (startPos > 0 && result.selectionStart() < startPos)))
         result = QTextCursor(); // null cursor
   } else result = replace(start, doc, endPos);
   
   if (result.isNull()) {
      STATUS_BAR->showMessage(QString("%1 '%2' not found with the Options given").arg((regularExpression() ? QString("RegExp") : QString("String")), getSearchString()));
      
      m_searching = false;
      updateReplace(isReplace());
   } else {
      current->changeTextCursor(result);
      STATUS_BAR->clearMessage();
      
      m_searching = true;
      updateReplace(isReplace());
   }
}

QTextCursor FindDialog::find(const QTextCursor &start, const QTextDocument *doc) {
   if (start.isNull())
      return start;

   const QString &searchString = getSearchString();
   
   QTextDocument::FindFlags findFlags = 0;
   if (findBackwards())
      findFlags |= QTextDocument::FindBackward;
   if (caseSensitive())
      findFlags |= QTextDocument::FindCaseSensitively;
   if (wholeWordsOnly())
      findFlags |= QTextDocument::FindWholeWords;
   
   if (regularExpression())
      return doc->find(QRegExp(searchString), start, findFlags);

   // plain-text search
   return doc->find(searchString, start, findFlags);
}

QTextCursor FindDialog::replace(const QTextCursor &start, QTextDocument *doc, int startPos, int endPos) {
   QTextCursor result = find(start, doc);

   if (!result.isNull()) {
      if ((endPos > 0 && result.selectionEnd() > endPos) || 
          (startPos > 0 && result.selectionStart() < startPos)) {
         result = QTextCursor(); // null cursor
      } else {
         result.removeSelectedText();
         result.insertText(getReplaceString());
         //result.setPosition(result.position() + getReplaceString().length());
      }
   }
   
   return result;
}

void FindDialog::replaceAll() {
   TextEditor *current = m_parent->m_activeEditor;
   QTextDocument *doc  = current->getTextDocument();
   QTextCursor start   = current->textCursor();
   QTextCursor result, last;
   int noOccurrences = 0, startPos = -1, endPos = -1;
   m_searching = false;
   
   // search within a selection
   if (start.hasSelection() && searchSelection()) {
      startPos = start.selectionStart();
      endPos = start.selectionEnd();
      
      // get rid of selection
      if (findBackwards())
         start.setPosition(endPos);
      else start.setPosition(startPos);
   } else start.movePosition(QTextCursor::Start);
   
   start.beginEditBlock();
   result = start;
   
   do {
      last   = result;
      result = replace(result, doc, startPos, endPos);
      if (result.isNull())
         break;
      
      // disallow infinite replacements of similar terms ('life' w/ 'LIFE')
      result.setPosition(result.selectionEnd());
      ++noOccurrences;
   } while(true);
   
   start.endEditBlock();
   if (noOccurrences <= 0) {
      STATUS_BAR->showMessage(QString("%1 '%2' not found with the Options given").arg((regularExpression() ? QString("RegExp") : QString("String")), getSearchString()));
   } else {
      current->changeTextCursor(last);
      STATUS_BAR->showMessage(QString("Replaced %1 occurence%2.").arg(QString::number(noOccurrences), (noOccurrences != 1 ? QString("s") : QString(""))));
   }
   
   updateReplace(true);
}

void FindDialog::show(bool replace, const QTextCursor &cursor) {
   if (!isVisible())
      m_searching = false;
   
   bool hasSelection  = cursor.hasSelection();
   if (hasSelection && !cursor.isNull())
      m_lineEdit->setText(cursor.selectedText());
   
   updateReplace(replace && m_modifiable);
   m_searchSelection->setEnabled(hasSelection);
   QWidget::show();
}

void FindDialog::updateReplace(bool enabled) {
   QString invokeText;

   if (enabled)
      invokeText = "&Replace";
   else invokeText = "&Find";

   if (m_searching)
      invokeText += " next";
   
   //cerr << invokeText.toStdString() << endl;
   m_invoked->setText(invokeText);

   m_replaceAll->setVisible(enabled);
   m_replaceWidget->setContentVisible(enabled);
   if (m_replaceWidget->isChecked() != enabled)
      m_replaceWidget->setChecked(enabled);
}

// when active TextEditor is read-only, don't display option for replace
void FindDialog::setModifiable(bool isModifiable) {
   m_modifiable = isModifiable;

   m_replaceWidget->setEnabled(isModifiable);
   updateReplace(false);
}

bool FindDialog::isReplace() const {
   return (m_replaceWidget->isChecked());
}

bool FindDialog::isFind() const {
   return !isReplace();
}

// Accessors for the the QLineEdits (find and replace)
QString FindDialog::getSearchString() const {
   return m_lineEdit->text();
}

QString FindDialog::getReplaceString() const {
   return m_replaceWidget->getReplaceString();
}

// Accessors for the user-defined properties of this FindDialog
bool FindDialog::regularExpression() const {
   return m_regularExpression->isChecked();
}

bool FindDialog::caseSensitive() const {
   return m_caseSensitive->isChecked();
}

bool FindDialog::wholeWordsOnly() const {
   return m_wholeWordsOnly->isChecked();
}

bool FindDialog::fromCursor() const {
   return m_fromCursor->isChecked();
}

bool FindDialog::findBackwards() const {
   return m_findBackwards->isChecked();
}

bool FindDialog::searchSelection() const {
   return m_searchSelection->isChecked();
}

/*bool FindDialog::promptOnReplace() const {
   return m_promptOnReplace->isChecked();
}*/

ReplaceWidget::ReplaceWidget(FindDialog *parent, QStyle *style) 
   : QGroupBox("Replace", parent), m_parent(parent)
{
   QVBoxLayout *layout = new QVBoxLayout();
   
   m_label = new QLabel(tr("Replace with:"), m_parent);
   m_lineEdit = new QLineEdit(m_parent);
   connect(m_lineEdit, SIGNAL(returnPressed()), m_parent, SLOT(accept()));
   
   layout->addWidget(m_label);
   layout->addWidget(m_lineEdit);
   layout->addStretch(1);
   
   setLayout(layout);
   setCheckable(true);
   setStyle(style);
}

void ReplaceWidget::setContentVisible(bool visible) {
   setUpdatesEnabled(false);
   m_label->setVisible(visible);
   m_lineEdit->setVisible(visible);
   //setFlat(!visible);
   setUpdatesEnabled(true);
}

QString ReplaceWidget::getReplaceString() const {
   return m_lineEdit->text();
}

