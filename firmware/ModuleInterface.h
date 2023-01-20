#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

#include "ModuleInterfaceHandler.h"

class ModuleInterface {

  public:
    enum EventOutcome { MODE_CHANGE, ADDRESS_ACCEPTED, ADDRESS_REJECTED, VALUE_ACCEPTED, VALUE_REJECTED };

    /**
     * @brief Construct a new ModuleInterface object.
     * 
     * @param defaultHandler - the ModuleInterfaceHandler object for mode 0.
     * @param numberOfModes  - the number of modes that are supported by this ModuleInterface.
     * @param revertInterval - the number of milliseconds a non-default mode can exist without
     *                         a call to handleButtonEvent() before the current mode will be
     *                         reset to the default mode.
     */
    ModuleInterface::ModuleInterface(ModuleInterfaceHandler *defaultHandler, int numberOfModes = 1, unsigned long revertInterval = 30000UL);

    /**
     * @brief Add a non-default ModuleInterfaceHandler object.
     * 
     * @param handler - the ModuleInterfaceHandler object.
     * @return int    - the mode number assigned to the new handler or
     *                  zero if the handler could not be assigned.
     */
    int ModuleInterface::addHandler(ModuleInterfaceHandler *handler);

    /**
     * @brief Get the current operating mode.
     * 
     * @return int    - the current operating mode.
     */
    int ModuleInterface::getCurrentMode();

    /**
     * @brief Get the time in milliseconds of the last invocation of handleButtonEvent().
     * 
     * @return unsigned long - timetsamp in milliseconds.
     */
    unsigned long ModuleInterface::getButtonPressedAt();

    /**
     * @brief Calls the an appropriate handler method for the current
     *        operating mode.
     * 
     * This method should be called with buttonState set to either
     * Button::PRESSED or Button::RELEASED. On a release, a long press
     * will result in a call to the current mode handler's setAddress()
     * method, otherwise to the current mode handler's processValue()
     * method, but only if an address has previously been set.
     * 
     * @param buttonState 
     * @param value 
     * @return int 
     */
    EventOutcome ModuleInterface::handleButtonEvent(bool buttonState, unsigned char value = 0);
    
  private:
    unsigned int numberOfModes;
    ModuleInterfaceHandler *handlers;
    unsigned int currentMode;
    unsigned long buttonPressedAt;
    unsigned long revertInterval;

};

#endif
