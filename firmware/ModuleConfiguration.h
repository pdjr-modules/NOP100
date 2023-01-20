#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include "ModuleInterfaceHandler.h"

/**
 * @brief Representation of a byte array that can be used as a
 *        basis for firmware configuration.
 * 
 * A module configuration is modelled as a byte array indexed from
 * zero.
 */
class ModuleConfiguration: public ModuleInterfaceHandler {

  public:
    /******************************************************************
     * @brief Construct a new Module Configuration object.
     * 
     * @param initialiser   - callback function that will be used to
     *                        load / default the module configuration.
     * @param validator     - callback function that will be used to
     *                        validate and approve every byte written
     *                        to the module configuration.
     * @param eepromAddress - address in EEPROM where the configuration
     *                        will be persisted.
     */    
    ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*validator)(unsigned int, unsigned char), unsigned int eepromAddress = 0);

    /******************************************************************
     * @brief Initialise the configuration by executing the initialiser
     *        callback.
     */
    void setup();

    /******************************************************************
     * @brief Set a byte in the configuration from a value subject to
     *        approval by the validator callback.
     * 
     * @param index  - the index within the configuration where value
     *                 should be saved.
     * @param value  - the value to be saved - this will be passed to
     *                 the validator callback for validation before
     *                 being committed.
     * @return true  - value was saved successfully.
     * @return false - value was not saved because index was out of
     *                 range or validation checks failed.
     */
    bool setByte(unsigned int index, unsigned char value);

    /******************************************************************
     * @brief Get a byte from the configuration.
     * 
     * @param                - index within the configuration of the
     *                         required value. 
     * @return unsigned char - the retrieved byte or 0xff if index was
     *                         out of range.
     */
    unsigned char getByte(unsigned int index);

    bool validate(unsigned char index);
    bool process(unsigned char address, unsigned char value);

    /******************************************************************
     * @brief Save a configuration byte to EEPROM using EEPROM.update().
     * 
     * @param index - the index of the configuration byte to be saved. 
     */
    void saveByte(unsigned int index);

    /******************************************************************
     * @brief Save the entire configuration array to EEPROM.
     */
    void save();

    /******************************************************************
     * @brief Load the entire configuration array from EEPROM.
     */
    void load();

    void erase();

  private:
    unsigned char* (*initialiser)(int&, unsigned int);
    bool (*validator)(unsigned int, unsigned char);
    unsigned int eepromAddress;

    unsigned long interactionTimeout = 30000UL;
    unsigned char *configuration = 0;
    unsigned int size = 0;
    
};

#endif