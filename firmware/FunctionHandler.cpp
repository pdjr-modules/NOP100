#include "FunctionHandler.h"

FunctionHandler::FunctionHandler(FunctionMap *functionMapArray) {
  this->functionMapArray = functionMapArray;
}

bool FunctionHandler::validate(unsigned char functionCode) {
  bool retval = false;

  for (unsigned int i = 0; this->functionMapArray[i].handler != 0; i++) {
    if (this->functionMapArray[i].functionCode == functionCode) {
      retval = true;
      break;
    }
  }
  return(retval);
}

bool FunctionHandler::process(unsigned char functionCode, unsigned char value) {
  bool retval = false;

  for (unsigned int i = 0; this->functionMapArray[i].handler != 0; i++) {
    if (this->functionMapArray[i].functionCode == functionCode) {
      retval = this->functionMapArray[i].handler(functionCode, value);
      break;
    }
  }
  return(retval);

}
