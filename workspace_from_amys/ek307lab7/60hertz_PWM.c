//EC583 Final Project
//jeffrey kittredge, may 2, 2013
//Gate driving outputs to create a 60Hz Sine wave using H-Bridge
//These two outputs directly drive the FET gates after being boosted by gate drivers

/*
C-A-U-T-I-O-N
1- This code is not fail safe.  If microprosser is unplugged while H-bridge is functioning;
the state of the gates becomes unknown and two FETs on the same side of the H-bridge can be
on at once.
2- DO NOT UNPLUG MICROCONTROLLER WHILE H-BRIDGE IS ACTIVE.
*/

/*
 * ----Output assignment----
 * Pin 1.4 (P1OUT 0x10) = Gate driver signal for "Bottom Left" & "Top Right" FETs
 * Pin 1.5 (P1OUT 0x20) = Gate driver signal for "Bottom Right" & "Top Left" FETs
 *
 */


#include <msp430g2211.h>

unsigned char counter = 0;//counter is duration of one period, EG counter will always go 0 to 17
unsigned char index=0;//select the current pulse width from an array of pulse widths.

//Array of pulse widths.
const unsigned char values[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

//define the clock registers
const unsigned short CALDCO_16MHZ = 0x10F8;
const unsigned short CALBC1_16MHZ = 0x10F9;

void main(void) {
	// Configure the on-chip oscillators to run at 16 MHz, fastest possible
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL  = CALDCO_16MHZ;
	//stop the watch dog
	WDTCTL = WDTPW + WDTHOLD;

	// time between interrupts = (1/16000000) * TACCR0
	TACTL = TASSEL_2 + ID_0 + MC_1;
	TACCR0 = 100;    //call the interupt once every 100 clock cycles
	TACCTL0 = CCIE;

	P1SEL = 0;
	P1OUT = 0;
	P1DIR = 255;

	_bis_SR_register(GIE+LPM0_bits);
}

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR(void) {
	if (counter >= 17) {//represents the constant width of a PWM cycle
		counter = 0;  // Reset the counter

		if (index !=31) {//if not at last index go to next index
			++index;}
		else {
			index = 0;}
	}//if at last index, go back to first
	else if ( counter >= values[index]){
		P1OUT |= 0x20;//set p1.5 on
		P1OUT &= ~0x10;// set p1.4 off
	}

	else {
		P1OUT &= ~0x20;//set p1.5 off
		P1OUT |= 0x10;//set p1.4 on
	}

++counter; // increment the counter
}

//if we get in a watchdog, disable it
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void) {
    IE1 &= ~WDTIE;
    IFG1 &= ~WDTIFG;
    WDTCTL = WDTPW + WDTHOLD;
}
