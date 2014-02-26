/*
 * spiConfig.c
 *
 *  Created on: Jan 16, 2014
 *      Author: Jeff
 */

//configures the SPI bus to run the DAC7512 on UCB0

#include "spiConfig.h"

void spiConfig( void){
	 UCB0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCMSB;
	//jpk_removed_Dec_29     UCCKPL+
	 // synchronous (=SPI) master 3 wire SPI, clock polarity High
	/*SPI mode is selected when the UCSYNC bit is set and SPI
	mode (3-pin or 4-pin) is selected with the UCMODEx bits.*/
	 //use SCLK : 4MHz (MCP4131 supports up to 10MHz write via SPI)
	 UCB0CTL1 |= UCSSEL_2;
	 // set baud rate = SMCLK, no further division
	 UCB0BR0 = 10;
	 UCB0BR1 = 0;
	 UCB0CTL1 &= ~UCSWRST; // **Initialize USCI **
}

