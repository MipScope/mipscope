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
    file: TableView.H
    auth: Brian Moore
    acct: bjmoore
    date: 6/2/2007
 \* ---------------------------------------------- */

#ifndef _TABLEVIEW_H_
#define _TABLEVIEW_H_

#include <QFrame>
#include <QMap>
#include <QHash>
#include <QSignalMapper>

class QGraphicsScene;
class QGraphicsView;
class QSlider;
class QPointF;
class QPainterPath;
class QRectF;
class State;
class AddressIdentifier;
class TableMemView;
class QGraphicsSimpleTextItem;

/*
 * TableView class
 * - main widget of TableMemView dock widget
 * - contains widgets: slider and graphicsview
 * - the graphicsview displays the actual memory table view and labels
 */
class TableView : public QFrame
{
    Q_OBJECT
        
public:
    TableView(TableMemView *memView);
    
    void clearData();
    void addData(AddressIdentifier *chunk);
    
    void setupScene(State *programState=NULL);
    
    // utility class method
    static QPointF OriginForAddress(unsigned int addr);
    static QPainterPath PathForRange(unsigned int addr, unsigned int len);
    static QRectF RectForAddress(unsigned int addr);
    static unsigned int RowForAddress(unsigned int addr);
    static unsigned int ColumnForAddress(unsigned int addr);
    static QPointF LabelAnchorForRow(unsigned int row);
    
signals:
    void gotoDeclaration(unsigned int address);
    
private slots:
    void zoomView(int factor);
    void gotoBlockDeclaration(QObject *obj);
    
private:
    void addObjectsForIdentifier(AddressIdentifier *var, unsigned int& lastAddr, State *state);
    
    QMap<unsigned int, AddressIdentifier *> m_addresses;
    QHash<unsigned int, QGraphicsSimpleTextItem *> m_rowLabels;
    
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QSlider *m_slider;
    
    QSignalMapper m_gotoDeclMap;
};

#endif /* _TABLEVIEW_H_ */
