//transplant of
//http://fortytwoandnow.blogspot.com/2012/07/msp430g2-usci-spi-interface-1.html
//which was set up for UCA0
//adapted for UCB0 by Jeff
// set up for AD5504
#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdint.h>

volatile unsigned char potLevel = 0x04;
uint16_t TXdata1 = 0x7004;
uint16_t TXdata2 = 0x1A0F;			//put channel as at about 30V using 0x180F

void clockConfig()
{
 // configure the CPU clock (MCLK)
 // to run from DCO @ 16MHz and SMCLK = DCO / 4
 BCSCTL1 = CALBC1_16MHZ; // Set DCO
 DCOCTL = CALDCO_16MHZ;
 BCSCTL2= DIVS_2 + DIVM_0; // divider=4 for SMCLK and 1 for MCLK
}

void pinConfig()
{
 // set the pin mode 3 for pins 5  6 and 7 of port 1 (USCI mode), ???6 is not really needed
 P1SEL = BIT5 + BIT6 + BIT7; // low bit = 1 for pin 1 and 2 4. BIT 3 is 0 (CS via GPIO)
 P1SEL2 = BIT5 + BIT6 + BIT7; // high bit = 1 for pin 1 and 2 4 BIT 3 is 0 (CS via GPIO)

 P1DIR |= BIT3; // p1.3 set to output to drive CS
 P1OUT |= BIT3; //jpk_Set p1.3 high for inactive start // pull p1.3 to low - CS low
}

// USCI
void spiConfig()
{
 UCB0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCMSB + UCCKPH;
//jpk_removed_Dec_29     UCCKPL+
 // synchronous (=SPI) master 3 wire SPI, clock polarity High
/*SPI mode is selected when the UCSYNC bit is set and SPI
mode (3-pin or 4-pin) is selected with the UCMODEx bits.*/
 //use SCLK : 4MHz (MCP4131 supports up to 10MHz write via SPI)
 UCB0CTL1 |= UCSSEL_2;
 // set baud rate = SMCLK, no further division
 UCB0BR0 = 10;
 UCB0BR1 = 0;
 //UCB0MCTL = 0; // No modulation//jpk Apparently UCB does not have modulation control
 UCB0CTL1 &= ~UCSWRST; // **Initialize USCI **
}


// drives a MCP4131 digital potentiometer via SPI
void setPotValue(uint16_t dataOut)
{//why??? do we need the !(IFG2 & UCA0TXIFG) why not just !(UCA0TXIFG)  ???
 //while (!(IFG2 & UCB0TXIFG)); // wait for TX buffer ready
 while (!( UCB0TXIFG));
 P1OUT &= ~BIT3;
 __delay_cycles(10);
 UCB0TXBUF = (dataOut >> 8);  // Send command 0
 __delay_cycles(350);
 //while (!(IFG2 & UCB0TXIFG)); // wait for TX buffer ready
 while (!( UCB0TXIFG));
 UCB0TXBUF = dataOut; // Send wiper level
 __delay_cycles(350);
 P1OUT |= BIT3;
}


void main(void)
{
 // setup
 WDTCTL = WDTPW + WDTHOLD;
 clockConfig();
 pinConfig();
 spiConfig();
    // program execution loop
 while (1)
 {

   	setPotValue(TXdata1);
   __delay_cycles(100000); // I know, there are better ways...
  	setPotValue(TXdata2);
  __delay_cycles(100000); // I know, there are better ways...
    }



}

