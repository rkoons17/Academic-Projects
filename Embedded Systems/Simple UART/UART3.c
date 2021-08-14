//Lab 8.3: Sending an ASCII string over UART
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


void init_UART(void);
void uart_write_char(unsigned char ch);
void uart_write_string(char * str);

int main(void)
{   //string to Tx to PC
    char mystring[] = "Hello World!";

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
     PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

     init_UART(); //Initialize
     uart_write_string( &mystring[0] ); //call function


    return 0;
}

void uart_write_string(char * str)
{
 int i=0;//parsing integer
 while( str[i] != '\0')//while not at end of string
 {
     uart_write_char(str[i]);//pass current char
     i++;
 }
 return;
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


