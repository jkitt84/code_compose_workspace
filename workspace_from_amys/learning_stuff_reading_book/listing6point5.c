/*
 * listing6point5.c
 *
 *  Created on: May 27, 2013
 *  toggle LEDs using interupts
 */
/*
#include  <msp430g2452.h>
#include  <intrinsics.h>
//--------------------------------------------------------------------
//Pins for LEDs
#define LED1	BIT2
#define LED2	BIT3
//--------------------------------------------------------------------
void main (void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1OUT = LED1;
	P1DIR = LED1|LED2;
	TACCR0 = 9999;//TimerA_CaptureCompareRegister0 flags once TAR=49999
	TACCTL0 = CCIE;//TimerA_CaptureCompareControl0 set to enable interupts on Compare 0
	TACTL = MC_1|ID_3|TASSEL_2|TACLR;//TimerA_Control set with Mode= Count Up to TACCRO, Internal division = devide by 8, TimerA_Source_Select = 2 (SMCLK), clear
	__enable_interrupt();//this might have to be GIE
	for ( ; ; ){
		//__low_power_mode_0();//NOT NEEDED if you dont want to go to Low Power Mode
	}
}
//--------------------------------------------------------------------
//Interupt service routine for Timer A channel 0

#pragma vector = TIMER0_A0_VECTOR//this had to change from book
__interrupt void TA0_ISR (void)
{
	P1OUT ^= LED1|LED2;
}
*/