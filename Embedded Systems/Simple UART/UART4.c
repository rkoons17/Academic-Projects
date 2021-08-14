//Lab 8.4: Alternative UART Config, counting up infintely
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


void init_UART2(void); //alternate UART Init. Function
void uart_write_char(unsigned char ch);
void uart_write_uint16(unsigned int n);
void convert_32khz(void); //use 32kHz crystal soldered to the board




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
     convert_32khz();
     init_UART2(); //Initialize

     while(1)
     {             //delay between numbers
                 for(i=0; i<65000; i++){}
                 P1OUT ^= redLED; //toggle LEDS
                     uart_write_uint16(n); //print number to screen
                    uart_write_char(nl); //Tx newline (down a line)
                    uart_write_char(cr); //Tx carriage return (leftmost column)

                    n++;
             }

}

//Function for Converting ACLK to 32768 Hz
void convert_32khz()
{
    //By default, ACLK runs on  LFMODCLK at 5Mhz/128= 39 Khz

    //Reroute pins to LFXIN/LFXOUT functionality
    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    //wait until oscillator fault flags are cleared
    CSCTL0 = CSKEY;  //unlock CS registers
    do {
        CSCTL5 &= ~LFXTOFFG;     //local fault flag
        SFRIFG1 &= ~OFIFG;       //global fault flag
    } while((CSCTL5 & LFXTOFFG) != 0 );

    CSCTL0_H = 0;   //Lock CS registers
    return;
}


//UART config
//4800 Baud, 8-bit data, LSB first, no parity, 1 stop bit, no flow CTL
//Initial clock: ACLK @ 32768 Hz, no oversampling
void init_UART2(void)
{
    //Divert GPIO pins for UART
        P3SEL1 &= ~(BIT4|BIT5);  //Using 3.4--UCA1TXD, 3.5--UCA1RXD
        P3SEL0 |= (BIT4|BIT5);

        //Use ACLK, other settings default
        UCA1CTLW0 |= UCSSEL_1;

        //Configure clock dividers and modulators
        //UCBR=6, UCBRS=0xEE
        UCA1BRW =6;
        UCA1MCTLW = UCBRS7|UCBRS6|UCBRS5|UCBRS3|UCBRS2|UCBRS1;

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

