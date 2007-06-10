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
   file: TableView.C
   auth: Brian Moore
   acct: bjmoore
   date: 6/2/2007
\* ---------------------------------------------- */

#include <QtGui>

#include <iostream>
#include "TableView.H"
#include "TableMemView.H"
#include "Identifier.H"
#include "ParseNode.H"
#include "Statement.H"
#include "State.H"
#include "MemBlock.H"
#include <assert.h>
#include <cmath>

#define CELL_HEIGHT (25*0.8)
#define CELL_WIDTH (55*0.8)
#define LABEL_MARGIN (10)
#define COL_SEPARATION (5)
#define TABLE_WIDTH (2)

TableView::TableView(TableMemView *memView) :
QFrame(memView),
m_scene(NULL),
m_view(NULL),
m_slider(NULL)
{
    m_view = new QGraphicsView(NULL, this);
    m_slider = new QSlider(this);
    m_slider->setMinimum(100);
    m_slider->setMaximum(300);
    
    m_view->setInteractive(true);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setRenderHint(QPainter::TextAntialiasing, true);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(zoomView(int)));
    connect(&m_gotoDeclMap, SIGNAL(mapped(QObject*)), this, SLOT(gotoBlockDeclaration(QObject*)));
    
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(m_view);
    hbox->addWidget(m_slider);
    
    clearData();
    setupScene();
}

void TableView::zoomView(int factor)
{
    qreal scale = ::pow(2.0, (factor - 250) / 50.0);
    
    QMatrix newMatrix;
    newMatrix.scale(scale, scale);
    
    m_view->setMatrix(newMatrix);
}

void TableView::gotoBlockDeclaration(QObject *obj)
{
    MemBlock *block = dynamic_cast<MemBlock*>(obj);
    
    emit gotoDeclaration(block->getStartAddress());
}

void TableView::clearData()
{
    m_addresses.clear();
}

void TableView::addData(AddressIdentifier *var)
{
    m_addresses[var->getValue()] = var;
}

unsigned int TableView::ColumnForAddress(unsigned int addr)
{
    return (addr - DATA_BASE_ADDRESS)%(4*TABLE_WIDTH);
}

unsigned int TableView::RowForAddress(unsigned int addr)
{
    return (addr - DATA_BASE_ADDRESS)/(4*TABLE_WIDTH);
}

QPointF TableView::LabelAnchorForRow(unsigned int row)
{
    return QPointF(-LABEL_MARGIN, (row+0.5)*CELL_HEIGHT);
}

QPointF TableView::OriginForAddress(unsigned int addr)
{
    unsigned int row;
    unsigned int col;
    
    // calculate row and column in the memory table
    row = RowForAddress(addr);
    col = ColumnForAddress(addr);
    
    return QPointF(col*CELL_WIDTH + (col/4)*COL_SEPARATION, row*CELL_HEIGHT);
}

QRectF TableView::RectForAddress(unsigned int addr)
{
    return QRectF(OriginForAddress(addr), QSizeF(CELL_WIDTH, CELL_HEIGHT));
}

QPainterPath TableView::PathForRange(unsigned int addr, unsigned int len)
{
    QPainterPath path;
    
    // special case: range takes only one row
    if ((addr & ~3) == ((addr+len-1) & ~3))
    {
        QPointF orig = OriginForAddress(addr);
        
        path.addRect(orig.x(), orig.y(), CELL_WIDTH*len, CELL_HEIGHT);
    }
    else // otherwise we can split it up into a general case
    {
        unsigned int middleTopLeftAddr = (addr & ~3) + 4;
        unsigned int topRtAddr = middleTopLeftAddr - 1;
        unsigned int bottomRightAddr = addr+len-1;
        unsigned int bottomLeftAddr = bottomRightAddr & ~3;
        unsigned int middleBottomRightAddr = bottomLeftAddr - 1;
        
        // make constituent rectangles
        QRectF topRect = QRectF(RectForAddress(addr).topLeft(), RectForAddress(topRtAddr).bottomRight());
        QRectF middleRect = QRectF(RectForAddress(middleTopLeftAddr).topLeft(), RectForAddress(middleBottomRightAddr).bottomRight());
        QRectF bottomRect = QRectF(RectForAddress(bottomLeftAddr).topLeft(), RectForAddress(bottomRightAddr).bottomRight());
        
        // draw around the boxes
        path.moveTo(topRect.topLeft());
        path.lineTo(topRect.topRight());
        path.lineTo(middleRect.bottomRight());
        path.lineTo(bottomRect.topRight());
        path.lineTo(bottomRect.bottomRight());
        path.lineTo(bottomRect.bottomLeft());
        path.lineTo(middleRect.topLeft());
        path.lineTo(topRect.bottomLeft());
        path.closeSubpath();
    }
    
    return path;
}

void TableView::addObjectsForIdentifier(AddressIdentifier *var, unsigned int& endOfLast, State *state)
{
    ParseNode *parseNode;
    Statement *statement;
    unsigned int identifierStart;
    unsigned int identifierSize;
    QString identifierName;
    QString identifierType;
    
    parseNode = var->getParseNode();
    assert(parseNode != NULL);
        
    statement = parseNode->getStatement();
    assert(statement != NULL);
          
    identifierStart = var->getValue();
    identifierSize = parseNode->getSize();
    identifierName = var->getID();
    identifierType = QString(statement->getName());
    
    // set startAddr if this is the first time around
    if (endOfLast == 0)
    {
        // we're just starting, just set endOfLast to the current address
        endOfLast = DATA_BASE_ADDRESS;
    }
    
    // cout << "Identifier Info:" << endl;
    //     cout << "    Name: " << identifierName.toStdString() << endl;
    //     cout << "    Size:" << identifierSize << endl;
    //     cout << "    Address: " << identifierStart << endl;
    //     cout << "    Identifier Type: " << identifierType.toStdString() << endl;
    //     cout << "    isDirective: " << statement->isDirective() << endl;
    //     
    //     cout << "End of last = " << endOfLast << endl;
    
    // if (identifierStart - endOfLast > 0)
    //     {
    //         //cout << "**** Skipped " << identifierStart - endOfLast << " bytes." << endl;
    //         // UnusedSpanItem *unusedSpan;
    //         //         
    //         //         unusedSpan = new UnusedSpanItem(endOfLast, identifierStart - endOfLast);
    //         //         m_scene->addItem(unusedSpan);
    //     }
    //     else
    //     {
    //         if (ColumnForAddress(endOfLast) > 0)
    //         {
    //             BlockSeparatorItem *separator;
    //             
    //             separator = new BlockSeparatorItem(identifierStart);
    //             
    //             m_scene->addItem(separator);
    //         }
    //     }
    
    // make label for this identifier
    QGraphicsSimpleTextItem *labelText;
    QPointF labelAnchor;
    QRectF labelBoundBox;
    
    // if there's already a label on that row, append the current identifier onto the string
    if (m_rowLabels.contains(RowForAddress(identifierStart)))
    {
        labelText = m_rowLabels[RowForAddress(identifierStart)];
        labelText->setText(QString("%1, %2:").arg(labelText->text()).arg(identifierName));
    }
    else // otherwise make new label
    {
        labelText = m_scene->addSimpleText(QString("%1:").arg(identifierName));
        m_rowLabels[RowForAddress(identifierStart)] = labelText;
    }
    
    // position the label properly
    labelAnchor = LabelAnchorForRow(RowForAddress(identifierStart));
    labelBoundBox = labelText->boundingRect();
    
    labelText->setPos(labelAnchor.x() - labelBoundBox.width(), labelAnchor.y() - labelBoundBox.height()/2.0);
    
    QPointF currOrigin;
    MemBlock *currBlock;
    
    // allocate the right kind of block for the current identifier
    if (identifierType == ".asciiz")
    {
        currBlock = new ByteBlock(identifierStart, identifierSize, ByteItem::AsciiDisplay, identifierName);
    }
    else if (identifierType == ".byte")
    {
        currBlock = new ByteBlock(identifierStart, identifierSize, ByteItem::NumberDisplay, identifierName);
    }
    else if (identifierType == ".word")
    {
        currBlock = new WordBlock(identifierStart, identifierSize/4, identifierName);
    }
    else
    {
        currBlock = new ByteBlock(identifierStart, identifierSize, ByteItem::NumberDisplay, identifierName);
    }
    
    // if there's a state, give its values to the block
    if (state)
    {
        currBlock->takeValues(*state);
    }
    
    // connect blocks to the mapper for gotoDeclaration signals
    m_gotoDeclMap.setMapping(currBlock, currBlock);
    connect(currBlock, SIGNAL(gotoDeclaration()), &m_gotoDeclMap, SLOT(map()));
    m_scene->addItem(currBlock);
    
    // update lastAddr with address at the end of var
    endOfLast = identifierStart + identifierSize;
}

void TableView::setupScene(State *programState)
{
    QGraphicsScene *oldScene = m_scene;
    
    m_scene = new QGraphicsScene(this);
    m_rowLabels.clear();
    
    unsigned int currAddr = 0;
    
    foreach(AddressIdentifier *addrId, m_addresses)
    {
        addObjectsForIdentifier(addrId, currAddr, programState);
    }
    
    m_view->setScene(m_scene);
    
    if (oldScene)
    {
        delete oldScene;
    }
}
