#include "ModuleInterface.h"
  
ModuleInterface(ModuleInterfaceHandler *defaultHandler, int numberOfModes, unsigned long revertInterval = 0UL) {
  this->numberOfModes = numberOfModes;
  this->handlers = new ModuleInterfaceHandler[this->numberOfModes];
  for (unsigned int i = 0; i < this->numberOfModes; i++) this->handlers[i] = (i == 0)?defaultHandler:0;
  this->revertInterval = revertInterval;
  this->currentMode = 0;
  this->buttonPressedAt = 0UL;
}

int ModuleInterface::addHandler(ModuleInterfaceHandler *handler) {
  int mode;

  for (mode = 0; mode < this->numberOfModes; mode++) {
    if (this->handlers[mode] == 0) {
      this->handlers[mode] = handler;
      break;
    }
  }
  return(mode);
}

int ModuleInterface::getCurrentMode() {
  return(this->currentMode);
}

unsigned long ModuleInterface::getButtonPressedAt() {
  return(this->buttonPressedAt);
}
    
EventOutcome ModuleInterface::handleButtonEvent(bool buttonState, unsigned char value = 0) {
  EventOutcome retval = MODE_CHANGE;
  unsigned long now = millis();
  static unsigned char address = -1;

  if (buttonState == Button::PRESSED) {
    this->buttonPressedAt = now;
  } else {
    if ((this->buttonPressedAt) && (now < (this->buttonPressedAt + 1000))) {
      if (address != -1) {
        retval = (this->handlers[this->currentMode].process(address, value))?VALUE_ACCEPTED:VALUE_REJECTED;
        address = -1;
      } else {
        this->currentMode = ((this->currentMode + 1) % this->numberOfModes);
        retval = MODE_CHANGE;
      }
    } else {
      if (this->handlers[this->currentMode].validate(value)) {
        address = value;
        retval = ADDRESS_ACCEPTED;
      } else {
        address = -1;
        retval = ADDRESS_REJECTED;
      }
    }
  }
  return(retval);
}
