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
   file: RegisterView.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __REGISTER_VIEW_H__
#define __REGISTER_VIEW_H__

#include "Utilities.H"
#include <QDockWidget>
#include <QLabel>

#define REGISTER_HOVER_DELAY     (0)
#define NO_REGISTERS    (32 + 3)

extern const char *const registerAliases[];
extern QPixmap *watchPoint;

class Gui;
class EditorPane;
class RegViewLabel;
class RegisterLabel;
class IDLabel;
class QTabWidget;
class RegisterPane;
class QTime;
class QPixmap;
class ExtendedView;
class ParseNode;
//class RegisterOptionsPane;
class CustomBackgroundWidget;
extern QString getNoInBase(unsigned int no, int base);
extern QString getRegisterText(int register, bool displayType);

class RegisterView : public QDockWidget {
   
   Q_OBJECT
   
   public:  // this is a horribly designed set of classes.. sorry
      friend class RegisterPane;
//      friend class RegisterOptionsPane;
      friend class IDLabel;
      
      RegisterView(Gui *parent, EditorPane *editorPane);
   
      bool *getWatchpoints();
      void displayTypeChanged();
      void updateDisplay(int state);
      
      QString getRegisterText(int registerNo) const;
      
   public slots:
      // Clears all registers
      void reset();
      
      void updateDisplay(bool);
      void updateBase(int);

      // Redisplays all register values
      void updateDisplay();
      
      // Updates display of modified register
      void registerChanged(unsigned int reg, unsigned int value, int status, ParseNode *pc);

      void pcChanged(ParseNode *pc, bool justRolledBack);
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      
      // tabbed widget w/ View/Options tabs
      QTabWidget *m_tabWidget;
      RegisterPane *m_registerPane;
//      RegisterOptionsPane *m_optionsPane;
};

class RegisterPane : public QWidget {
   
   Q_OBJECT
   
   public:
      friend class RegisterView;
      friend class RegisterLabel;
      friend class IDLabel;
      
      RegisterPane(RegisterView *regView);
      
      bool *getWatchpoints();
      void showExtended(const QPoint &pos, const QString &text, RegisterLabel *orig);
      void reset();
      void testMouseMove(QMouseEvent *e, const QPoint &pos);
      void testMousePress(QMouseEvent *e, const QPoint &pos);
   
      QString getRegisterText(int registerNo);
      void displayTypeChanged();
      
      // IDLabels notify RegisterPane which notifies the active prog
      // which notifies the active Debugger
      void watchPointModified(unsigned int reg, bool watchPoint);
      
   protected:
      RegisterView *m_parent;
      // View tab contains a 9x12 QGridLayout of RegViewLabels (generic)
      IDLabel *m_registerLabels[NO_REGISTERS];
      QRect m_bounds;
      CustomBackgroundWidget *m_widget;
      ExtendedView *m_extended;
      
      bool m_watchPoints[register_count];
      
      // @override
//      virtual void leaveEvent(QEvent *);
      virtual void leaveEvent(QEvent *);
};

// View of Value of a label
class RegisterLabel : public QLabel {
   
   Q_OBJECT
   
   public:
      RegisterLabel(RegisterPane *regPane);
      
      virtual void handleClick(QMouseEvent *e) = 0;
      virtual void showExtended(const QPoint &p) = 0;//, bool alreadyAdjusted = false, RegisterLabel *v = NULL) = 0;
      virtual void testMouseMove(QMouseEvent *e, const QPoint &parentPos);
      //void mousePressed(QMouseEvent *e, const QPoint &parentPos);
      void clicked(QMouseEvent *e, const QPoint &pos);
      
//   protected slots:
//      void checkForExtended();
   
   protected:
      RegisterPane *m_parent;
      
      // Pertaining to detection of extended popup info
      //QTime *m_time;
      //QPoint m_lastPos;
      
      // @overridden mouse hover/click detection
      //virtual void mousePressEvent(QMouseEvent *);
      //virtual void mouseReleaseEvent(QMouseEvent *);
      virtual void mouseMoveEvent(QMouseEvent *);
      //virtual void enterEvent(QEvent *);
      //virtual void leaveEvent(QEvent *);*/
};

class ValueLabel;
class IDLabel : public RegisterLabel {
   
   Q_OBJECT
   
   public:
      friend class ValueLabel;
      friend class RegisterPane;
      
      IDLabel(RegisterPane *regVew, unsigned int id);
      
      void reset();
      void handleClick(QMouseEvent *e);
      void showExtended(const QPoint &p);//, bool alreadyAdjusted = false, RegisterLabel *v = NULL);
      void setValue(unsigned int value, ParseNode *pc = NULL);
      unsigned int getValue() const;
      
      QPixmap merge(const QPixmap *pix, const QString &txt);
      ValueLabel *getValueLabel();

   public slots:
      void updateDisplay();   
      
   protected:
      unsigned int m_register; // id
      unsigned int m_value; // current value
      
      ValueLabel *m_valueLabel;
      bool m_watchPoint;

      // who was last to modify this register
      ParseNode *m_lastModified;
};

//enum IDDisplay { D_REGISTER, D_ALIAS };

// Different ways values in registers may be viewed
//enum ValueDisplay { D_HEX, /*D_BINARY, */D_SIGNED_DECIMAL, D_UNSIGNED_DECIMAL };
//extern ValueDisplay m_displayType;
//extern IDDisplay m_registerDisplayType;

class ValueLabel : public RegisterLabel {
   
   Q_OBJECT
   
   public:
            
      ValueLabel(RegisterPane *regVew, IDLabel *idLabel);
      
      virtual void handleClick(QMouseEvent *e);
      virtual void showExtended(const QPoint &p);//, bool alreadyAdjusted = false, RegisterLabel *v = NULL);
      void setValue(unsigned int newValue);
      
   protected:
      IDLabel *m_idLabel;
};

class ExtendedView : public QLabel {
   
   Q_OBJECT
   
   public:
      ExtendedView(RegisterPane *regPane, QWidget *parent);
       
      void show(const QString &text, const QString &statusText, const QPoint &pos, RegisterLabel *orig);
      
      // @overridden
      virtual void mousePressEvent(QMouseEvent*);
      virtual void mouseMoveEvent(QMouseEvent *);
      
      RegisterLabel *m_orig;

   public slots:
      void hide();

   protected:
      RegisterPane *m_registerPane;
      QWidget *m_parent;

      enum State { S_Hidden, S_Appearing, S_Fading, S_Normal };
      QTimer *m_timer;
      State m_state;
      int m_alpha;
      
      void paintEvent(QPaintEvent *e);
};

/*class RegisterOptionsPane : public QWidget {
   
   Q_OBJECT
   
   public:
      RegisterOptionsPane(RegisterView *parent);
      
   private slots:
      void registerDisplayTypeChanged(int);
      void defaultBaseChanged(int);
      
   protected:
      RegisterView *m_parent;
};*/



#endif // __REGISTER_VIEW_H__

