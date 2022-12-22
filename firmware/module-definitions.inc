/**********************************************************************
 * module-definitions.inc
 *
 * Contains #defines for the constant values which characterise a
 * particular NMEA module.
 *
 * For the NOP100 module these definitions are as vanilla / null as
 * they can be and some, perhaps all, will need to be tweaked to
 * support a real application. Additionally, definitions and function
 * declarations which are specific to the application must be appended
 * to this minimal set.
 */
  
#include <ArduinoQueue.h>

 /**********************************************************************
 * DEVICE INFORMATION
 * 
 * Because of NMEA's closed standard, most of this is fiction. Maybe it
 * can be made better with more research.
 *
 * DEVICE_CLASS and DEVICE_FUNCTION are explained in the document
 * "NMEA 2000 Appendix B.6 Class & Function Codes".
 * 
 * INDUSTRY_GROUP we can be confident about (4 says maritime).
 * 
 * MANUFACTURER_CODE is only allocated to subscribed NMEA members and,
 * unsurprisingly, an anonymous code has not been assigned: 2046 is
 * currently unused, so we adopt that.  
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


/**********************************************************************
 * PRODUCT INFORMATIOM
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

/**********************************************************************
 * NMEA2000 LIBRARY DEFINITIONS
 *
 * NMEA_TRANSMIT_MESSAGE_PGNS is an initialiser for an array of PGNs
 * which defines the messages transmitted by this firmware. The list
 * must terminate with a zero value.
 *
 * NMEA_PGN_HANDLERS is an initialiser for a jump vector which defines
 * a list of pairs each of which maps a PGN to the function responsible
 * for processing messages of the defined type when they are received
 * over the CAN interface. For example, { 127501, myFunc }. The list
 * must terminate with the special flag value { 0, 0 }.
 */
#define NMEA_TRANSMIT_MESSAGE_PGNS { 0 }
#define NMEA_PGN_HANDLERS  { { 0L, 0 } }
