#include "FunctionHandler.h"

FunctionHandler::FunctionHandler(unsigned int size) {
  this->size = size;
  this->functionTable = new FunctionVector[this->size];
  for (unsigned int i = 0; i < this->size; i++) {
    this->functionTable[i].functionCode = 0;
    this->functionTable[i].handler = 0;
  }
}

void FunctionHandler::addHandler(unsigned char functionCode, bool (*handler)(unsigned char, unsigned char)) {
  for (unsigned int i = 0; i < this->size; i++) {
    if (this->functionTable[i].functionCode == 0) {
      this->functionTable[i].functionCode = functionCode;
      this->functionTable[i].handler = handler;
      break;
    }
  }
}

bool FunctionHandler::validate(unsigned char functionCode) {
  bool retval = false;

  for (unsigned int i = 0; i < this->size; i++) {
    if (this->functionTable[i].functionCode == functionCode) {
      retval = true;
      break;
    }
  }
  return(retval);
}

bool FunctionHandler::process(unsigned char functionCode, unsigned char value) {
  bool retval = false;

  for (unsigned int i = 0; i < this->size; i++) {
    if (this->functionTable[i].functionCode == functionCode) {
      retval = this->functionTable[i].handler(functionCode, value);
      break;
    }
  }
  return(retval);

}
