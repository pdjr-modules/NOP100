#include "FunctionHandler.h"

FunctionHandler::FunctionHandler(FunctionMap *functionMapArray) {
  this->functionVectors = functionVectors;
}

bool FunctionHandler::validate(unsigned char functionCode) {
  bool retval = false;

  for (unsigned int i = 0; this->functionVectors[i].handler != 0; i++) {
    if (this->functionVectors[i].functionCode == functionCode) {
      retval = true;
      break;
    }
  }
  return(retval);
}

bool FunctionHandler::process(unsigned char functionCode, unsigned char value) {
  bool retval = false;

  for (unsigned int i = 0; this->functionVectors[i].handler != 0; i++) {
    if (this->functionVectors[i].functionCode == functionCode) {
      retval = this->functionVectors[i].handler(functionCode, value);
      break;
    }
  }
  return(retval);

}
