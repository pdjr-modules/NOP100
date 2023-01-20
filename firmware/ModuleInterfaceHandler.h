#ifndef MODULE_INTERFACE_HANDLER_H
#define MODULE_INTERFACE_HANDLER_H

class ModuleInterfaceHandler {
  public:
    virtual bool validate(unsigned char address) = 0;
    virtual bool process(unsigned char address, unsigned char value) = 0;
};

#endif