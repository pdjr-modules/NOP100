#include "StateMachine.h"

StateMachine::StateMachine(int initialState, tJumpVector *jumpVector) {
    this->state = initialState;
    this->jumpVector = jumpVector;
}

void StateMachine::setState(int state) {
    this->state = state;
}

void StateMachine::process(uint8_t value) {
  for (int i = 0; this->jumpVector[i].handler != 0; i++) {
    if (this->jumpVector[i].state == this->state) {
      this->state = (this->jumpVector[i].handler)(value);
      break;
    }
  }
}