//transplant of
//http://fortytwoandnow.blogspot.com/2012/07/msp430g2-usci-spi-interface-1.html

//adapted for UCB0 and UCA0 by Jeff simultaneously

#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdint.h>

volatile uint16_t potLevel = 0x08C5;

//***Start UCA0 code
void UARTSendArray( char *TxArray, char ArrayLength);// Dec 31 removed both unsigned keywords

//JPK__ static volatile char data;??? Jan 1, whay is it static??? should be volatile?
static char data;//jpk Dec 30_ removed the volatile keyword and works fine Dec 31 removed unsigned keyword
//***End UCA0 code


void clockConfig()
{
 // configure the CPU clock (MCLK)
 // to run from DCO @ 16MHz and SMCLK = DCO / 4
 //BCSCTL1 = CALBC1_16MHZ; // Set DCO
 //DCOCTL = CALDCO_16MHZ;
 BCSCTL1 = CALBC1_1MHZ; // Set DCO
 DCOCTL = CALDCO_1MHZ;
 //__jpk_Jan1__ BCSCTL2= DIVS_2 + DIVM_0; // divider=4 for SMCLK and 1 for MCLK
}

void pinConfig()
{
 // set the pin mode 3 for pins 5  6 and 7 of port 1 (USCI mode), ???6 is not really needed
 P1SEL = BIT1 + BIT2 + BIT5 + BIT7; // low bit = 1 for pin 1 and 2 4. BIT 3 is 0 (CS via GPIO), P1.1 = RXD, P1.2=TXD
 P1SEL2 = BIT1 + BIT2 + BIT5 + BIT7; // high bit = 1 for pin 1 and 2 4 BIT 3 is 0 (CS via GPIO), P1.1 = RXD, P1.2=TXD
 //jan1 removed BIT6 from both P1SEL and SEL2

 P1DIR |= BIT3 + BIT0 + BIT6; // p1.3 set to output to drive CS and the two LEDs for visual aid
 P1OUT |= BIT3 + BIT0; //jpk_Set p1.3 high for inactive start // pull p1.3 to low - CS low
}

// USCI
void spiConfig()
{
 UCB0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCMSB;
//jpk_removed_Dec_29     UCCKPL+
 // synchronous (=SPI) master 3 wire SPI, clock polarity High
/*SPI mode is selected when the UCSYNC bit is set and SPI
mode (3-pin or 4-pin) is selected with the UCMODEx bits.*/
 //use SCLK : 4MHz (MCP4131 supports up to 10MHz write via SPI)
 UCB0CTL1 |= UCSSEL_2;
 // set baud rate = SMCLK, no further division
 UCB0BR0 = 10;
 UCB0BR1 = 0;
 UCB0CTL1 &= ~UCSWRST; // **Initialize USCI **
}


// drives a MCP4131 digital potentiometer via SPI
void setPotValue( uint16_t dataOut)
{//why??? do we need the !(IFG2 & UCA0TXIFG) why not just !(UCA0TXIFG)  ???
 //while (!(IFG2 & UCB0TXIFG)); // wait for TX buffer ready
 while (!( IFG2 && UCB0TXIFG));
 P1OUT &= ~BIT3;
 __delay_cycles(10);
 UCB0TXBUF = ( dataOut >> 8 );  // Send command 0
 __delay_cycles(120);
 //while (!(IFG2 & UCB0TXIFG)); // wait for TX buffer ready
 while (!( UCB0TXIFG));
 UCB0TXBUF = dataOut; // Send wiper level
 __delay_cycles(120);
 P1OUT |= BIT3;
}


void main(void)
{
 // setup
 WDTCTL = WDTPW + WDTHOLD;
 clockConfig();
 pinConfig();
 spiConfig();


 //////////////////////////////from UART program
/////////////This can be left here for now but should be moved later
 /* Configure hardware UART */

  UCA0CTL1 |= UCSSEL_2; // Use SMCLK
  UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
  UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
  UCA0CTL0 |= UCMSB; //MSB first, to be consistent with SPI
  UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
  IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt

 __bis_SR_register( GIE); // Enter LPM0, interrupts enabled
//removed as argument Jan1      LPM0_bits +

 ////////////////////////////////End UART program





 // program execution loop
 while (1)
 {
	 setPotValue(potLevel);
   __delay_cycles(4000); // I know, there are better ways...
    }

}





//Functions from UART program

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
data = UCA0RXBUF;
UARTSendArray("Received command: ", 18);
UARTSendArray(&data, 1);
UARTSendArray("\n\r", 2);

switch(data){
 //case 'R':
case 82:
{
 P1OUT |= BIT0;
 }
 break;
 case 'r':
 {
 P1OUT &= ~BIT0;
 }
 break;
 case 'G':
 {
 P1OUT |= BIT6;
 }
 break;
 case 'g':
 {
 P1OUT &= ~BIT6;
 }
 break;
 default:
 {//uncomment this stuff to get things back from the UART on the matlab
 //UARTSendArray("Unknown Command: ", 17);
 //UARTSendArray(&data, 1);
 //UARTSendArray("\n\r", 2);
 }
 break;
 }
}

void UARTSendArray( char *TxArray, char ArrayLength){//removed both unsigned
 // Send number of bytes Specified in ArrayLength in the array at using the hardware UART 0
 // Example usage: UARTSendArray("Hello", 5);
 // int data[2]={1023, 235};
 // UARTSendArray(data, 4); // Note because the UART transmits bytes it is necessary to send two bytes for each integer hence the data length is twice the array length

while(ArrayLength--){ // Loop until StringLength == 0 and post decrement
 while(!(IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready for new data
 UCA0TXBUF = *TxArray; //Write the character at the location specified by the pointer
 TxArray++; //Increment the TxString pointer to point to the next character
 }
}
