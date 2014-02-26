/*
 * ascii2dec.h
 *
 *  Created on: Jan 14, 2014
 *      Author: Jeff
 */

#ifndef ASCII2DEC_H_
#define ASCII2DEC_H_

#include <stdint.h>
#include <math.h>

uint8_t asciiByteToDecByte( uint8_t asciiByte);
/*takes a single byte representing a value off the ascii chart and converts it to a decimal value
 * only valid for the digit 0-9 and letters A-F
 * EG the ASCII decimal 54 (the digit 6") will be produce the output "6"
 */

#endif /* ASCII2DEC_H_ */
