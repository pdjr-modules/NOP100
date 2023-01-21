/**
 * @file NOP100.cpp
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Extensible firmware based on the NMEA2000 library.
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 *
 * This firmware is targetted at hardware based on the
 * [NOP100](https://github.com/preeve9534/NOP100)
 * module design.
 * It implements a functional NMEA 2000 device that performs no
 * real-world task, but which can be easily extended or specialised
 * into a variant that can perform most things required of an NMEA 2000
 * module.
 * 
 * The firmware implements basic bus connectivity and the normal
 * housekeeping required by NMEA through use of the
 * [NMEA2000](https://github.com/ttlappalainen/NMEA2000) library.
 * It also relieves any derived application of much of the hard work
 * associated with module configuration using the switch inputs of the
 * NOP100 hardware.
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <NMEA2000_CAN.h>
#include <Button.h>
#include <N2kTypes.h>
#include <N2kMessages.h>
#include <IC74HC165.h>
#include <IC74HC595.h>
#include <StatusLeds.h>
#include "ModuleInterface.h"
#include "ModuleConfiguration.h"
#include "FunctionHandler.h"
#include <arraymacros.h>

#include "includes.h"

/**
 * @brief Device information required by the NMEA2000 library.
 * 
 * Most specialisations of NOP100 will want to override DEVICE_CLASS,
 * DEVICE_FUNCTION and perhaps DEVICE_UNIQUE_NUMBER.
 * 
 * DEVICE_CLASS and DEVICE_FUNCTION are explained in the document
 * "NMEA 2000 Appendix B.6 Class & Function Codes".
 * 
 * DEVICE_INDUSTRY_GROUP we can be confident about (4 says maritime).
 * 
 * DEVICE_MANUFACTURER_CODE is only allocated to subscribed NMEA
 * members so we grub around and use 2046 which is currently not
 * allocated.  
 * 
 * DEVICE_UNIQUE_NUMBER is a bit of mystery.
 */
#define DEVICE_CLASS 10                 // System Tools
#define DEVICE_FUNCTION 130             // Diagnostic
#define DEVICE_INDUSTRY_GROUP 4         // Maritime
#define DEVICE_MANUFACTURER_CODE 2046   // Currently not allocated.
#define DEVICE_UNIQUE_NUMBER 849        // Bump me?

/**
 * @brief Product information required by the NMEA2000 library.
 * 
 * Specialisations of NOP100 will want to override most of these.
 * 
 * PRODUCT_CERTIFICATION_LEVEL is granted by NMEA when a product is
 * officially certified. We won't be.
 * 
 * PRODUCT_CODE is our own unique numerical identifier for this device.
 * 
 * PRODUCT_FIRMWARE_VERSION should probably be generated automatically
 * from semewhere.
 * 
 * PRODUCT_LEN specifies the Load Equivalence Network number for the
 * product which encodes the normal power loading placed on the host
 * NMEA bus. One LEN = 50mA and values are rounded up.
 * 
 * PRODUCT_N2K_VERSION is the version of the N2K specification witht
 * which the firmware complies. 
 */
#define PRODUCT_CERTIFICATION_LEVEL 0   // Not certified
#define PRODUCT_CODE 002                // Our own product code
#define PRODUCT_FIRMWARE_VERSION "1.1.0 (Jun 2022)"
#define PRODUCT_LEN 1                   // This device's LEN
#define PRODUCT_N2K_VERSION 2100        // N2K specification version 2.1
#define PRODUCT_SERIAL_CODE "002-849"   // PRODUCT_CODE + DEVICE_UNIQUE_NUMBER
#define PRODUCT_TYPE "SIM108"           // The product name?
#define PRODUCT_VERSION "1.0 (Mar 2022)"

/**
 * @brief Zero terminated list of PGNs transmitted by this firmware
 *        (required by the NMEA2000 library).
 *
 * Specialisations of NOP100 will probably need to override this.
 */
#define NMEA_TRANSMIT_MESSAGE_PGNS { 0L }

/**
 * @brief Vector of PGNs handled by this application and the callback
 *        functions that process them (required by the NMEA2000
 *        library). 
 * 
 * Specialisations of NOP100 may need to override this.
 * 
 * Each entry is a pair { *pgn*, *callback* }, for example
 * { 127501L, handlerForPgn127501 }, and the list must terminate with
 * the special flag value { 0L, 0 }.
 */
#define NMEA_PGN_HANDLERS  { { 0L, 0 } }

/**
 * @brief Enable or disable process messaging on the Arduino serial
 *        output.
 */
#define DEBUG_SERIAL

/**
 * @brief Delay start of firmware execution after boot. 
 *
 * When the Teensy reboots it switches its USB port to serial emulation
 * and it can take a few seconds for a connected host computer to
 * recognise the switch. This millisecond delay can be used to prevent
 * loss of early debug output.
 */
#define DEBUG_SERIAL_START_DELAY 4000

/**
 * @brief EEPROM address at which to persist module configuration data. 
 */
#define MODULE_CONFIGURATION_EEPROM_ADDRESS 0

/**
 * @brief Microcontroller pin definitions for the Teensy 3.2/4.0.
 */
#define GPIO_SIPO_DATA 0
#define GPIO_SIPO_LATCH 1
#define GPIO_SIPO_CLOCK 2
#define GPIO_CAN_TX 3
#define GPIO_CAN_RX 4
#define GPIO_D5 5
#define GPIO_D6 6
#define GPIO_D7 7
#define GPIO_D8 8
#define GPIO_D9 9
#define GPIO_PISO_DATA 10
#define GPIO_PISO_LATCH 11
#define GPIO_PISO_CLOCK 12
#define GPIO_POWER_LED 13
#define GPIO_PRG 14
#define GPIO_TRANSMIT_LED 15
#define GPIO_D16 16
#define GPIO_D17 17
#define GPIO_D18 18
#define GPIO_D19 19
#define GPIO_D20 20
#define GPIO_D21 21
#define GPIO_D22 22
#define GPIO_D23 23

/**
 * @brief Size of the module configuration array. 
 * 
 * A specialization will almost always need to override this definition
 * (in definitions.h) to suit its own configuration needs.
 */
#define CM_SIZE 1

/**
 * @brief List of named locations in the configuration array. 
 */
#define CM_CAN_SOURCE_INDEX 0

/**
 * @brief List of default (initialisation) values for each location in
 *        the configuration array. 
 */
#define CM_CAN_SOURCE_DEFAULT 22

/**
 * @brief Number of milliseconds that the PRG button must be held
 *        closed to constitute a 'long' button press. 
 */
#define CM_LONG_BUTTON_PRESS_INTERVAL 1000UL

/**
 * @brief Number of milliseconds of inactivity that must elapse within
 *        a dialog before the dialog times out. 
 * 
 */
#define CM_DIALOG_INACTIVITY_TIMEOUT 30000UL

/**
 * @brief Number of milliseconds of inactivity that must elapse before
 *        extended operating mode reverts to normal mode.
 */
#define CM_EXTENDED_OPERATING_MODE_INACTIVITY_TIMEOUT 60000UL // 60 seconds

/**
 * @brief Number of milliseconds between updates of the transmit LED.
 * 
 * This value implicitly sets the ON-period and OFF-period of a
 * flashing LED. 
 */
#define SL_TRANSMIT_LED_UPDATE_INTERVAL 100UL

/**
 * @brief The number of status LEDS supported by the firmware.
 * 
 * The NOP100 hardware supports a maximum of 16 LEDs. Most applications
 * will be happy with a maximum of eight. 
 */
#define SL_NUMBER_OF_STATUS_LEDS 8

/**
 * @brief Number of milliseconds between updates of the status LEDs.
 * 
 * This value implicitly sets the ON-period and OFF-period of all
 * status LEDs. 
 */
#define SL_STATUS_LEDS_UPDATE_INTERVAL 100UL

#include "defines.h"

/**
 * @brief Variable capturing the firmware's current operating mode.
 */
enum OperatingMode { normal, extended } OPERATING_MODE = normal;

/**
 * @brief Declarations of local functions.
 */
void messageHandler(const tN2kMsg&);
bool configurationValidator(unsigned int index, unsigned char value);
unsigned char* configurationInitialiser(int& size, unsigned int eepromAddress);

/**
 * @brief Create a ModuleConfiguration object for managing all module
 *        configuration data.
 * 
 * ModuleConfiguration implements the ModuleInterfaceHandler interface
 * and can be managed by the user-interaction manager.
*/
ModuleConfiguration MODULE_CONFIGURATION(configurationInitialiser, configurationValidator, MODULE_CONFIGURATION_EEPROM_ADDRESS);

/**
 * @brief Create a FunctionHandler object for managing all extended
 *        configuration functions.
 * 
 * FunctionHandler implements the ModuleInterfaceHandler interface
 * and can be managed by the user-interaction manager. We'all add
 * functions later in setup().
 */
FunctionHandler::FunctionMap functionMapArray[] = {
  { 255, [](unsigned char i, unsigned char v) -> bool { MODULE_CONFIGURATION.erase(); return(true); } },
  { 0, 0 }
};
FunctionHandler FUNCTION_HANDLER(functionMapArray);

/**
 * @brief Create a ModuleInterface supporting ModuleConfiguration and
 *        FunctionHandler objects.
 */
ModuleInterfaceHandler  *ModeHandlers[] = { &MODULE_CONFIGURATION, &FUNCTION_HANDLER, 0 };
ModuleInterface MODULE_INTERFACE(ModeHandlers);

/**
 * @brief 
 * 
 */
/**
 * @brief Create and initialise an array of transmitted PGNs.
 * 
 * Array initialiser is specified in defined.h. Required by NMEA2000
 * library. 
 */
const unsigned long TransmitMessages[] = NMEA_TRANSMIT_MESSAGE_PGNS;

/**
 * @brief Create and initialise a vector of received PGNs and their
 *        handlers.
 * 
 * Array initialiser is specified in defined.h. Required by NMEA2000
 * library. 
 */
typedef struct { unsigned long PGN; void (*Handler)(const tN2kMsg &N2kMsg); } tNMEA2000Handler;
tNMEA2000Handler NMEA2000Handlers[] = NMEA_PGN_HANDLERS;

/**
 * @brief Create a Button object for debouncing the module's PRG
 *        button.
 */
Button PRG_BUTTON(GPIO_PRG);

/**
 * @brief Register for remembering the time of the most recent PRG
 *        button press. 
 */
unsigned long PRG_PRESSED_AT = 0UL;

/**
 * @brief Interface to the IC74HC165 PISO IC that connects the eight 
 *        DIL switch parallel inputs.
 */
IC74HC165 DIL_SWITCH (GPIO_PISO_CLOCK, GPIO_PISO_DATA, GPIO_PISO_LATCH);

/**
 * @brief Interface to the IC74HC595 SIPO IC that operates the eight
 *        status LEDs. 
 */
IC74HC595 STATUS_LEDS_SIPO(GPIO_SIPO_CLOCK, GPIO_SIPO_DATA, GPIO_SIPO_LATCH);

/**
 * @brief StatusLed object for operating the transmit LED.
 * 
 * The transmit LED is connected directly to a GPIO pin, so the lambda
 * callback just uses a digital write operation to drive the output.
 */
StatusLeds TRANSMIT_LED(1, SL_TRANSMIT_LED_UPDATE_INTERVAL, [](unsigned char status){ digitalWrite(GPIO_TRANSMIT_LED, (status & 0x01)); });

/**
 * @brief StatusLed object for operating the status LEDs.
 * 
 * The status LEDs are connected through a SIPO IC, so the lambda
 * callback can operate all eight LEDs in a single operation.
 */
StatusLeds STATUS_LEDS(SL_NUMBER_OF_STATUS_LEDS, SL_STATUS_LEDS_UPDATE_INTERVAL, [](unsigned char status){ STATUS_LEDS_SIPO.writeByte(status); });

#include "definitions.h"

/**********************************************************************
 * MAIN PROGRAM - setup()
 */
void setup() {
  #ifdef DEBUG_SERIAL
  Serial.begin(9600);
  delay(DEBUG_SERIAL_START_DELAY);
  #endif

  // Initialise all core GPIO pins.
  pinMode(GPIO_POWER_LED, OUTPUT);
  pinMode(GPIO_TRANSMIT_LED, OUTPUT);
  PRG_BUTTON.begin();
  DIL_SWITCH.begin();
  STATUS_LEDS_SIPO.begin();

  // Initialise module configuration (see configurationInitialiser())
  MODULE_CONFIGURATION.setup();

  // Run a startup sequence in the LED display: all LEDs on to confirm
  // function.
  TRANSMIT_LED.setStatus(0xff); STATUS_LEDS.setStatus(0xff); delay(100);
  TRANSMIT_LED.setStatus(0x00); STATUS_LEDS.setStatus(0x00);

  #include "setup.h"

  // Initialise and start N2K services.
  NMEA2000.SetProductInformation(PRODUCT_SERIAL_CODE, PRODUCT_CODE, PRODUCT_TYPE, PRODUCT_FIRMWARE_VERSION, PRODUCT_VERSION);
  NMEA2000.SetDeviceInformation(DEVICE_UNIQUE_NUMBER, DEVICE_FUNCTION, DEVICE_CLASS, DEVICE_MANUFACTURER_CODE);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, MODULE_CONFIGURATION.getByte(CM_CAN_SOURCE_INDEX)); // Configure for sending and receiving.
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.ExtendTransmitMessages(TransmitMessages); // Tell library which PGNs we transmit
  NMEA2000.SetMsgHandler(messageHandler);
  NMEA2000.Open();

  #ifdef DEBUG_SERIAL
  Serial.println();
  Serial.println("Starting:");
  Serial.print("  N2K Source address is "); Serial.println(NMEA2000.GetN2kSource());
  #endif

  MODULE_INTERFACE.revertModeMaybe();
}

/**********************************************************************
 * MAIN PROGRAM - loop()
 * 
 * With the exception of NMEA2000.parseMessages() all of the functions
 * called from loop() implement interval timers which ensure that they
 * will mostly return immediately, only performing their substantive
 * tasks at intervals defined by program constants.
 */ 
void loop() {
  
  // Before we transmit anything, let's do the NMEA housekeeping and
  // process any received messages. This call may result in acquisition
  // of a new CAN source address, so we check if there has been any
  // change and if so save the new address to EEPROM for future re-use.
  NMEA2000.ParseMessages();
  if (NMEA2000.ReadResetAddressChanged()) {
    MODULE_CONFIGURATION.setByte(CM_CAN_SOURCE_INDEX, NMEA2000.GetN2kSource());
  }

  #include "loop.h"

  // If the PRG button has been operated, then call the button handler.
  if (PRG_BUTTON.toggled()) {
    switch (MODULE_INTERFACE.handleButtonEvent(PRG_BUTTON.read(), DIL_SWITCH.readByte())) {
      case ModuleInterface::MODE_CHANGE:
        break;
      default:
        break;
    }
  }

  if (!MODULE_INTERFACE.getCurrentMode()) {
    // Maybe update the transmit and status LEDs.
    TRANSMIT_LED.update(false, true);
    STATUS_LEDS.update(false, true);
  }

  //cancelExtendedOperatingModeMaybe();
}

void messageHandler(const tN2kMsg &N2kMsg) {
  int iHandler;
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  if (NMEA2000Handlers[iHandler].PGN != 0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
}


#ifndef CONFIGURATION_INITIALISER
/**********************************************************************
 * NOP100's configuration consists of just one byte holding the CAN
 * interface source address. We create a one-element array to hold this
 * value and attempt to load any previously saved buffer from EEPROM.
 * If there is no previous value (i.e. this is the first time the
 * firmware has been used) the we write a default value into the buffer
 * and return the whole thing.
*/
unsigned char* configurationInitialiser(int& size, unsigned int eepromAddress) {
  static unsigned char *buffer = new unsigned char[size = CM_SIZE];
  EEPROM.get(eepromAddress, buffer);
  if (buffer[CM_CAN_SOURCE_INDEX] == 0xff) {
    buffer[CM_CAN_SOURCE_INDEX] = CM_CAN_SOURCE_DEFAULT;
    EEPROM.put(eepromAddress, buffer);
  }
  return(buffer);
}
#endif

#ifndef CONFIGURATION_VALIDATOR
/**********************************************************************
 * NOP100's configuration consists of just one byte holding the CAN
 * interface source address, so <index> will only ever be 0. We accept
 * any value and there is no need to advise any other software
 * components of an update.
 */
bool configurationValidator(unsigned int index, unsigned char value) {
  return(true);
}
#endif

/**********************************************************************
 * This function is called when value has been entered through the
 * configuration dialogue (i.e. by a short button press) without any
 * prior entry of a configuration address. This means that the user has
 * requested a special function, but NOP100 doesn't support any!
*/
#ifndef EXTENDED_INTERACT
int extendedInteract(unsigned int value, bool longPress) {
  return(0);
}
#endif
