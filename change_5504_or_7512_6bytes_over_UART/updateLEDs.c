/*
 * updateLEDs.c
 *
 *  Created on: Jan 15, 2014
 *      Author: Jeff
 */
// updates the LEDs based on the contents of twoBytes

#include "updateLEDs.h"

void updateLEDs( uint16_t twoBytes ){
	switch( twoBytes){
		case 0x0001: { P1OUT |= BIT0; }// "R" turn Red LED on
	 	 	break;
		case 0x3A45: { P1OUT &= ~BIT0; }// "r" turn Red LED off
			break;
		case 0xFF00: { P1OUT |= BIT6; }// "G" turn Green LED on
			break;
		case 0x0101: { P1OUT &= ~BIT6; }// "g" turn Green LED off
			break;
		default:
			;//should put something here
	 		break;
	 }
}





