
/*
 *Configure USCI to communicate with DAC7512 on UCB0
 *Pin 1.5 = Clock
 *Pin 1.7 = Data
 *Pin 1.4 = ~Chip Select *not good, need to change
 * main.c
 */


#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdint.h>

uint16_t TXdata1 = 0x080F;


void main(void) {
//not Chris
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_16MHZ;               		// 16MHz clock
	DCOCTL  = CALDCO_16MHZ;

//clean up port 2 because it is unused
//Select = 0 (digital I/O selected), Dir =0 (inputs), enable the pull down (P2OUT=0 ???) on the unused outputs and crystal pins
	P2SEL = 0;
	P2DIR = 0;
	P2REN = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
//USCI fix these, DEC 28, 2013 ??? will override these settings for SCLK, SDI and SDO (but not P1.4)
	P1OUT = BIT4;
	P1DIR = BIT4;//only P1.4 will be the SlaveSelect output handled by software
	P1REN = BIT0 | BIT1 | BIT2 | BIT3 ; //pull the resistors on the unused pins

//CHRIS
	// Disable USCI Peripheral and clocked from SMCLK
	UCB0CTL1 = UCSWRST | UCSSEL_2;
	// 8 bit (default), 3-wire (default, mode 0), synchronous, MSB first. MASTER MODE
	UCB0CTL0 = UCMSB | UCMST | UCSYNC | UCMODE_0;

	// Setup output pins
	//jpk not really needed P1OUT  |= (BIT5 | BIT7);            // SPI output pins low
	P1SEL  |= BIT5 | BIT6 | BIT7;	    //jpkBit6 not needed // configure P1.5, P1.6, P1.7 for USCI
	P1SEL2 |= BIT5 | BIT6 | BIT7;       //jpkBit6 not needed // more required SPI configuration

	//bit rate control
	// Default to SMCLK/16
	UCB0BR1 = 0;	//upper byte
	UCB0BR0 = 60;   //lower byte devide SMCLK by 60

	UCB0CTL1 &= ~UCSWRST; // Enable SPI peripheral
	



}
