/**
 * @file defines.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief Defines for a switch input module based on Click 5981 modules.
 * @version 0.1
 * @date 2024-07-01
 * @copyright Copyright (c) 2024
 */

/**********************************************************************
 * @brief Specify the MikroBus sockets occupied by Click 5891 modules.
 * 
 * Options are MIKROBUS_SOCKET_LEFT, MIKROBUS_SOCKET_RIGHT or
 * MIKROBUS_SOCKET_LEFT_AND_RIGHT.
 */
#define MIKROBUS_SOCKET_LEFT_AND_RIGHT  // Both sockets

/**********************************************************************
 * @brief NMEA2000 device information overrides.
 */
#define DEVICE_CLASS 30                 // Electrical Distribution
#define DEVICE_FUNCTION 130             // Binary Event Monitor
#define DEVICE_UNIQUE_NUMBER 108        // Bump me?

/**********************************************************************
 * @brief NMEA2000 product information overrides.
 */
#define PRODUCT_CODE 100
#define PRODUCT_FIRMWARE_VERSION "240701"
#define PRODUCT_LEN 1
#define PRODUCT_SERIAL_CODE "100-108"   // PRODUCT_CODE + DEVICE_UNIQUE_NUMBER
#define PRODUCT_TYPE "NOP100-SIM"           // The product name?
#define PRODUCT_VERSION "240701 (Jul 2024)"

/**********************************************************************
 * @brief NMEA2000 transmit and receive PGN overrides.
 */
#define NMEA_TRANSMITTED_PGNS { 127501L, 0 }

/**********************************************************************
 * @brief ModuleConfiguration library stuff.
 */
#define MODULE_CONFIGURATION_SIZE 3                               // Total configuration size in bytes

#define MODULE_CONFIGURATION_PGN127501_TRANSMIT_PERIOD_INDEX 1    // Index of PGN 127501 transmit period in seconds
#define MODULE_CONFIGURATION_PGN127501_TRANSMIT_OFFSET_INDEX 2    // Index of PGN 127501 transmit offset in 10s of milli-seconds

#define MODULE_CONFIGURATION_TRANSMIT_PERIOD_DEFAULT 0x02         // Every two seconds
#define MODULE_CONFIGURATION_TRANSMIT_OFFSET_DEFAULT 0x00         // Zero times 10 milliseconds

#define MODULE_CONFIGURATION_DEFAULT { \
  MODULE_CONFIGURATION_CAN_SOURCE_DEFAULT, \
  MODULE_CONFIGURATION_TRANSMIT_PERIOD_DEFAULT, \
  MODULE_CONFIGURATION_TRANSMIT_OFFSET_DEFAULT \
}

/**********************************************************************
 * @brief NOP100 function overrides.
 */
#define CONFIGURATION_VALIDATOR
#define ON_N2K_OPEN

/**********************************************************************
 * @brief Configuration of attached Click 5981 modules.
 */
#define MIKROE5981_MODULE_0 { GPIO_MIKROBUS_MODULE0_CS, GPIO_MIKROBUS_MODULE0_EN, GPIO_MIKROBUS_MODULE0_INT, GPIO_MIKROBUS_RST, GPIO_MIKROBUS_MODULE0_PWM }
#define MIKROE5981_MODULE_1 { GPIO_MIKROBUS_MODULE1_CS, GPIO_MIKROBUS_MODULE1_EN, GPIO_MIKROBUS_MODULE1_INT, GPIO_MIKROBUS_RST, GPIO_MIKROBUS_MODULE1_PWM }

#ifdef MIKROBUS_SOCKET_LEFT
#define MIKROBUS_CONFIGURATION { MIKROE5981_MODULE_0, { 0,0,0,0,0 } }
#endif

#ifdef MIKROBUS_SOCKET_RIGHT
#define MIKROBUS_CONFIGURATION { MIKROE5981_MODULE_1, { 0,0,0,0,0 } }
#endif

#ifdef MIKROBUS_SOCKET_LEFT_AND_RIGHT
#define MIKROBUS_CONFIGURATION { MIKROE5981_MODULE_0, MIKROE5981_MODULE_1, { 0,0,0,0,0 } }
#endif

/**********************************************************************
 * @brief number of milliseconds between checks on switch inputs and
 * consequent update of switchbank state.
 */
#define SWITCHBANK_UPDATE_INTERVAL 100
