/*
 * clockConfig.c
 *
 *  Created on: Jan 15, 2014
 *      Author: Jeff
 */

#include "clockConfig.h"

//configure the clock

void clockConfig( void){
	BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
	DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz
}




