/*
 * pinConfig.c
 *
 *  Created on: Jan 14, 2014
 *      Author: Jeff
 */

#include "pinConfig.h"

//sets up all the pins
void pinConfig( void){

	// set the pin mode 3 for pins 5  6 and 7 of port 1 (USCI mode), ???6 is not really needed
	P1SEL = BIT1 + BIT2 + BIT5 + BIT7; // low bit = 1 for pin 1 and 2 4. BIT 3 is 0 (CS via GPIO), P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 + BIT5 + BIT7; // high bit = 1 for pin 1 and 2 4 BIT 3 is 0 (CS via GPIO), P1.1 = RXD, P1.2=TXD
	//jan1 removed BIT6 from both P1SEL and SEL2

	P1DIR = BIT4 + BIT3 + BIT0 + BIT6; // p1.3 set to output to drive CS for the 7512
	P1OUT |= BIT4 + BIT3 + BIT0; //jpk_Set p1.3 high for inactive start // pull p1.3 to low - CS low



}




