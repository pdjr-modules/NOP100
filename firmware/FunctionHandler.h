#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H

class FunctionHandler: public ModuleInterfaceHandler {
  
  public:
    FunctionHandler(unsigned long size = 16);
    void addHandler(unsigned char functionCode, int (*handler)(unsigned char, unsigned char));
    bool validate(unsigned char functionCode);
    bool process(unsigned char functionCode, unsigned char value);

  private:
    struct FunctionVector { unsigned int functionCode; int (*handler)(unsigned char, unsigned char); };
    unsigned int size = 0;
    FunctionVector *functionTable;

};

#endif