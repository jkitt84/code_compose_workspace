/*
 * ascii2dec.c
 *
 *  Created on: Jan 14, 2014
 *      Author: Jeff
 */
#include "ascii2dec.h"

uint8_t asciiByteToDecByte( uint8_t asciiByte){
	if ( asciiByte <= '9' && asciiByte >= '0')
		return (asciiByte - '0');
	else if ( asciiByte <= 'F' && asciiByte >= 'A')
		return (asciiByte - 'A' + 10);
	else
		return 0; //FIXME need to add exception here
}



