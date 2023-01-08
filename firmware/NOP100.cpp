/**********************************************************************
 * NOP100.cpp - firmware for an NMEA 2000 module that does nothing.
 * Copyright (c) 2021-22 Paul Reeve <preeve@pdjr.eu>
 *
 * Target platform: Teensy 4.0
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
#include <arraymacros.h>

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
#include "module-libraries.inc"
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

/**********************************************************************
 * SERIAL DEBUG
 * 
 * The firmware can be built so that it writes copious trace and debug
 * data to the MCU's serial output by defining DEBUG_SERIAL. When the
 * Teensy reboots it switches its USB port to serial emulation and it
 * can take a few seconds for a connected host computer to recognise
 * the switch: - DEBUG_SERIAL_START_DELAY introduces a delay that can
 * be used to prevent loss of early debug output.
 */
#define DEBUG_SERIAL
#define DEBUG_SERIAL_START_DELAY 4000

/**********************************************************************
 * MCU EEPROM (PERSISTENT) STORAGE ADDRESSES
 * 
 * Module configuration is persisted to Teensy EEPROM storage and here
 * are the locations used by NOP100.
 */
#define SOURCE_ADDRESS_EEPROM_ADDRESS 0
#define MODULE_INSTANCE_EEPROM_ADDRESS 1
#define FIRST_AVAILABLE_APPLICATION_EEPROM_ADDRESS 2

/**********************************************************************
 * MCU PIN DEFINITIONS
 * 
 * GPIO pin definitions for the Teensy 3.2/4.0 MCU and some collections
 * that can be used as array initialisers
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

#define NMEA2000_SOURCE_ADDRESS_SEED 22     // Arbitrary seed value
#define NMEA2000_INSTANCE_UNDEFINED 255     // NMEA defines 255 as "undefined"
#define DEFAULT_SOURCE_ADDRESS NMEA2000_SOURCE_ADDRESS_SEED
#define DEFAULT_INSTANCE_ADDRESS NMEA2000_INSTANCE_UNDEFINED

#define TRANSMIT_LED_UPDATE_INTERVAL 100UL   // 10 times a second
#define NUMBER_OF_STATUS_LEDS 8
#define STATUS_LEDS_UPDATE_INTERVAL 200UL // 5 times a second
#define LONG_BUTTON_PRESS_INTERVAL 1000UL // 1 second
#define CONFIGURATION_INACTIVITY_TIMEOUT 30000UL // 30 seconds 

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
#include "module-directives.inc"
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

/**********************************************************************
 * Declarations of local functions.
 */
void messageHandler(const tN2kMsg&);
void updateTransmitLed(unsigned char status);
void updateStatusLeds(unsigned char status);
void prgButtonHandler(bool state, int value);
void configureModuleSettingMaybe(int value = 0xffff, bool longPress = false);
uint8_t getModuleSetting(int address);
void setModuleSetting(int address, uint8_t value);

ModuleConfiguration MODULE_CONFIGURATION(EEPROM.size(), EEPROM_CONFIGURATION_ADDRESS, configurationCallback);

/**********************************************************************
 * List of PGNs transmitted by this program.
 * 
 * PGN 127501 Binary Status Report.
 */
const unsigned long TransmitMessages[] = NMEA_TRANSMIT_MESSAGE_PGNS;

/**********************************************************************
 * NMEA2000Handlers -  vector mapping each PGN handled by this module
 * onto a function which will process any received messages.
 */
typedef struct { unsigned long PGN; void (*Handler)(const tN2kMsg &N2kMsg); } tNMEA2000Handler;
tNMEA2000Handler NMEA2000Handlers[] = NMEA_PGN_HANDLERS;

/**********************************************************************
 * PRG_BUTTON - debounced GPIO_PRG.
 */
Button PRG_BUTTON(GPIO_PRG);

/**********************************************************************
 * DIL_SWITCH - interface to the IC74HC165 IC that connects the eight
 * DIL switch parallel inputs.
 */
IC74HC165 DIL_SWITCH (GPIO_PISO_CLOCK, GPIO_PISO_DATA, GPIO_PISO_LATCH);


IC74HC595 STATUS_LEDS_SIPO(GPIO_SIPO_CLOCK, GPIO_SIPO_DATA, GPIO_SIPO_LATCH);

/**********************************************************************
 * Create handlers for the transmit LED (connected to a GPIO pin) and
 * the status LEDs connected to the SIPO. 
 */
StatusLeds TRANSMIT_LED(1, TRANSMIT_LED_UPDATE_INTERVAL, [](unsigned char status){ digitalWrite(GPIO_TRANSMIT_LED, (status & 0x01)); });
StatusLeds STATUS_LEDS(NUMBER_OF_STATUS_LEDS, STATUS_LEDS_UPDATE_INTERVAL, [](unsigned char status){ STATUS_LEDS_SIPO.writeByte(status); });

/**********************************************************************
 * MODULE_INSTANCE - working storage for the module instance number.
 * The user selected value is recovered from hardware and assigned
 * during module initialisation and reconfiguration.
 */
unsigned char MODULE_INSTANCE = DEFAULT_INSTANCE_ADDRESS;

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
#include "module-definitions.inc"
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

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
  PRG_BUTTON.begin();
  DIL_SWITCH.begin();
  pinMode(GPIO_TRANSMIT_LED, OUTPUT);
  STATUS_LEDS_SIPO.begin();

  // We assume that a new host system has its EEPROM initialised to all
  // 0xFF. We test by reading a byte that in a configured system should
  // never be this value and if it indicates a scratch system then we
  // set EEPROM memory up in the following way.
  //
  // Address | Value                                    | Size in bytes
  // --------+------------------------------------------+--------------
  // 0x00    | N2K source address                       | 1
  // 0x01    | N2K module instance number               | 1
  //
  //EEPROM.write(SOURCE_ADDRESS_EEPROM_ADDRESS, 0xff);
  
  if (EEPROM.read(SOURCE_ADDRESS_EEPROM_ADDRESS) == 0xff) {
    EEPROM.write(SOURCE_ADDRESS_EEPROM_ADDRESS, DEFAULT_SOURCE_ADDRESS);
    EEPROM.write(MODULE_INSTANCE_EEPROM_ADDRESS, DEFAULT_INSTANCE_ADDRESS);
  }
  MODULE_CONFIGURATIOM.setup();

  // Run a startup sequence in the LED display: all LEDs on to confirm
  // function, then a display of the module instance number.
  TRANSMIT_LED.setStatus(0xff); STATUS_LEDS.setStatus(0xff); delay(100);
  TRANSMIT_LED.setStatus(0x00); STATUS_LEDS.setStatus(MODULE_INSTANCE); delay(1000);
  TRANSMIT_LED.setStatus(0x00); STATUS_LEDS.setStatus(0x00);

  /*********************************************************************/
  /*********************************************************************/
  /*********************************************************************/
  #include "module-setup.inc"
  /*********************************************************************/
  /*********************************************************************/
  /*********************************************************************/

  // Initialise and start N2K services.
  NMEA2000.SetProductInformation(PRODUCT_SERIAL_CODE, PRODUCT_CODE, PRODUCT_TYPE, PRODUCT_FIRMWARE_VERSION, PRODUCT_VERSION);
  NMEA2000.SetDeviceInformation(DEVICE_UNIQUE_NUMBER, DEVICE_FUNCTION, DEVICE_CLASS, DEVICE_MANUFACTURER_CODE);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, EEPROM.read(SOURCE_ADDRESS_EEPROM_ADDRESS)); // Configure for sending and receiving.
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.ExtendTransmitMessages(TransmitMessages); // Tell library which PGNs we transmit
  NMEA2000.SetMsgHandler(messageHandler);
  NMEA2000.Open();

  #ifdef DEBUG_SERIAL
  Serial.println();
  Serial.println("Starting:");
  Serial.print("  N2K Source address is "); Serial.println(NMEA2000.GetN2kSource());
  #endif

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
  if (NMEA2000.ReadResetAddressChanged()) EEPROM.update(SOURCE_ADDRESS_EEPROM_ADDRESS, NMEA2000.GetN2kSource());

  /*********************************************************************/
  /*********************************************************************/
  /*********************************************************************/
  #include "module-loop.inc"
  /*********************************************************************/
  /*********************************************************************/
  /*********************************************************************/

  // Timeout any hung configuration interaction.
  MODULE_CONFIGURATION.interact();

  // If the PRG button has been operated, then call the button handler.
  if (PRG_BUTTON.toggled()) prgButtonHandler(PRG_BUTTON.read(), DIL_SWITCH.readByte());
  
  // Maybe update the transmit and status LEDs.
  TRANSMIT_LED.update(false, true);
  STATUS_LEDS.update(false, true);
}

void messageHandler(const tN2kMsg &N2kMsg) {
  int iHandler;
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  if (NMEA2000Handlers[iHandler].PGN != 0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
}

#ifndef PRG_BUTTON_HANDLER
/**********************************************************************
 * prgButtonHandler - handle a change of state on the PRG button which
 * now has the state indicated by <released> (where true says released)
 * and returns the value of DIL_SWITCH - positive if
 * the result of a short button press; negative if the result of a long
 * button press..
 * 
 * If the causal event was a button press, then a timer is started so
 * that the duration of the press can be measured. When the button is
 * released, the value of DIL_SWITCH is read and, if the causal button
 * press was long the value is incremented by 256. A call is then made
 * to the state machine's process() method with the value of the DIL
 * switch as argument. I 
 */
void prgButtonHandler(bool state, int value) {
  static unsigned long deadline = 0UL;
  unsigned long now = millis();

  switch (state) {
    case Button::RELEASED :
      if (MODULE_CONFIGURATION.interact(int value,  ((deadline) && (now > deadline)))) {
        TRANSMIT_LED.setState(0, LedStatus::flash);
      } else {
        TRANSMIT_LED.setState(0, LedStatus::off);
      }
      deadline = 0UL;
      break;
    case Button::PRESSED :
      deadline = (now + LONG_BUTTON_PRESS_INTERVAL);
      break;
  }
}
#endif

#ifndef CONFIGURATION_CALLBACK
void configurationCallback(unsigned int address, unsigned char value) {

}
#endif