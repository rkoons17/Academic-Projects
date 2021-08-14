//Lab 8.1: Tx 0-9 to computer in infinite loop, and use keyoard 1/2 for greenLED ON/OFF
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

//function prototypes
void init_UART(void);
void uart_write_char(unsigned char ch);
unsigned char uart_read_char(void);

void main(void)
{
    unsigned int i; //integer for delay loop
    unsigned char ch; //char for producing 0-9 infinite loop
  //formatting parameters
    unsigned char nl='\n';
    unsigned char cr='\r';
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
     PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

     //LED init.
     P1DIR |= redLED; //set as output
     P9DIR |= greenLED;
     P1OUT &= ~redLED;    //red OFF
     P9OUT &= ~greenLED; //green OFF

     init_UART(); //Initialize

     for(;;) //infinite loop
     {
         for(ch='0'; ch<='9'; ch++)//infinite transmission loop
         {    //delay loop
             for(i=0; i<50000; i++)//check for keyboard response during delay loop
             {
              unsigned char result=uart_read_char(); //read from keyboard
              //alternatively use if( uart_read_char() == 1)
              if(result == '1')//If user types 1
                  P9OUT |= greenLED; //Turn on green LED
              if(result == '2') //If user types 2
                  P9OUT &= ~greenLED; //Turn off
             }
                uart_write_char(ch); //Tx current ch to PC
                uart_write_char(nl); //Tx newline (down a line)
                uart_write_char(cr); //Tx carriage return (leftmost column)
                P1OUT ^= redLED; //toggle LED

         }

     }


}


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


//TXFLAG: 1 When ready to transmit, 0 during transmission
void uart_write_char(unsigned char ch)
{
    //wait for any ongoing transmission  to complete
    while((FLAGS & TXFLAG) ==0 ){}

    //Write byte to the transmit buffer
    TXBUFFER= ch;
}

//RXFLAG--0--no new data, 1--byte is being received.
//Copy byte from receive buffer and then flag goes to zero.
unsigned char uart_read_char(void)
{
    unsigned char temp;

    //Return NULL if no byte received
    if((FLAGS & RXFLAG) == 0 )
        return '\0';

    temp= RXBUFFER; //else copy received byte (auto clears flag)
    return temp;
}
