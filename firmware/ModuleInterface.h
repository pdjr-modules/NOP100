#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

#include "ModuleInterfaceHandler.h"

class ModuleInterface {

  public:
    /**
     * @brief Result codes for the handleButtonEvent() method.
     */
    enum EventOutcome { MODE_CHANGE, ADDRESS_ACCEPTED, ADDRESS_REJECTED, VALUE_ACCEPTED, VALUE_REJECTED };

    /**
     * @brief Structure used for passing ModuleInterfaceHandlers into the
     * 
     */
    typedef struct { unsigned int mode; ModuleInterfaceHandler *handler; } ModeHandler;

    ModuleInterface(ModeHandler *modeHandlers, unsigned long revertInterval = 30000);

    /**
     * @brief Get the current operating mode.
     * 
     * @return int    - the current operating mode.
     */
    int getCurrentMode();

    /**
     * @brief Get the time in milliseconds of the last invocation of handleButtonEvent().
     * 
     * @return unsigned long - timetsamp in milliseconds.
     */
    unsigned long getButtonPressedAt();

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
    EventOutcome handleButtonEvent(bool buttonState, unsigned char value = 0);

    void revertModeMaybe();
    
  private:
    ModeHandler *modeHandlers;
    unsigned int currentMode;
    int currentAddress;
    unsigned long buttonPressedAt;
    unsigned long revertInterval;

};

#endif
