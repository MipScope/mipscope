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
   file: TableMemView.H
   auth: Brian Moore
   acct: bjmoore
   date: 6/2/2007
\* ---------------------------------------------- */

#ifndef _TABLEMEMVIEW_H_
#define _TABLEMEMVIEW_H_

#include <QDockWidget>

class Gui;
class Program;
class TableView;

class TableMemView : public QDockWidget
{
    Q_OBJECT
    
public:
    TableMemView(Gui *gui);
    
    void updateDisplay(Program *program);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
private:
    Gui *m_gui;
    TableView *m_table;
};


#endif /* _TABLEMEMVIEW_H_ */

