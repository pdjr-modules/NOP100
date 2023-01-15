/**
 * @file defines.h
 * @author Paul Reeve (preeve@pdjr.eu)
 * @brief #define directives.
 * 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 */

/**
 * @brief Device information required by the NMEA2000 library.
 * 
 * NMEA's closed standard, means that most of this is fiction. Maybe it
 * can be made better with more research.
 *
 * DEVICE_CLASS and DEVICE_FUNCTION are explained in the document
 * "NMEA 2000 Appendix B.6 Class & Function Codes".
 * 
 * INDUSTRY_GROUP we can be confident about (4 says maritime).
 * 
 * MANUFACTURER_CODE is only allocated to subscribed NMEA members so we
 * grub around and use 2046 which is currently not allocated.  
 * 
 * UNIQUE_NUMBER is combined in some way so that together they define
 * a value which must be unique (is some way) on any N2K bus. An easy
 * way to achieve this is just to bump the unique number for every
 * software build. Really this needs automating.
 */
#define DEVICE_CLASS 10                 // System Tools
#define DEVICE_FUNCTION 130             // Diagnostic
#define DEVICE_INDUSTRY_GROUP 4         // Maritime
#define DEVICE_MANUFACTURER_CODE 2046   // Currently not allocated.
#define DEVICE_UNIQUE_NUMBER 849        // Bump me?

/**
 * @brief Product information required by the NMEA2000 library.
 * 
 * This poorly structured set of values is what NMEA expects a product
 * description to be shoe-horned into.
 */
#define PRODUCT_CERTIFICATION_LEVEL 1
#define PRODUCT_CODE 002
#define PRODUCT_FIRMWARE_VERSION "1.1.0 (Jun 2022)"
#define PRODUCT_LEN 1
#define PRODUCT_N2K_VERSION 2022        // The N2K specification version?
#define PRODUCT_SERIAL_CODE "002-849"   // PRODUCT_CODE + DEVICE_UNIQUE_NUMBER
#define PRODUCT_TYPE "SIM108"           // The product name?
#define PRODUCT_VERSION "1.0 (Mar 2022)"

/**
 * @brief Zero terminated list of PGNs transmitted by this firmware
 *        (required by the NMEA2000 library).
 */
#define NMEA_TRANSMIT_MESSAGE_PGNS { 0L }

/**
 * @brief Vector of PGNs handled by this application and the callback
 *        functions that process them (required by the NMEA2000
 *        library). 
 * 
 * Each entry is a pair { *pgn*, *callback* }, for example
 * { 127501L, handlerForPgn127501 }, and the list must terminate with
 * the special flag value { 0L, 0 }.
 */
#define NMEA_PGN_HANDLERS  { { 0L, 0 } }
