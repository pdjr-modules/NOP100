#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H

#include "ModuleInterfaceHandler.h"

class FunctionHandler: public ModuleInterfaceHandler {
  
  public:
    FunctionHandler(unsigned int size = 16);
    void addHandler(unsigned char functionCode, bool (*handler)(unsigned char, unsigned char));
    bool validate(unsigned char functionCode);
    bool process(unsigned char functionCode, unsigned char value);

  private:
    struct FunctionVector { unsigned int functionCode; bool (*handler)(unsigned char, unsigned char); };
    unsigned int size = 0;
    FunctionVector *functionTable;

};

#endif