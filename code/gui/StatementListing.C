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
   file: StatementListing.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */
#include "StatementListing.H"
#include "EditorPane.H"
#include "Gui.H"
#include <QtGui>

#include "../simulator/Statement.H"

const char *statementStrings[] = {
   "Arithmetic", "Comparison", "Control Flow", "Data Transfer", 
   "Special", "Directives", NULL
};

StatementListing::StatementListing(Gui *gui, EditorPane *editorPane)
   : QDockWidget(tr("MIPS Reference"), gui), m_gui(gui), 
   m_editorPane(editorPane)
{
   setObjectName(tr("MIPS Reference"));
   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
   
   m_tree = new TreeListing(this);
   m_tree->setContextMenuPolicy(Qt::NoContextMenu);

   m_contextMenu = new QMenu(this);
   m_insertAction = new QAction(tr("&Insert"), this);
   connect(m_insertAction, SIGNAL(triggered()), this, SLOT(insertAction()));
   m_contextMenu->addAction(m_insertAction);
   
   // Populate the listing of MIPS Statements
   updateListing();

   connect(m_tree, SIGNAL(doubleClicked(const QModelIndex&)), 
         this, SLOT(clicked(const QModelIndex&)));
   setWidget(m_tree);
}

QString StatementListing::fix(const char *str) {
   return QString(str).replace("&lt;", "<").replace("&gt;", ">");
}

void StatementListing::showAllInstructionsChanged(bool showAll) {
   updateListing();
}

void StatementListing::updateListing() {
//   cerr << "updateListing" << endl;

   // Clear any previous listings
   m_tree->clear();

   m_tree->setColumnCount(3);
   QVector<QTreeWidgetItem*> items(0);
      
   int i = 0;
   while(statementStrings[i] != NULL) {
      items.append(new QTreeWidgetItem(m_tree, QStringList(statementStrings[i++])));
   }
   
   // There's an Option to display only the most important & frequently used instructions
   int importanceThreshold = (Options::showAllInstructions() ? 0 : 51);

   QTreeWidgetItem *item;
   foreach(Instruction *instr, instructionMap.values()) {
      if (instr != NULL && instr->importance() >= importanceThreshold) {
         int type = instr->getType();

         item = new QTreeWidgetItem(items[type], QStringList(instr->getName()) << fix(instr->getSyntax()) << fix(instr->getDescription()));
         const QString &toolTip = QString("<b>%1</b> %2<br><br>%3").arg(instr->getName(), instr->getSyntax(), instr->getDescription());

         item->setToolTip(0, toolTip);
         item->setToolTip(1, toolTip);
         item->setToolTip(2, toolTip);

         items[type]->addChild(item);
      }
   }

   foreach(Directive *dir, directiveMap.values()) {
      if (dir->importance() < importanceThreshold)
         continue;

      item = new QTreeWidgetItem(items[DIRECTIVE], QStringList(dir->getName()) << fix(dir->getSyntax()) << fix(dir->getDescription()));

      const QString &toolTip = QString("<b>%1</b> %2<br><br>%3").arg(dir->getName(), dir->getSyntax(), dir->getDescription());

      item->setToolTip(0, toolTip);
      item->setToolTip(1, toolTip);
      item->setToolTip(2, toolTip);

      items[DIRECTIVE]->addChild(item);
   }

   i = 0;

   foreach(QTreeWidgetItem *cur, items) {
      QFont font = cur->font(0);
      font.setBold(true);
      cur->setFont(0, font);
      cur->sortChildren(0, Qt::AscendingOrder);
      m_tree->insertTopLevelItem(i++, cur);
   }

   m_tree->setAlternatingRowColors(true);
//   m_tree->header()->hide(); // don't show header titles
   m_tree->setHeaderLabels(QStringList() << "Name" << "Syntax" << "Description");
}

bool StatementListing::isHeading(const QModelIndex &index) {
   QTreeWidgetItem *item = m_tree->getItem(index);
   const QString text = item->text(0);
   bool statement = true;

   int i = 0;
   while(statementStrings[i] != NULL) {
      if (text == QString(statementStrings[i++])) {
         statement = false;
         break;
      }
   }
   
   return (!statement); // heading
}

void StatementListing::clicked(const QModelIndex &index) {
   QTreeWidgetItem *item = m_tree->getItem(index);
   const QString text = item->text(0);

   if (!isHeading(index))
      m_editorPane->m_activeEditor->textCursor().insertText(item->text(0));
   
   //         QStandardItem *item = m_dirModel->itemFromIndex(index);

   //         QColor c(157, 187, 227);
   //         item->setForeground(QBrush(c));
}

void StatementListing::contextMenuRequested(const QPoint &pos) {
   // Hack to get QTreeWidget to do the correct mapping;  this is a 
   // bug in Qt 4.3 rc1 with the titlebarwidget of the QDockWidget not 
   // being factored into the coordinate mappings... ugh
   int y = pos.y() - (geometry().height() - m_tree->geometry().height());
   
   QPoint pos2(pos.x(), y);
   m_menuIndex = m_tree->getIndex(m_tree->itemAt(m_tree->mapFromParent(pos2)));
   
   if (m_menuIndex == QModelIndex() || isHeading(m_menuIndex))
      return;
   
   m_insertAction->setText(QString("insert '%1'").arg(m_tree->getItem(m_menuIndex)->text(0)));
   m_contextMenu->popup(mapToGlobal(pos));
}

void StatementListing::insertAction() {
   clicked(m_menuIndex);
}

TreeListing::TreeListing(StatementListing *listing) : QTreeWidget(listing) {
   m_listing = listing;
}

QTreeWidgetItem *TreeListing::getItem(const QModelIndex &index) {
   return itemFromIndex(index);
}

QModelIndex TreeListing::getIndex(QTreeWidgetItem *item) {
   return indexFromItem(item);
}


