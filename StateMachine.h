#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>

class StateMachine {
  public:
    typedef struct { int state; int (*handler)(uint8_t value); } tJumpVector;

    StateMachine(int initialState, tJumpVector *jumpVector);
    void setState(int state);
    void process(uint8_t value);
  
  private:
    tJumpVector *jumpVector;
    int state = 0;

};

#endif