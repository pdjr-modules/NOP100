/**
 * @file definitions.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Everything required to implement NOP100-ROM.
 * @version 0.1
 * @date 2024-07-16
 * @copyright Copyright (c) 2024
 */

/**
 * @brief Scheduler instance to manage transmission of PGN 127501.
 */
tN2kSyncScheduler PGN127501Scheduler;

/**
 * @brief Interface to Click 5981 MikroBus modules
 */

MIKROE5675::tPins MikroBusConfiguration[3] = MIKROBUS_CONFIGURATION;
MIKROE5675 MikrobusRelayOutputs (MikroBusConfiguration);

/**
 * @brief Buffer holding current input channel states.
 * 
 * We choose the tN2kBinaryStatus type rather than any alternate
 * representation because this can then be used without further
 * processing in a PGN 127501 message.
 * 
 * The buffer is updated directly each time the Click 5981 modules
 * are polled for their channel states.
 */
tN2kBinaryStatus SwitchbankStatus;

/**********************************************************************
 * Process a received PGN 127502 Switch Bank Control message by
 * decoding the switchbank status message and comparing the requested
 * channel state(s) with the current SwitchbankStatus. Any mismatch 
 * results in a call to update the appropriate MikroE 5675 module to
 * reflect the state commanded by the received PGN.
 */
void handlePGN127502(const tN2kMsg &n2kMsg) {
  uint8_t instance;
  uint8_t index;
  uint8_t statusByte;
  bool changed;
  tN2kBinaryStatus commandedSwitchbankStatus;
  tN2kOnOff commandedChannelStatus;

  // retrieve target instance and switchbank status
  if (ParseN2kPGN127501(n2kMsg, instance, commandedSwitchbankStatus)) {
    // if we are the target instance
    if (instance == (unsigned char) CodeSwitchPISO.read()) {
      // iterate over configured relay modules
      for (unsigned int m = 0; m < RelayOutputModule.getModuleCount(); m++) {
        statusByte = 0; changed = false;
        // iterate over relay channels
        for (unsigned int c = 0; c < MIKROE5675::CHANNEL_COUNT; c++) {
          // compute index into switchbank status structure
          index = (m * MIKROE5675::CHANNEL_COUNT) + c + 1;
          // recover commanded state of channel at index
          commandedChannelStatus = N2kGetStatusOnBinaryStatus(commandedSwitchbankStatus, index);
          // Make sure commanded state is valid
          if ((commandedChannelStatus == N2kOnOff_On) || (commandedChannelStatus == N2kOnOff_Off)) { 
            // Make a status byte equivalent to the commanded status for this relay module
            statusByte = (statusByte << 1) | ((commandedChannelStatus == N2kOnOff_On)?1:0);
            // Flag if the commanded state is different to the current state       
            changed = changed || (commandedChannelStatus != N2kGetStatusOnBinaryStatus(SwitchbankStatus, index));
          }
        }
        // If flagged then update the current module to the commanded state
        if (changed) RelayOutputModule.setRelayStatus(m, statusByte);
      }
    }
  }
}

/**
 * @brief Transmit PGN 127501 and flash transmit LED.
 * 
 * Recover module instance address from hardware switch settings and
 * create and transmit an NMEA 2000 message from the value of
 * SwitchbankStatus. 
 */
void transmitPGN127501() {
  #ifdef DEBUG_SERIAL
  Serial.println("transmitPGN127501()...");
  #endif
  static tN2kMsg N2kMsg;

  // Recover module instance address from the hardware code switches.
  unsigned char instance = (unsigned char) CodeSwitchPISO.read();

  if (instance != 255) {
    SetN2kPGN127501(N2kMsg, instance, SwitchbankStatus);
    NMEA2000.SendMsg(N2kMsg);
    CanLed.setLedState(0, LedManager::ONCE);
  }
}  

/**********************************************************************
 * @brief Record switch channel input states and respond to any state
 * changes.
 * 
 * If a channel has changed state then the value of SwitchbankStatus
 * is updated and a call is made to immediately transmit the update
 * over NMEA.
 * 
 * This function is intended to operate as a callback method for
 * IC74HC165.
 * 
 * @param status - current status of modules switch input channels.
 */
void updateSwitchbankStatus(uint32_t status) {
  bool updated = false;
  int state;

  #ifdef DEBUG_SERIAL
  Serial.print("processSwitchInputs("); Serial.println(")...");
  #endif

  for (unsigned int i = 0; i < (MIKROE5675::CHANNEL_COUNT * MikrobusRelayOutputs.getModuleCount()); i++) {
    state = (status >> i) && 1;
    if (state != ((N2kGetStatusOnBinaryStatus(SwitchbankStatus, (i + 1)) == N2kOnOff_On)?1:0)) {
      N2kSetStatusBinaryOnStatus(SwitchbankStatus, (state)?N2kOnOff_On:N2kOnOff_Off, (i + 1));
      updated = true;
    }
  }
  if (updated) transmitPGN127501();
}

///////////////////////////////////////////////////////////////////////
// The following functions override the defaults provided in NOP100. //
///////////////////////////////////////////////////////////////////////

/**
 * @brief Callback invoked when N2K bus connection becomes active.
 * 
 * Start the scheduler used for transmission of PGN 127501 supplying
 * configuration data from the module configuration.
 * 
 * @note Overrides the eponymous function in NOP100.
 */
void onN2kOpen() {
  #ifdef DEBUG_SERIAL
  Serial.println("OnN2kOpen()...");
  #endif

  PGN127501Scheduler.SetPeriodAndOffset(
    (uint32_t) (ModuleConfiguration.getByte(MODULE_CONFIGURATION_PGN127501_TRANSMIT_PERIOD_INDEX) * 1000),
    (uint32_t) (ModuleConfiguration.getByte(MODULE_CONFIGURATION_PGN127501_TRANSMIT_OFFSET_INDEX) * 10)
  );
}

/**
 * @brief ModuleConfiguration callback invoked to validate proposed
 * changes to the module configuration.
 * 
 * @note Overrides the eponymous function in NOP100.
 */
bool configurationValidator(unsigned int index, unsigned char value) {
  #ifdef DEBUG_SERIAL
  Serial.print("configurationValidator("); Serial.print(index); Serial.print(", "); Serial.print(value); Serial.println(")...");
  #endif
  
  switch (index) {
    case MODULE_CONFIGURATION_CAN_SOURCE_INDEX:
      return(true);
    case MODULE_CONFIGURATION_PGN127501_TRANSMIT_PERIOD_INDEX:
      return(true);
      break;
    case MODULE_CONFIGURATION_PGN127501_TRANSMIT_OFFSET_INDEX:
      return(true);
      break;
    default:
      return(false);
      break;
  }
}
