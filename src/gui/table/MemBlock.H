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

#ifndef _MEMBLOCK_H_
#define _MEMBLOCK_H_

#include <QGraphicsItem>

class MemBlock;
class WordBlock;
class ByteBlock;
class State;

/**
 * MemItem class
 * - graphical representation of an atomic memory element (i.e. byte or word)
 * - has an address, width (in bytes), and a parent MemBlock
 */
class MemItem : public QGraphicsItem, public QObject
{    
public:
    typedef enum 
    {
        AsciiDisplay, NumberDisplay
    } DisplayType;
    
    enum AppearanceFlag
    {
        FirstItemFlag = 0x1,
        LastItemFlag = 0x2  
    };
    Q_DECLARE_FLAGS(AppearanceFlags, AppearanceFlag);
    
    MemItem(unsigned int addr, unsigned char bytesWide, QString typeName, MemBlock *parent);
    
    unsigned int getAddress();
    
    DisplayType getDisplayType();
    void setDisplayType(DisplayType type);
    
    AppearanceFlags appearanceFlags();
    void setAppearanceFlags(AppearanceFlags newFlags); 

    QRectF boundingRect() const;
    
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    
    void paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget); 
    
    virtual QString stringValue() = 0;
    virtual void takeValue(State& state) = 0;
        
protected:
    MemBlock *m_block;
    unsigned int m_address;
    
    unsigned int m_bytesWide;
    QString m_typeName;
    
    QRectF m_rect;
    DisplayType m_display;
    AppearanceFlags m_flags;
};

/**
 * ByteItem class
 * - represents a byte in memory
 * - subclass of MemItem
 */
class ByteItem : public MemItem
{
public:
    ByteItem(unsigned int addr, ByteBlock *parent = NULL);
    
    unsigned char value();
    void setValue(unsigned char val);
    
    virtual void takeValue(State& state);
    virtual QString stringValue();
    
private:
    unsigned char m_val;
};

/**
 * WordItem class
 * - represents a word in memory
 * - subclass of MemItem
 */
class WordItem : public MemItem
{
public:
    WordItem(unsigned int addr, WordBlock *parent = NULL);
    
    virtual void takeValue(State& state);
    virtual QString stringValue();

private:
    unsigned int m_val;
};

/**
 * MemBlock class
 * - represents a contiguous region of homogeneous memory
 * - collection of MemItems
 */
class MemBlock : public QObject, public QGraphicsItem
{
    Q_OBJECT
    
public:
    MemBlock(unsigned int start, unsigned int length, QString labelName=QString());
    
    QRectF boundingRect() const;
    
    // shape() is actually pretty important for MemBlocks
    // it's default implementation here just puts all the
    //   children bounding rects into a shape
    //QPainterPath shape() const;
    
    bool contains ( const QPointF & point ) const;

    void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);
            
    unsigned int getStartAddress();
    unsigned int getLength();
    QString getLabelName();
    
    void takeValues(State &state);
    
signals:
    void gotoDeclaration();

protected:
    unsigned int m_startAddr;
    unsigned int m_len;
    QString m_labelName;
    
    QList<MemItem *> m_items;
};

/**
 * ByteBlock class
 * - represents a contiguous sequence of bytes
 * - concrete subclass of MemBlock
 */
class ByteBlock : public MemBlock
{
public:
     ByteBlock(unsigned int start, unsigned int size, MemItem::DisplayType display = MemItem::NumberDisplay, QString labelName=QString());

private:
};

/**
 * WordBlock class
 * - represents a contiguous sequence of words
 * - concrete subclass of MemBlock
 */
class WordBlock : public MemBlock
{
public:
     WordBlock(unsigned int start, unsigned int length, QString labelName=QString());

private:
};

#endif /* _MEMBLOCK_H_ */
