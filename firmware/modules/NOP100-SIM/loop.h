/**
 * @file loop.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Code to be executed in Arduino loop().
 * @version 0.1
 * @date 2023-01-16
 * @copyright Copyright (c) 2023
 */

CodeSwitchPISO.callbackMaybe();

if (PGN127501Scheduler.IsTime()) { PGN127501Scheduler.UpdateNextTime(); transmitPGN127501(); }
