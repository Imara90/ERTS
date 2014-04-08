/***************************************************************/
// Definitions.h
// Author: Daniel Lemus 870754
/***************************************************************/

#ifndef _Definitions_h
#define _Definitions_h

//VARIABLE TYPES
#define BYTE unsigned char
#define WORD unsigned short

//VARIABLE LIMITS
#define INT8_MAX					127
#ifndef INT8_MIN
#define INT8_MIN					-127
#endif
#define INT16_MAX					32767
#define INT16_MIN					-32768


//MODES LIST
#define MODE_SAFE				0x00
#define MODE_PANIC				0x01
#define MODE_MANUAL				0x02
#define MODE_CALIBRATION		0x03
#define MODE_YAW_CONTROL		0x04
#define MODE_FULL_CONTROL		0x05
#define MODE_P					0x06
#define MODE_ABORT			0x07

//PACKAGE DEF
#define HEADER_LENGTH			0x01
#define MODE_LENGTH				0x01
#define PARAM_LENGTH			0x04
#define CHKSUM_LENGTH			0X01
#define PKGLEN					HEADER_LENGTH + MODE_LENGTH + PARAM_LENGTH + CHKSUM_LENGTH
#define HEADER_POS				0x00
#define MODE_POS				HEADER_LENGTH
#define DATA_POS				HEADER_LENGTH+MODE_LENGTH
#define CHKSUM_POS				HEADER_LENGTH+MODE_LENGTH+PARAM_LENGTH


#endif
