#ifndef MODULE_INTERFACE_HANDLER_H
#define MODULE_INTERFACE_HANDLER_H

/**
 * @brief Abstract class defining interface used by ModuleInterface.
 * 
 * ModuleInterface supports different operating modes where each mode
 * is handled by a class which does somethind specific. The interface
 * between these components is ModuleInterfaceHandler.
 * 
 * A class that will be used by ModuleInterface must implement this
 * interface.
 */
class ModuleInterfaceHandler {

  public:

    /**
     * @brief Validate the *address* value deriving from a
     *        ModuleInterface dialogue.
     * 
     * ModuleInterface implements a two-stage data entry mode in which
     * an *address* is entered, followed by a *value*. This method is
     * used by a handler to validate an *address* before it is required
     * to process it.
     * 
     * @param address - *address* value to be validated.
     * @return true   - the address is valid.
     * @return false  - the address is not valid.
     */
    virtual bool validateAddress(unsigned char address) = 0;

    /**
     * @brief Process an *address* and an associated *value*.
     * 
     * ModuleInterface implements a two-stage data entry mode in which
     * an *address* is entered, followed by a *value*. This method is
     * used by a handler to process an *address* / *value* pair.
     *
     * @param address - the *address* value. 
     * @param value   - the *value* value.
     * @return true   - processing of the data pair was successfully. 
     * @return false  - processing of the data pair failed.
     */
    virtual bool processValue(unsigned char address, unsigned char value) = 0;
};

#endif