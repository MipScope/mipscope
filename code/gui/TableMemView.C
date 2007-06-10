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
   file: TableMemView.C
   auth: Brian Moore
   acct: bjmoore
   date: 6/2/2007
\* ---------------------------------------------- */

#include <QtGui>
#include <iostream>
#include "TableMemView.H"
#include "table/TableView.H"
#include "Gui.H"
#include "Program.H"
#include "Identifier.H"
#include "ParseNode.H"
#include "Statement.H"
#include "State.H"

using namespace std;

TableMemView::TableMemView(Gui *gui) :
QDockWidget(tr("Table Memory View"), gui),
m_gui(gui),
m_table(NULL)
{
    QDockWidget::setObjectName(tr("Table Memory View"));
    
    m_table = new TableView(this);
    setWidget(m_table);
    
    connect(m_table, SIGNAL(gotoDeclaration(unsigned int)), m_gui, SLOT(gotoDeclaration(unsigned int)));
}


void TableMemView::updateDisplay(Program *program)
{
    // clear whatever had been added before
    m_table->clearData();
    
    // get all the labels in the program
    LabelMap *labelMap = program->getLabelMap();
    
    foreach(QString currKey, labelMap->keys())
    {
        AddressIdentifier *addr = (*labelMap)[currKey];
        
        ParseNode *node = addr->getParseNode();
        Statement *statement = node->getStatement();
        if (statement && statement->isDirective())
        {            
            // if it's a statement and a directive, it's a memory block
            m_table->addData(addr);
        }       
    }
    
    // make the view layout the data
    m_table->setupScene(program->getState());
}

QSize TableMemView::minimumSizeHint() const 
{
   return QSize(10, 10);
}

QSize TableMemView::sizeHint() const 
{
   return QSize(100, 200);
}


