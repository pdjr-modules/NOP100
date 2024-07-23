/**
 * @file setup.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Code to be executed in Arduino setup().
 * @version 0.1
 * @date 2024-07-01
 * @copyright Copyright (c) 2024
 */

SPI.begin();

MikrobusSwitchInputs.configureCallback(updateSwitchbankStatus, SWITCHBANK_UPDATE_INTERVAL);

N2kResetBinaryStatus(SwitchbankStatus);