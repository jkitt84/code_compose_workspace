/*
 * set7512.h
 *
 *  Created on: Jan 16, 2014
 *      Author: Jeff
 */

#ifndef SET7512_H_
#define SET7512_H_

#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdint.h>

// set texas instruments DAC7512 via SPI on UCB0 with ~CS on p1.3
void set7512( uint16_t dataOut);

#endif /* SET7512_H_ */
