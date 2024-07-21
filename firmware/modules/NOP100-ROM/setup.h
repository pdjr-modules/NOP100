/**
 * @file setup.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Code to be executed in Arduino setup().
 * @version 0.1
 * @date 2023-01-16
 * @copyright Copyright (c) 2023
 */

Wire.begin();

MikrobusRelayOutputs.configureCallback(updateSwitchbankStatus, SWITCHBANK_UPDATE_INTERVAL);
N2kResetBinaryStatus(SwitchbankStatus);