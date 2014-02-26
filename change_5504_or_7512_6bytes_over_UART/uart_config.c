/*
 * uart_config.c
 *
 *  Created on: Jan 14, 2014
 *      Author: Jeff
 */
//sets up the UART settings on UCA0
#include "uart_config.h"


void uartConfig( void){

	/* Configure hardware UART */
	 UCA0CTL0 |= UCMSB; //MSB first, to be consistent with SPI
	 UCA0CTL1 |= UCSSEL_2; // Use SMCLK
	 UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
	 UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
	 UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
	 UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
	 IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}



