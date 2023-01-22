#ifndef MODULE_INTERFACE_HANDLER_H
#define MODULE_INTERFACE_HANDLER_H

/**
 * @brief Abstract class defining interface used by ModuleInterface
 *        client classes.
 * 
 * Each mode supported by @ref ModuleInterface is handled by a class
 * which must implement this interface, probably by sub-classing it.
 */
class ModuleInterfaceHandler {

  public:

    /**
     * @brief Validate the *address* value deriving from a
     *        ModuleInterface dialogue.
     * 
     * ModuleInterface calls this function immediately an address has
     * been supplied by the user and before entry of an associated
     * value. If validation of the address fails, then ModuleInterface
     * can signal an issue and cancel a problematic dialogue.
     * 
     * @param address - *address* value to be validated.
     * @return true   - the address is valid.
     * @return false  - the address is not valid.
     */
    virtual bool validateAddress(unsigned char address) = 0;

    /**
     * @brief Process an *address* and an associated *value*.
     * 
     * ModuleInterface calls this function after an address has been
     * validated with the value supplied by the user consequent on
     * entry af the address.
     * 
     * @param address - the previously validated *address* value. 
     * @param value   - the *value* value.
     * @return true   - processing of the data pair was successfully. 
     * @return false  - processing of the data pair failed. Maybe value
     *                  is invalid, maybe the address/value combination
     *                  is invalid, maybe the invoked process failed.
     */
    virtual bool processValue(unsigned char address, unsigned char value) = 0;
};

#endif