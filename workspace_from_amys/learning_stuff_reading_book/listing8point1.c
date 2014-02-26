/*
 * LIsting 8.1
 * Demonstrate the WatchDogTimer
 * Button must be pressed every 1 S of the LED will go on indicating a Watch Dog Occurred
 */
//--------------------------------------------------------------
/*
#include <msp430g2452.h>
//--------------------------------------------------------------
//pins for LED and the button
#define LED1 		BIT0
#define LED2 		BIT6
#define B1			BIT3
// Watch dog configuration: active, ACLK/32768 -> 1s interval; clear counter
#define WDTCONFIG 	(WDTCNTCL | WDTSSEL)//and leave WDTISx as default for longest period
//include settings for _RST/NMI pin here as well

//temp variable for testing the state of IFG1 bit 0 (the WDTIFG)
volatile char isFlagged = 0;//is the WDTIFG active?
volatile char isButton = 0;//is the button depressed?


void main (void){
	WDTCTL = WDTPW | WDTCONFIG;
	P1OUT = 0x00;
	P1DIR = LED1 | LED2;
	for ( ; ; ){
		//test the status of IFT1 bit 0 (the WDTIFG) and set LED2 accordingly
		if ( (IFG1 & 0x01) ==1 ){
			isFlagged = 0x01;
			P1OUT |= LED2;//if the watchdog flag is set, turn the LED on
		} else {
			isFlagged = 0x00;
			P1OUT &= ~LED2;//if the watchdog is NOT set, turn the LED off
		}
		//test the status of the button
		if ( (P1IN & B1) == 1){//button depressed
			isButton = 1;
		} else {
			isButton = 0;
		}

		if ( isButton == 1){
			P1OUT |= LED1;//if the button is depressed, turn LED1 on
			WDTCTL = WDTPW | WDTCONFIG;
		} else{
			P1OUT &= ~LED1;
		}


	}
}
*/
