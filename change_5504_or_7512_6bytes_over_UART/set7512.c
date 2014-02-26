/*
 * set7512.c
 *
 *  Created on: Jan 16, 2014
 *      Author: Jeff
 */


#include "set7512.h"
//writes a value to the dac7512 using UCB0 and p1.3 as the chip select
void set7512( uint16_t dataOut)
{
	UCB0CTL0 &= ~UCCKPH; //the 7512 needs CKPH cleared
 while (!( IFG2 && UCB0TXIFG)); //originally this was only &
 P1OUT &= ~BIT3; //lower the chip select for 7512 (make active)
 __delay_cycles(10);
 UCB0TXBUF = ( dataOut >> 8 );  // the Most Significant byte
 __delay_cycles(120);
 while (!( IFG2 && UCB0TXIFG)); //originally this was only &
 UCB0TXBUF = dataOut; // the least significant byte
 __delay_cycles(120);
 P1OUT |= BIT3; //raise the chip select for 7512 (make inactive)
}
