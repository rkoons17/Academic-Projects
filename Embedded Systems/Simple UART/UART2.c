//Lab 8.2: Sending unsigned 16-bit int incrementing loop to PC via UART
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
void uart_write_uint16(unsigned int n);

int main(void)
{
    unsigned int n=0; //16-bit int to  pass during function call
    unsigned int i=0; //delay loop between integers
    unsigned char nl='\n';//formatting parameters
    unsigned char cr='\r';

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
     PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

     init_UART(); //Initialize
     while(1)
     {
         //increment n loop
     for(n=0; n<=65534; n++){


         uart_write_uint16( n ); //call function
         //delay loops
         for(i=0; i<65000; i++)
                               {}
         uart_write_char(nl); //Tx newline (down a line)
         uart_write_char(cr); //Tx carriage return (leftmost column)
     }
     }


    return 0;
}


//function for properly Tx 16-bit int (0-65535) to PC
void uart_write_uint16(unsigned int n)
{
 int digit; //digit for parsing through

 //check ten=thousandths place
 if(n>=10000)
 {
     //modulo 10 ensures no digits past 16-bit int allowed (out of constraint)
     digit = (n/10000) % 10; //takes leftmost digit first
     uart_write_char('0' + digit); //pass correct ascii value for digit to computer
 }

//check thousandths place
 if(n>=1000)
  {
      digit = (n/1000) % 10; //takes next leftmost digit
      uart_write_char('0' + digit); //print next digit on screen
  }
//modulo is also used to hack off other digits of int that is being displayed
 //(i.e. ten thousandths place that already printed to PC)

 //Hundredths place
 if(n>=100)
  {
      digit = (n/100) % 10;//take next digit
      uart_write_char('0' + digit); //print
  }

//tenths place
 if(n>=10)
  {  //process repeats similarly
      digit = (n/10) % 10;
      uart_write_char('0' + digit);
  }

//ones place always exectures
 digit = n%10; //get rid of all other digits
 uart_write_char('0'+ digit); //print digit on  PC

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
