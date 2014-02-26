/*
 * listing4point17.c
toggle LEDs by polling timerA
timerA is SMCLK divided by 8, compare mode

toggles an LED from p1.0 to p1.1
 */
/*
#include  <msp430g2452.h>
#include <stdio.h>
//#include <io430.h>

#define LED1	BIT0
#define LED2	BIT1
char timerAflag = 0; //used for bit testing


void main(void){
	WDTCTL = WDTPW|WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ; //BasicClockSystemControl1
	DCOCTL  = CALDCO_1MHZ; //DigitallyControllerOscillatorControl

	P1OUT = LED1;
	P1DIR = LED1|LED2;
	TACCR0 = 10000;//just put Timer_A in mode 1 and it will count to Timer_A_Counter_Compare_Register_0
	TACTL = MC_1|ID_3|TASSEL_2|TACLR; //Timer A, ModeControl=1,InputDivider=3,TimerA_SourceSelect=2(SMCLK),TimerA_clear(start timer A)
	for( ; ; ){
		while (timerAflag == 0){//do nothing while the TimerA_ControlBit, TimerA_Interupt_Flag is not set
			if((TACTL & 0x0001) ==0 ){
				timerAflag = 0;
			} else {
				timerAflag = 1;
			}
		}
		//TACTL_bit.TAIFG = 0;//clear the overflow flag, does Chris know how to get this working
		TACTL &= ~(0x0001);//clear the overflow flag bit, derived from datasheet
		timerAflag = 0;//clear the temp variable
		P1OUT ^=LED1|LED2;//toggle the lights

	}

}
*/
