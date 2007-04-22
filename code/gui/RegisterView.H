/* ---------------------------------------------- *\
   file: RegisterView.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/18/2007
\* ---------------------------------------------- */
#ifndef __REGISTER_VIEW_H__
#define __REGISTER_VIEW_H__
#include <QDockWidget>
#include <QLabel>

#define REGISTER_HOVER_DELAY     (0)
#define NO_REGISTERS    (32 + 3)
#define EXTENDED_FADE_INTERVAL   (33)
#define EXTENDED_FADE_DURATION   (EXTENDED_FADE_INTERVAL * 10)


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
class RegisterOptionsPane;
extern QString getNoInBase(unsigned int no, int base);
extern QString getRegisterText(int register, int displayType);

class RegisterView : public QDockWidget {
   
   Q_OBJECT
   
   public:
      friend class RegisterOptionsPane;
      
      RegisterView(Gui *parent, EditorPane *editorPane);
   
      void displayTypeChanged();
      
   private slots:
      // Clears all registers
      void reset();
      
      // Updates display of modified register
      void registerChanged(int reg, int value);
      
   protected:
      Gui *m_gui;
      EditorPane *m_editorPane;
      
      // tabbed widget w/ View/Options tabs
      QTabWidget *m_tabWidget;
      RegisterPane *m_registerPane;
      RegisterOptionsPane *m_optionsPane;
};

class RegisterPane : public QWidget {
   
   Q_OBJECT
   
   public:
      friend class RegisterView;
      friend class RegisterLabel;
      friend class IDLabel;
      
      RegisterPane(RegisterView *regView);
      
      void showExtended(const QPoint &pos, const QString &text, RegisterLabel *orig);
      void reset();
      void testMouseMove(QMouseEvent *e, const QPoint &pos);
      void testMousePress(QMouseEvent *e, const QPoint &pos);
   
      void displayTypeChanged();

   protected:
      RegisterView *m_parent;
      // View tab contains a 9x12 QGridLayout of RegViewLabels (generic)
      IDLabel *m_registerLabels[NO_REGISTERS];
      QRect m_bounds;
      QWidget *m_widget;
      ExtendedView *m_extended;
      
      // @override
      virtual void leaveEvent(QEvent *);
      //virtual QSize sizeHint() const;
      //virtual QSize minimumSize() const;
};

// View of Value of a label
class RegisterLabel : public QLabel {
   
   Q_OBJECT
   
   public:
      RegisterLabel(RegisterPane *regPane);
      
      virtual void handleClick(QMouseEvent *e) = 0;
      virtual void showExtended(const QPoint &p, bool alreadyAdjusted = false, RegisterLabel *v = NULL) = 0;
      virtual void testMouseMove(QMouseEvent *e, const QPoint &parentPos);
      void mousePressed(QMouseEvent *e, const QPoint &parentPos);
      
   protected slots:
      void checkForExtended();
   
   protected:
      RegisterPane *m_parent;

      // Pertaining to detection of extended popup info
      bool m_isInside;
      QTime *m_time;
      QPoint m_lastPos;
      
      // @overridden mouse hover/click detection
      virtual void mousePressEvent(QMouseEvent *);
      virtual void mouseReleaseEvent(QMouseEvent *);
      virtual void mouseMoveEvent(QMouseEvent *);
      virtual void enterEvent(QEvent *);
      virtual void leaveEvent(QEvent *);
};

class ValueLabel;
class IDLabel : public RegisterLabel {
   
   Q_OBJECT
   
   public:
      friend class ValueLabel;
      
      IDLabel(RegisterPane *regVew, unsigned int id);
      
      void reset();
      void handleClick(QMouseEvent *e);
      void showExtended(const QPoint &p, bool alreadyAdjusted = false, RegisterLabel *v = NULL);
      void setValue(int value);
      
      QPixmap merge(const QPixmap *pix, const QString &txt);
      ValueLabel *getValueLabel();

   public slots:
      void updateDisplay();   
      
   protected:
      unsigned int m_register; // id
      unsigned int m_value; // current value
      
      ValueLabel *m_valueLabel;
      bool m_watchPoint;


      // etwas for who last changed register

};

enum IDDisplay { D_REGISTER, D_ALIAS };

// Different ways values in registers may be viewed
enum ValueDisplay { D_HEX, D_BINARY, D_SIGNED_DECIMAL, D_UNSIGNED_DECIMAL };
extern ValueDisplay m_displayType;
extern IDDisplay m_registerDisplayType;

class ValueLabel : public RegisterLabel {
   
   Q_OBJECT
   
   public:
            
      ValueLabel(RegisterPane *regVew, IDLabel *idLabel);
      
      virtual void handleClick(QMouseEvent *e);
      virtual void showExtended(const QPoint &p, bool alreadyAdjusted = false, RegisterLabel *v = NULL);
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

class RegisterOptionsPane : public QWidget {
   
   Q_OBJECT
   
   public:
      RegisterOptionsPane(RegisterView *parent);
      
   private slots:
      void registerDisplayTypeChanged(int);
      void defaultBaseChanged(int);
      
   protected:
      RegisterView *m_parent;
};



#endif // __REGISTER_VIEW_H__

