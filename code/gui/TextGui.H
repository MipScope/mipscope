#ifndef TEXTGUI_H_
#define TEXTGUI_H_

#include <QObject>

class QStringList;
class Debugger;
class SyscallListener;
class TextOutputConsole;
class QApplication;

#define PROGRAM_TIMEOUT 30 // seconds

class TextGui : public QObject {
   
   Q_OBJECT
   
   public:
      TextGui(QStringList args);
      void executeProgram(QApplication* app);
      
   public slots:
      void syscallReceived(int no, int valueOfa0);
      void programTerminated(int reason);
      
   private:
      Debugger* m_debugger;
      SyscallListener* m_syscallListener;
      TextOutputConsole* m_output;   
   
};

#endif /*TEXTGUI_H_*/
