/*
 * set5504.h
 *
 *  Created on: Jan 16, 2014
 *      Author: Jeff
 */

#ifndef SET5504_H_
#define SET5504_H_

#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdint.h>

// set analog devices AD5504 via SPI on UCB0 with ~CS on p1.4
void set5504( uint16_t dataOut);


#endif /* SET5504_H_ */
