/*
 * SPI bus to set outputs on Analog Devices TI DAC7512
 *
 */
#include <msp430g2452.h>
#include <intrinsics.h>
#include <stdint.h>

//follow the standard data sheet for the MSP you are using for SDO (pin 1.6) and SCLK (pin 1.5)
//put SS (slave select) on pin 1.4

uint16_t TXdata1 = 0x080F;

//not using these right now.
//uint8_t whichOne = 0;

void main(void) {
	BCSCTL1 = CALBC1_16MHZ;               		// 16MHz clock
	DCOCTL  = CALDCO_16MHZ;

//watch dog as interval timer, clear, SMCLK/ 8192 = 1.5kHz (0.7mS)
//Pass Word, Timer Mode Select= Interval Timer, Count Clear, Interval Selection = divide by 8192
	WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTIS0 ;
//enable WD interrupt, set Interrupt Enable Register 1 bit 0 to true (1)
	IE1 |= 0x01;
//clean up port 2 because it is unused
//Select = 0 (digital I/O selected), Dir =0 (inputs), enable the pull down (P2OUT=0 ???) on the unused outputs and crystal pins
	P2SEL = 0;
	P2DIR = 0;
	P2REN = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
//USI will override these settings for SCLK, SDI and SDO (but not P1.4)
	P1OUT = BIT4;
	P1DIR = BIT4;//only P1.4 will be the SlaveSelect output handled by software
	P1REN = BIT0 | BIT1 | BIT2 | BIT3 ; //pull the resistors on the unused pins
//enable SDI, SDO, SCLK, MSB first !!! NG, master, USIIGE is not being used, output enable, software reset
	USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIMST | USIOE | USISWRST;
// CPHA = 1 now, SPI not I2C, USI counter Interrupt Enable, set the Interrupt Flag
	USICTL1 = USIIE | USIIFG;// can't clear the USIIFG in reset mode __removed 12/23/2013     USICKPH |
//SCLK = SMCLK / 128, clock idles low
	USICKCTL = USIDIV_7 | USISSEL_2;
	USICTL0 &= ~USISWRST; //release from reset
	USICTL1 &= ~USIIFG; //avoid unwanted interrupt
	for ( ; ; ){//loop forever with interrupts
		__low_power_mode_0();//LPM0 between interrupts
		//++waitRaise; delay cycles removed the potential use of this
	}
}
//----------------------------------------------------------------------------
// ISR for the interrupt that is created by the watchdog, DOES NOT CREATE AN ACTUAL WATCHDOG FAULT
//----------------------------------------------------------------------------
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR (void) //acknowledged automatically
{
	P1OUT &= ~BIT4; //lower the Slave Select line (make active)
		USISR = TXdata1;// 1.6V
	//increment the iterator so next time the 15V command goes
	//Start SPI to transfer 16 bits, TRANSFER STARTS NOW, managed by hardware
	USICNT = USI16B | 16; //counter is configured for 16 bits and set to 16 bits
}
//----------------------------------------------------------------------------
// ISR for USI, this is called when USICNT gets to 0
//----------------------------------------------------------------------------
#pragma vector = USI_VECTOR
__interrupt void USI_ISR (void)
{

	__delay_cycles(100);//~CS need to wait to raise until after the last falling edge of Clock
	P1OUT |= BIT4;// raise the Slave Select line (make inactive)

	USICTL1 &= ~USIIFG; //clear the interrupt flag

}
