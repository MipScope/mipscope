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
   file: OptionsDialog.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */
#include "OptionsDialog.H"
#include "Gui.H"
#include "EditorPane.H"
#include "Utilities.H"
#include <QtGui>
#include "OptionsPages.H"

OptionsDialog::OptionsDialog(Gui *gui, EditorPane *editorPane)
   : QDialog(gui), m_gui(gui), m_editorPane(editorPane)
{
   m_contents = new QListWidget;
   m_contents->setViewMode(QListView::IconMode);
   m_contents->setIconSize(QSize(64, 64));
   m_contents->setMovement(QListView::Static);
   m_contents->setMaximumWidth(96);
   m_contents->setSpacing(12);

   m_pages = new QStackedWidget;
   m_pages->addWidget(m_generalOptions = new GeneralOptionsPage(this));
   m_pages->addWidget(m_editingOptions = new EditingOptionsPage(this));
   m_pages->addWidget(m_debuggingOptions = new DebuggingOptionsPage(this));
   
   connect(m_editingOptions, SIGNAL(fontChanged(const QFont&)), m_editorPane, SLOT(currentFontChanged(const QFont&)));

   QPushButton *closeButton = new QPushButton(tr("Close"));
   
   createIcons();
   m_contents->setCurrentRow(0);

   connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

   QHBoxLayout *horizontalLayout = new QHBoxLayout;
   horizontalLayout->addWidget(m_contents);
   horizontalLayout->addWidget(m_pages, 1);

   QHBoxLayout *buttonsLayout = new QHBoxLayout;
   buttonsLayout->addStretch(1);
   buttonsLayout->addWidget(closeButton);

   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(horizontalLayout);
   mainLayout->addStretch(1);
   mainLayout->addSpacing(12);
   mainLayout->addLayout(buttonsLayout);
   setLayout(mainLayout);

   setWindowTitle(tr("Options"));
}

void OptionsDialog::createIcons()
{
   QListWidgetItem *configButton = new QListWidgetItem(m_contents);
   configButton->setIcon(QIcon(IMAGES"/optionsGeneral.png"));
   configButton->setText(tr("General"));
   configButton->setTextAlignment(Qt::AlignHCenter);
   configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

   QListWidgetItem *updateButton = new QListWidgetItem(m_contents);
   updateButton->setIcon(QIcon(IMAGES"/optionsEditing.png"));
   updateButton->setText(tr("Editing"));
   updateButton->setTextAlignment(Qt::AlignHCenter);
   updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

   QListWidgetItem *queryButton = new QListWidgetItem(m_contents);
   queryButton->setIcon(QIcon(IMAGES"/optionsDebugging.png"));
   queryButton->setText(tr("Debugging"));
   queryButton->setTextAlignment(Qt::AlignHCenter);
   queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

   connect(m_contents,
         SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
         this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void OptionsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
   if (!current)
      current = previous;

   m_pages->setCurrentIndex(m_contents->row(current));
}

