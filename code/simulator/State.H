#ifndef STATE_H_
#define STATE_H_

#include <QObject>
#include <QVector>

#include "typedefs.H"
class ParseNode;

class State : public QObject {
   Q_OBJECT

   public:
      State();		

      // increments timestamp
      TIMESTAMP newTimestamp();

      // memory
      void setMemoryWord(unsigned int address, unsigned int value);
      void setMemoryByte(unsigned int address, unsigned char value);
      unsigned int getMemoryWord(unsigned int address) const;
      unsigned char getMemoryByte(unsigned int address) const;

      void ensureValidAlignment(unsigned int address, unsigned int align) const;

      // utility methods for bulk-operations (mainly for Directive-initialization)
      void memcpy(unsigned int destAddress, const void *src, unsigned int size);
      void memset(unsigned int destAddress, const int value, unsigned int size);

      // registers
      void setRegister(int reg, unsigned int value);
      unsigned int getRegister(int reg) const;

      // pc
      void incrementPC();
      void setPC(ParseNode* value);
      ParseNode* getPC(void) const;

      // undo
      void undoUntilTimestamp(TIMESTAMP timestamp);
      TIMESTAMP getCurrentTimestamp(void) const;

      // special
      void syscall(void);
      void assertEquals(int val1, int val2); // throws AssertionFailure
      
      // completely resets the state to its original state
      void reset();
      
signals:

      /* this is emitted whenever a word in memory is changed, even if only
       * one byte of this word was changed with a call to setMemoryByte. */
      void memoryWordChanged(ParseNode* who, unsigned int address, unsigned int previous, unsigned int current);

      void registerChanged(ParseNode* who, int reg, unsigned int previous, unsigned int current);

      void pcChanged(ParseNode* who, ParseNode* previous, ParseNode* current);

   private:
      unsigned int m_registers[register_count];
      QHash<unsigned int, unsigned int> m_memory;
      //QVector<char> m_text;

      ParseNode* m_pc;

      TIMESTAMP m_currentTimestamp;
};



#endif // STATE_H_

