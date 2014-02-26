//trying to blink LEDs using double byte words from the UART
#include "msp430g2553.h"
#include <stdint.h>
#include <math.h>

#include "ascii2dec.h"
#include "uart_config.h"
#include "stateMachine.h"
#include "pinConfig.h"
#include "updateLEDs.h"
#include "clockConfig.h"
#include "spiConfig.h"
#include "set7512.h"
#include "set5504.h"


uint16_t rxWord = 0x0000; //this will hold the 2 byte (4 hex digits) input word

unsigned char slaveStatus = IDLE; // slave status will hold the value designating if message is being received

volatile uint16_t valueFor7512 = 0x0145; //0x0145 is an arbitrary choice giving about .278V
volatile uint16_t valueFor5504 = 0x7004; //0x7004 is the "enable all" command for AD5504

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT

	clockConfig();
	pinConfig();
	uartConfig();
	spiConfig();


	__bis_SR_register(GIE);


	 while (1) //loop forever with interrupts enabled but also setting the two DACs periodically
	 {
		 set7512( valueFor7512);;
	   __delay_cycles(4000); // I know, there are better ways...
	   	 set5504( valueFor5504);
	   	__delay_cycles(4000); // I know, there are better ways...
	    }

}



#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{//really need to make this "if" IFG && UCA0RXISR or equivalent
if( IFG2 && UCA0RXIFG) {

	unsigned char data = UCA0RXBUF;

switch (slaveStatus){
case IDLE:
		if ( data == SOH ){
			slaveStatus = READY_DEVICE_SELECT;
		}
		else //should put something for else, meaning did not receive SOH to start the frame
			;//Do nothing for now, but should throw error
	break;

case READY_DEVICE_SELECT:
		if ( data == DC1){
			slaveStatus = READY_MS_CHAR1;
		}
		//THIS ELSE IF GOES HERE for 5504
		else if ( data == DC2)
			slaveStatus = READY_MS_CHAR2;
		else
			;// do nothing for now, but should throw error
	break;

//For the AD5504 _ COARSE DAC
case READY_MS_CHAR1:// need to make sure these are numbers coming in, really need exception
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 12 );
	slaveStatus = READY_CHAR1_3;
	break;
case READY_CHAR1_3:
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 8 );//FIXME all the asciiByteToDecByte calls should likely be "in-line" to run faster
	slaveStatus = READY_CHAR1_2;
	break;
case READY_CHAR1_2:
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 4 );
	slaveStatus = READY_LS_CHAR1;
	break;
case READY_LS_CHAR1:
	rxWord = rxWord + asciiByteToDecByte( (uint8_t) data ) ;
	slaveStatus = IDLE;
	//updateLEDs( rxWord );
	valueFor5504 = rxWord;
	rxWord = 0;
	break;




//For the DAC7512	FINE DAC
case READY_MS_CHAR2:// need to make sure these are numbers coming in, really need exception
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 12 );
	slaveStatus = READY_CHAR2_3;
	break;
case READY_CHAR2_3:
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 8 );//FIXME all the asciiByteToDecByte calls should likely be "in-line" to run faster
	slaveStatus = READY_CHAR2_2;
	break;
case READY_CHAR2_2:
	rxWord = rxWord + (asciiByteToDecByte( (uint8_t) data ) << 4 );
	slaveStatus = READY_LS_CHAR2;
	break;
case READY_LS_CHAR2:
	rxWord = rxWord + asciiByteToDecByte( (uint8_t) data ) ;
	slaveStatus = IDLE;
	//updateLEDs( rxWord );
	valueFor7512 = rxWord;
	rxWord = 0;
	break;

//really it should wait for EOT to go idle, IDLE just above should be replaced with state "FINISHED" or similar, this should also clear the rxWord
//really there should be a latch for RX word, or at least don't look at it until after the last case (only call there?)
}

}
}

//Deleted all the UART send array stuff to clean things up (including the declaration)
