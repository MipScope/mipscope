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
   file: MemBlock.C
   auth: Brian Moore
   acct: bjmoore
   date: 6/2/2007
\* ---------------------------------------------- */

#include <QtGui>
#include <iostream>
#include "State.H"
#include "MemBlock.H"
#include "TableView.H"
#include <cmath>

#define SEPARATOR_WIDTH (1.0)
#define CORNER_ROUNDNESS (50.0)

MemItem::MemItem(unsigned int addr, unsigned char bytesWide, QString typeName, MemBlock *parent) :
QGraphicsItem(parent),
m_block(parent),
m_address(addr),
m_bytesWide(bytesWide),
m_typeName(typeName),
m_display(NumberDisplay)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(0);
    
    QString toolTip = m_typeName;
    
    toolTip += QString("<br>Address: 0x%1").arg((uint)addr, 8, 16, (QChar)'0');
    
    if (m_block->getLength() > 1)
    {
        toolTip += QString("<br>Part of: %1").arg(m_block->getLabelName());
        toolTip += QString("<br>Index: %1").arg((m_address - m_block->getStartAddress())/bytesWide);
    }
    else if (m_block->getLength() == 1)
    {
        toolTip += QString("<br>Label: %1").arg(m_block->getLabelName());
    }
    
    setToolTip(toolTip);
    
    m_rect = TableView::RectForAddress(m_address);
    m_rect.setWidth(m_rect.width()*m_bytesWide);
}

unsigned int MemItem::getAddress()
{
    return m_address;
}

QRectF MemItem::boundingRect() const
{
    return m_rect;
}

MemItem::DisplayType MemItem::getDisplayType()
{
    return m_display;
}

void MemItem::setDisplayType(DisplayType type)
{
    m_display = type;
}

MemItem::AppearanceFlags MemItem::appearanceFlags()
{
    return m_flags;
}

void MemItem::setAppearanceFlags(AppearanceFlags newFlags)
{
    m_flags = newFlags;
}

void MemItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    
    QAction *gotoAction = menu.addAction("Goto .data declaration");
    connect(gotoAction, SIGNAL(triggered()), m_block, SIGNAL(gotoDeclaration()));
    
    menu.exec(event->screenPos());
}

void MemItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(widget);
    
    QBrush brush;
    
    if (option->state & QStyle::State_Selected)
    {
        brush = option->palette.highlight();
    }
    else
    {
        brush = QColor("white");
    }
    
    painter->setBrush(brush);
    
    QRectF left(m_rect.x(), m_rect.y(), m_rect.width()/2, m_rect.height());
    QRectF right(left.x()+left.width(), left.y(), left.width(), left.height());
    
    // expand clipping rect to allow for some antialiasing
    left.adjust(-1, -1, 0, 1);
    right.adjust(0, -1, 1, 1);
    
    if ((m_flags & FirstItemFlag) && (m_flags & LastItemFlag))
    {
        // draw a full rounded rect
        painter->drawRoundRect(m_rect.adjusted(SEPARATOR_WIDTH, 0, 0, 0), 
                               CORNER_ROUNDNESS*(m_rect.height()/m_rect.width()), CORNER_ROUNDNESS);
        
    }
    else if (m_flags & FirstItemFlag) // just left rounded
    {
        painter->setClipping(true);
        
        painter->setClipRect(left);
        painter->drawRoundRect(m_rect.adjusted(SEPARATOR_WIDTH, 0, 0, 0), 
                               CORNER_ROUNDNESS*(m_rect.height()/m_rect.width()), CORNER_ROUNDNESS);
        
        painter->setClipRect(right);
        painter->drawRect(m_rect);
        
        painter->setClipping(false);
    }
    else if (m_flags & LastItemFlag) // just right rounded
    {
        painter->setClipping(true);
        
        painter->setClipRect(right);
        painter->drawRoundRect(m_rect.adjusted(0, 0, -SEPARATOR_WIDTH, 0),
                               CORNER_ROUNDNESS*(m_rect.height()/m_rect.width()), CORNER_ROUNDNESS);
        
        painter->setClipRect(left);
        painter->drawRect(m_rect);
                
        painter->setClipping(false);
    }
    else // just draw it normally
    {
        painter->drawRect(m_rect);
    }
    
    painter->drawText(m_rect, stringValue(), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

ByteItem::ByteItem(unsigned int addr, ByteBlock *parent) :
MemItem(addr, 1, "Byte", parent),
m_val(0)
{    
}

unsigned char ByteItem::value()
{
    return m_val;
}

void ByteItem::setValue(unsigned char val)
{
    m_val = val;
}

void ByteItem::takeValue(State& state)
{
    setValue(state.getMemoryByteUnobserved(getAddress()));
}

QString ByteItem::stringValue()
{
    QString displayString;
    
    if (m_display == NumberDisplay)
    {
        displayString = QString("%1").arg((uint)m_val, 2, 16, (QChar)'0');
    }
    else // ascii display
    {
        if (m_val >= '!' && m_val <= '~')
        {
            displayString = QString("%1").arg((char)m_val);
        }
        // special cases for \t, \r, \n and space
        else if (m_val == ' ')
        {
            displayString = QString("SPC");
        }
        else if (m_val == '\n')
        {
            displayString = QString("\\n");
        }
        else if (m_val == '\r')
        {
            displayString = QString("\\r");
        }
        else if (m_val == '\t')
        {
            displayString = QString("\\t");
        }
        else
        {
            displayString = QString("\\%1").arg((uint)m_val);
        }
    }
    
    return displayString;
}


WordItem::WordItem(unsigned int addr, WordBlock *parent) :
MemItem(addr, 4, "Word", parent),
m_val(0)
{
}

void WordItem::takeValue(State& state)
{
    m_val = state.getMemoryWordUnobserved(m_address);
}

QString WordItem::stringValue()
{
    return QString("0x%1").arg(m_val, 8, 16, (QChar)'0');
}

MemBlock::MemBlock(unsigned int start, unsigned int length, QString labelName) :
m_startAddr(start),
m_len(length),
m_labelName(labelName)
{
}

QRectF MemBlock::boundingRect() const
{
    return childrenBoundingRect();
}
// 
// QPainterPath MemBlock::shape() const
// {
//     QPainterPath childShape;
//     
//     foreach (MemItem *currItem, m_items)
//     {
//         childShape.addRect(currItem->boundingRect());
//     }
//     
//     return childShape;
// }

// this is so all clicks go straight through to children
bool MemBlock::contains ( const QPointF & point ) const
{
    return false;
}

void MemBlock::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
    
    setHandlesChildEvents(false);
}

unsigned int MemBlock::getStartAddress()
{
    return m_startAddr;
}
unsigned int MemBlock::getLength()
{
    return m_len;
}

QString MemBlock::getLabelName()
{
    return m_labelName;
}

void MemBlock::takeValues(State &state)
{
    foreach (MemItem *currItem, m_items)
    {
        currItem->takeValue(state);
    }
}

// ByteBlock and WordBlock should probably be refactored, but I'm not sure how.  
ByteBlock::ByteBlock(unsigned int start, unsigned int size, ByteItem::DisplayType display, QString labelName) :
MemBlock(start, size, labelName)
{    
    ByteItem *currByte;
    
    // make display items for all consecutive bytes in this identifier
    for (unsigned int i=0; i<m_len; i++)
    {
        currByte = new ByteItem(m_startAddr+i, this);
        currByte->setDisplayType(display);
        
        if (i == m_len-1)
        {
            currByte->setAppearanceFlags(currByte->appearanceFlags() | MemItem::LastItemFlag);
        }
        
        if (i == 0)
        {
            currByte->setAppearanceFlags(currByte->appearanceFlags() | MemItem::FirstItemFlag);
        }
        
        // add it to the list
        m_items << currByte;
    }
}
           
WordBlock::WordBlock(unsigned int start, unsigned int length, QString labelName) :
MemBlock(start, length, labelName)
{
    WordItem *currWord;
    
    for (unsigned int i=0; i<length; i++)
    {
        currWord = new WordItem(start+4*i, this);
        
        if (i == m_len-1)
        {
            currWord->setAppearanceFlags(currWord->appearanceFlags() | MemItem::LastItemFlag);
        }
        
        if (i == 0)
        {
            currWord->setAppearanceFlags(currWord->appearanceFlags() | MemItem::FirstItemFlag);
        }
        
        m_items << currWord;
    }
}
