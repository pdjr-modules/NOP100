#include <Arduino.h>
#include <Button.h>
#include "ModuleInterface.h"
  
ModuleInterface::ModuleInterface(ModuleInterfaceHandler *defaultHandler, int numberOfModes, unsigned long revertInterval) {
  this->numberOfModes = numberOfModes;
  this->handlers = new ModuleInterfaceHandler*[this->numberOfModes];
  for (unsigned int i = 0; i < this->numberOfModes; i++) this->handlers[i] = (i == 0)?defaultHandler:0;
  this->revertInterval = revertInterval;
  this->currentMode = 0;
  this->buttonPressedAt = 0UL;
}

int ModuleInterface::addHandler(ModuleInterfaceHandler *handler) {
  unsigned int mode;

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

void ModuleInterface::revertModeMaybe() {
  if (millis() > (this->buttonPressedAt + this->revertInterval)) {
    this->currentMode = 0;
    this->currentAddress = -1;
  }
}
    
ModuleInterface::EventOutcome ModuleInterface::handleButtonEvent(bool buttonState, unsigned char value) {
  EventOutcome retval = MODE_CHANGE;
  unsigned long now = millis();

  if (buttonState == Button::PRESSED) {
    this->buttonPressedAt = now;
  } else {
    if ((this->buttonPressedAt) && (now < (this->buttonPressedAt + 1000))) {
      if (this->currentAddress != -1) {
        retval = (this->handlers[this->currentMode]->process((unsigned char) this->currentAddress, value))?VALUE_ACCEPTED:VALUE_REJECTED;
        this->currentAddress = -1;
      } else {
        this->currentMode = ((this->currentMode + 1) % this->numberOfModes);
        retval = MODE_CHANGE;
      }
    } else {
      if (this->handlers[this->currentMode]->validate(value)) {
        this->currentAddress = value;
        retval = ADDRESS_ACCEPTED;
      } else {
        this->currentAddress = -1;
        retval = ADDRESS_REJECTED;
      }
    }
  }
  return(retval);
}
