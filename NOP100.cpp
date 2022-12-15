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
#include <ProcessQueue.h>
#include <arraymacros.h>

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
 * Module configuration is persisted to Teensy EEPROM storage.
 * 
 * SOURCE_ADDRESS_EEPROM_ADDRESS is the storage address for the
 * module's 1-byte N2K/CAN source address.
 */
#define SOURCE_ADDRESS_EEPROM_ADDRESS 0
#define INSTANCE_ADDRESS_EEPROM_ADDRESS 1

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
#define GPIO_OUTPUT_PINS { GPIO_SIPO_CLOCK, GPIO_SIPO_DATA, GPIO_SIPO_LATCH, GPIO_PISO_CLOCK, GPIO_PISO_LATCH, GPIO_POWER_LED, GPIO_TRANSMIT_LED }

#define DEFAULT_SOURCE_ADDRESS 22         // Seed value for source address claim
#define DEFAULT_INSTANCE_ADDRESS 255
#define TRANSMIT_LED_UPDATE_INTERVAL 50   // Frequency at which to update the transmit LED.

#include "module-deviceinfo.defs"
#include "module-productinfo.defs"

/**********************************************************************
 * Declarations of local functions.
 */
void messageHandler(const tN2kMsg&);
void flashTransmitLedMaybe();
void processPrgButtonPress();
uint8_t getTransmitLedStatus();

/**********************************************************************
 * List of PGNs transmitted by this program.
 * 
 * PGN 127501 Binary Status Report.
 */
const unsigned long TransmitMessages[] PROGMEM={ 0 };

/**********************************************************************
 * NMEA2000Handlers -  vector mapping each PGN handled by this module
 * onto a function which will process any received messages.
 */
typedef struct { unsigned long PGN; void (*Handler)(const tN2kMsg &N2kMsg); } tNMEA2000Handler;
tNMEA2000Handler NMEA2000Handlers[]={ {0L, 0} };

/**********************************************************************
 * PRG_BUTTON - debounced GPIO_PRG.
 */
Button PRG_BUTTON(GPIO_PRG);

/**********************************************************************
 * TRANSMIT_LED_STATE - holds the state that should be assigned to the
 * GPIO_TRANSMIT_LED pin the next time its output is updated (the value
 * will be reset to 0 after each update). 
 */
int TRANSMIT_LED_STATE = 0;

/**********************************************************************
 * DIL_SWITCH - interface to the IC74HC165 IC that connects the eight
 * DIL switch parallel inputs.
 */
IC74HC165 DIL_SWITCH (GPIO_PISO_CLOCK, GPIO_PISO_DATA, GPIO_PISO_LATCH);

/**********************************************************************
 * MODULE_INSTANCE - working storage for the module instance number.
 * The user selected value is recovered from hardware and assigned
 * during module initialisation and reconfiguration.
 */
unsigned char MODULE_INSTANCE = INSTANCE_UNDEFINED;

/**********************************************************************
 * MAIN PROGRAM - setup()
 */
void setup() {
  #ifdef DEBUG_SERIAL
  Serial.begin(9600);
  delay(DEBUG_SERIAL_START_DELAY);
  #endif

  // Initialise all core GPIO pins.
  int opins[] = GPIO_OUTPUT_PINS;
  for (unsigned int i = 0 ; i < ELEMENTCOUNT(opins); i++) pinMode(opins[i], OUTPUT);
  PRG_BUTTON.begin();
  DIL_SWITCH.begin();

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
    EEPROM.write(INSTANCE_ADDRESS_EEPROM_ADDRESS, DEFAULT_INSTANCE_ADDRESS);
  }

  // If this module requires a instance numberRecover module instance number.
  MODULE_INSTANCE = EEPROM.read(INSTANCE_ADDRESS_EEPROM_ADDRESS);

  // Run a startup sequence in the LED display: all LEDs on to confirm
  // function, then a display of the module instance number.
  LED_STATUS_DISPLAY.writeByte(0xff); delay(100);
  LED_STATUS_DISPLAY.writeByte(MODULE_INSTANCE); delay(1000);
  LED_STATUS_DISPLAY.writeByte(0x00);

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
  Serial.print("  Module instance number is "); Serial.println(MODULE_INSTANCE);
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

  // If the PRG button has been operated, then update module instance.
  if (PRG_BUTTON.released()) processPrgButtonPress();

  flashTransmitLedMaybe();
}

void flashTransmitLedMaybe() {
  static unsigned long deadline = 0UL;
  unsigned long now = millis();

  if (now > deadline) {
    digitalWrite(GPIO_TRANSMIT_LED, TRANSMIT_LED_STATE); TRANSMIT_LED_STATE = 0;
    deadline = (now + TRANSMIT_LED_UPDATE_INTERVAL);
  }
}

void messageHandler(const tN2kMsg &N2kMsg) {
  int iHandler;
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  if (NMEA2000Handlers[iHandler].PGN != 0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
}

void processPrgButtonPress() {
  EEPROM.write(INSTANCE_ADDRESS_EEPROM_ADDRESS, DIL_SWITCH.readByte());
  MODULE_INSTANCE = EEPROM.read(INSTANCE_ADDRESS_EEPROM_ADDRESS);
  LED_STATUS_DISPLAY.writeByte(MODULE_INSTANCE); delay(1000);
}
