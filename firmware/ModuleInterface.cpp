#include <Arduino.h>
#include <Button.h>
#include "ModuleInterface.h"
  
ModuleInterface::ModuleInterface(ModeHandler *modeHandlers, unsigned long revertInterval) {
  this->modeHandlers = modeHandlers;
  this->revertInterval = revertInterval;
  this->currentMode = 0;
  this->currentAddress = -1;
  this->buttonPressedAt = 0UL;
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
        retval = (this->modeHandlers[this->currentMode].handler->process((unsigned char) this->currentAddress, value))?VALUE_ACCEPTED:VALUE_REJECTED;
        this->currentAddress = -1;
      } else {
        this->currentMode++;
        if (this->modeHandlers[this->currentMode].handler == 0) this->currentMode = 0;
        retval = MODE_CHANGE;
      }
    } else {
      if (this->modeHandlers[this->currentMode].handler->validate(value)) {
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
