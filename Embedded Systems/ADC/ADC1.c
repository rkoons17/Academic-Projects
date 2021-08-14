//Lab 10.1: ADC-Horizontal Joystick
#include <msp430.h> 
#define FLAGS UCA1IFG //Contains the Tx and Rx flags
#define RXFLAG UCRXIFG //Rx Flag
#define TXFLAG UCTXIFG //Tx Flag
#define TXBUFFER UCA1TXBUF// Tx Buffer
#define RXBUFFER UCA1RXBUF// Rx Buffer
#define redLED BIT0    //red LED at P1.0
#define greenLED BIT7  //green LED at P9.7
#define BUT1 BIT1     //Button 1 at P1.1
#define BUT2 BIT2     //BUtton 2 at P1.2

void init_adc(void); //function for initializing the ADc
void init_UART(void); //alternate UART Init. Function
void uart_write_char(unsigned char ch);
void uart_write_uint16(unsigned int n);


void main(void)
{
     unsigned int n=0; //incrementing 16-bit integer
     unsigned int i; //delay loop
    unsigned char nl='\n';//formatting parameters
    unsigned char cr='\r';
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
     PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

     //LED init.
     P1DIR |= redLED; //set as output
     P9DIR |= greenLED;
     P1OUT &= ~redLED;    //red OFF
     P9OUT &= ~greenLED; //green OFF
     init_UART(); //Initialize
     init_adc();



//infinite loop, set ADC12SC bit to start conversion
     //wait for ADC12BUSY bit to clear
     //
     while(1)
     {             //delay between numbers
                 for(i=0; i<65000; i++){}

                 //set ADC12SC bit
                 ADC12CTL0 |= ADC12SC; //set, start conversion for adc
                 //wait for flag to clear
                 while( (ADC12CTL1 & ADC12BUSY) != 0 ){} //wait here, use !=0, since there could be other bits in bit field

                    uart_write_uint16(ADC12MEM0); //print number to screen
                    uart_write_char(nl); //Tx newline (down a line)
                    uart_write_char(cr); //Tx carriage return (leftmost column)
                    P1OUT ^= redLED; //toggle LEDS

                    //n++;
             }

}


void init_adc() //function to init. adc
{
// Divert the pins to analog functionality
// X-axis: A10/P9.2, for A10 (P9DIR=x, P9SEL1=1, P9SEL0=1)
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;
//Y-axis: A4/P8.7, for A4(P8DIR=x P8SEL1/0=1)
     P8SEL0 |= BIT7;
     P8SEL1 |= BIT7;


// Turn on the ADC module
    ADC12CTL0 |= ADC12ON;

// Turn off ENC (Enable Conversion) bit while modifying the configuration
    ADC12CTL0 &= ~ADC12ENC;

//*************** ADC12CTL0 ***************/ //set number of cycles (fast mode)
    // Set ADC12SHT0 (select the number of cycles that you determined)
    ADC12CTL0 |= ADC12SHT0_3;  //32 cycles selected for bit field bits 8-111
    //*************** ADC12CTL1 ***************
    // Set ADC12SHS (select ADC12SC bit as the trigger)
    // Set ADC12SHP bit
    // Set ADC12DIV (select the divider you determined)
    // Set ADC12SSEL (select MODOSC)

    //ADC12SHS should be defaulted to use ADC, but just in case
    ADC12CTL1 |= ADC12SHS_0|ADC12SHP|ADC12DIV_0|ADC12SSEL_0;
//note: the last three value should all be defults, but they are stil set just in case


    //*************** ADC12CTL2 ***************
    // Set ADC12RES (select 12-bit resolution)
    ADC12CTL2 |= ADC12RES_2; //select 12 bit resolution //should be default, and unsigned binary
    //Omit Binary unsigned bitmask to have 0.
    // Set ADC12DF (select unsigned binary format)
    //this value is default
    //ADC12DF=0;  //default

    //*************** ADC12CTL3 ***************
    // Leave all fields at default values

    //*************** ADC12MCTL0 ***************

    // Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
    //should be default
   // ADC12VRSEL=0;
    ADC12MCTL0 |= ADC12INCH_10| ADC12VRSEL_0;
    // Set ADC12INCH (select channel A10)

    // Turn on ENC (Enable Conversion) bit at the end of the configuration
    ADC12CTL0 |= ADC12ENC;
    return;
    }









//UART config
//4800 Baud, 8-bit data, LSB first, no parity, 1 stop bit, no flow CTL
//Initial clock: ACLK @ 32768 Hz, no oversampling
//UART config
//9600 Baud, 8-bit data, LSB first, no parity, 1 stop bit, no flow CTL
//Initial clock: SMCLK @ 1.000 MHz with Oversampling (16x)
void init_UART(void)
{
    //Divert GPIO pins for UART
        P3SEL1 &= ~(BIT4|BIT5);  //Using 3.4--UCA1TXD, 3.5--UCA1RXD
        P3SEL0 |= (BIT4|BIT5);

        //Use SMCLK, other settings default
        UCA1CTLW0 |= UCSSEL_2;

        //Configure clock dividers and modulators
        //UCBR=6, UCBRF=8, UCBRS=0x20, UCOS16=1 (over-sampling)
        UCA1BRW =6;
        //bit masking for UCBRS aand UCBRF done using 0-7 bit number
        UCA1MCTLW = UCBRS5|UCBRF3|UCOS16;


        //exit reset state, to begin Tx/Rx
        UCA1CTLW0 &= ~UCSWRST;
}


void uart_write_uint16(unsigned int n)
{
 int digit; //digit for parsing through

 if(n>=10000)
 {
     //modulo 10 ensures no digits past 16-bit int allowed (out of constraint)
     digit = (n/10000) % 10; //takes leftmost digit first
     uart_write_char('0' + digit); //pass correct ascii value for digit to computer
 } ///1000

 if(n>=1000)
  {
      digit = (n/1000) % 10; //takes next leftmost digit
      uart_write_char('0' + digit);
  }
//modulo is also used to hack off other digits of int that is being displayed
 if(n>=100)
  {
      digit = (n/100) % 10;//take next digit
      uart_write_char('0' + digit); //print
  }

 if(n>=10)
  {  //process repeats similarly
      digit = (n/10) % 10;
      uart_write_char('0' + digit);
  }
//ones place always executes
 digit = n%10;
 uart_write_char('0'+digit);

 return;

}

//TXFLAG: 1 When ready to transmit, 0 during transmission
void uart_write_char(unsigned char ch)
{
    //wait for any ongoing transmission  to complete
    while((FLAGS & TXFLAG) ==0 ){}

    //Write byte to the transmit buffer
    TXBUFFER= ch;
}

