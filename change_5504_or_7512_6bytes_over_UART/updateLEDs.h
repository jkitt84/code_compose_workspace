/*
 * updateLEDs.h
 *
 *  Created on: Jan 15, 2014
 *      Author: Jeff
 */

#ifndef UPDATELEDS_H_
#define UPDATELEDS_H_

#include "msp430g2553.h"
#include <stdint.h>

void updateLEDs( uint16_t twoBytes );
/* updates the LEDs based on the contents of twoBytes
 * 0x0001 = RED on 			= 1
 * 0x3A45 = red off			= 14917
 * 0xFF00 = GREEN on		= 65280
 * 0x0101 = green off		= 257
 */


#endif /* UPDATELEDS_H_ */
