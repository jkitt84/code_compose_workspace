/*
 * set5504.c
 *
 *  Created on: Jan 16, 2014
 *      Author: Jeff
 */

#include "set5504.h"

// set analog devices AD5504 via SPI on UCB0 with ~CS on p1.4
void set5504( uint16_t dataOut){
	UCB0CTL0 |= UCCKPH; //the 5504 needs CKPH active

	 while (!( IFG2 && UCB0TXIFG)); //originally this was only &
	 P1OUT &= ~BIT4; //lower the chip select for 5504 (make active)
	 __delay_cycles(10);
	 UCB0TXBUF = ( dataOut >> 8 );  // the Most Significant byte
	 __delay_cycles(120);
	 while (!( IFG2 && UCB0TXIFG)); //originally this was only &
	 UCB0TXBUF = dataOut; // the least significant byte
	 __delay_cycles(120);
	 P1OUT |= BIT4; //raise the chip select for 5504 (make inactive)
}



