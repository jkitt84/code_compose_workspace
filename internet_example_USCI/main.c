//http://fortytwoandnow.blogspot.com/2012/07/msp430g2-usci-spi-interface-1.html
//start with USCI A

#include <msp430g2553.h>

volatile unsigned char potLevel = 0xE6;

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
 // set the pin mode 3 for pins 1  2 and 4 of port 1 (USCI mode)
 P1SEL = BIT1 + BIT2 + BIT4; // low bit = 1 for pin 1 and 2 4. BIT 3 is 0 (CS via GPIO)
 P1SEL2 = BIT1 + BIT2 + BIT4; // high bit = 1 for pin 1 and 2 4 BIT 3 is 0 (CS via GPIO)

 P1DIR |= BIT3; // p1.3 set to output to drive CS
 P1OUT |= BIT3; //jpk_Set p1.3 high for inactive start // pull p1.3 to low - CS low
}

// USCI
void spiConfig()
{
 UCA0CTL0 |= UCCKPL+ UCMST + UCMODE_0 + UCSYNC + UCMSB;
  // synchronous (=SPI) master 3 wire SPI, clock polarity High
/*SPI mode is selected when the UCSYNC bit is set and SPI
mode (3-pin or 4-pin) is selected with the UCMODEx bits.*/
 //use SCLK : 4MHz (MCP4131 supports up to 10MHz write via SPI)
 UCA0CTL1 |= UCSSEL_2;
 // set baud rate = SMCLK, no further division
 UCA0BR0 = 10;
 UCA0BR1 = 0;
 UCA0MCTL = 0; // No modulation
 UCA0CTL1 &= ~UCSWRST; // **Initialize USCI **
}


// drives a MCP4131 digital potentiometer via SPI
void setPotValue(unsigned char dataOut)
{//why??? do we need the !(IFG2 & UCA0TXIFG) why not just !(UCA0TXIFG)  ???
 while (!(IFG2 & UCA0TXIFG)); // wait for TX buffer ready
 P1OUT &= ~BIT3;
 UCA0TXBUF = 0;  // Send command 0
 __delay_cycles(500);
 while (!(IFG2 & UCA0TXIFG)); // wait for TX buffer ready
 UCA0TXBUF = dataOut; // Send wiper level
 __delay_cycles(500);
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
setPotValue(potLevel);
   __delay_cycles(100000); // I know, there are better ways...
    }



}
