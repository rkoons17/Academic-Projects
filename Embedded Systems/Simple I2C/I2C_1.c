//Simple I2C Transmission
#include <msp430.h> 
#define FLAGS UCA1IFG //Contains the Tx and Rx flags
#define RXFLAG UCRXIFG //Rx Flag
#define TXFLAG UCTXIFG //Tx Flag
#define TXBUFFER UCA1TXBUF// Tx Buffer
#define RXBUFFER UCA1RXBUF// Rx Buffer


void Initialize_I2C(void);
int i2c_read_word(unsigned char, unsigned char, unsigned int*);
int i2c_write_word(unsigned char, unsigned char, unsigned int);
void init_UART(void);
void uart_write_char(unsigned char ch);
void uart_write_uint16(unsigned int n);


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

	unsigned int manufacturerID=0;
	unsigned int deviceID=0;
	unsigned int count=0;
	unsigned int loop=0;
	unsigned int i =0;
	unsigned char nl='\n';
	unsigned char cr='\r';
	unsigned char colon=':';
	unsigned char space=' ';

	 init_UART(); //Initialize
     Initialize_I2C();
for(;;){
    for(loop=0; loop<=65534; loop++){

        uart_write_uint16(count);
        uart_write_char(colon);
        uart_write_char(space);

	//read manufacturer ID and Device ID registers
	i2c_read_word(0x44,0x7E, &manufacturerID);
	i2c_read_word(0x44,0x7F, &deviceID);
	uart_write_uint16(manufacturerID);
	uart_write_char(nl);
	uart_write_char(cr);
	uart_write_uint16(deviceID);
	uart_write_char(nl);
    uart_write_char(cr);
    uart_write_char(nl);
    uart_write_char(cr);
    count++;

    for( i=0; i<65000; i++){} //delay loop
            for( i=0; i<65000; i++){} //delay loop
            for( i=0; i<30000; i++){} //delay loop
            for( i=0; i<10000; i++){} //delay loop
    for( i=0; i<65000; i++){} //delay loop
	//i2c_read_word(0x22, 0x50, &data) read two bytes from register 0x50 on I2C device 0x22, store total in data (pass via uart)
	//unsigned int data= 0xABCD; i2c_write_word(0x22, 0x60, data); wrte 0xABCD to register 0x50on I2C device 0x22
    }
}
	return 0;
}


// Read a word (2 bytes) from I2C (address, register)
int i2c_read_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int * data)
{
unsigned char byte1, byte2;
// Initialize the bytes to make sure data is received every time
byte1 = 111;
byte2 = 111;
//********** Write Frame #1 ***************************
UCB1I2CSA = i2c_address; // Set I2C address 92
UCB1IFG &= ~UCTXIFG0;
UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal
while ((UCB1IFG & UCTXIFG0) ==0) {}
UCB1TXBUF = i2c_reg; // Byte = register address
while((UCB1CTLW0 & UCTXSTT)!=0) {}
if(( UCB1IFG & UCNACKIFG )!=0) return -1;
UCB1CTLW0 &= ~UCTR; // Master reads (R/W bit = Read)
UCB1CTLW0 |= UCTXSTT; // Initiate a repeated Start Signal
//****************************************************
//********** Read Frame #1 ***************************
while ( (UCB1IFG & UCRXIFG0) == 0) {}
byte1 = UCB1RXBUF;
//****************************************************
//********** Read Frame #2 ***************************
while((UCB1CTLW0 & UCTXSTT)!=0) {}
UCB1CTLW0 |= UCTXSTP; // Setup the Stop Signal
while ( (UCB1IFG & UCRXIFG0) == 0) {}
byte2 = UCB1RXBUF;
while ( (UCB1CTLW0 & UCTXSTP) != 0) {}
//****************************************************
// Merge the two received bytes
*data = ( (byte1 << 8) | (byte2 & 0xFF) );
return 0;
}

// Write a word (2 bytes) to I2C (address, register)
int i2c_write_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int data) {
unsigned char byte1, byte2;
byte1 = (data >> 8) & 0xFF; // MSByte
byte2 = data & 0xFF; // LSByte
UCB1I2CSA = i2c_address; // Set I2C address
UCB1CTLW0 |= UCTR; // Master writes (R/W bit = Write)
UCB1CTLW0 |= UCTXSTT; // Initiate the Start Signal
while ((UCB1IFG & UCTXIFG0) ==0) {}
UCB1TXBUF = i2c_reg; // Byte = register address
while((UCB1CTLW0 & UCTXSTT)!=0) {}
//********** Write Byte #1 ***************************
UCB1TXBUF = byte1;
while ( (UCB1IFG & UCTXIFG0) == 0) {}
//********** Write Byte #2 ***************************
UCB1TXBUF = byte2;
while ( (UCB1IFG & UCTXIFG0) == 0) {}
UCB1CTLW0 |= UCTXSTP;
while ( (UCB1CTLW0 & UCTXSTP) != 0) {}
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
        //bit masking for UCBRS and UCBRF done using 0-7 bit number
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



// Configure eUSCI in I2C master mode
void Initialize_I2C(void)
{
// Enter reset state before the configuration starts...
UCB1CTLW0 |= UCSWRST;
// Divert pins to I2C functionality
P4SEL1 |= (BIT1|BIT0);
P4SEL0 &= ~(BIT1|BIT0);
// Keep all the default values except the fields below...
// (UCMode 3:I2C) (Master Mode) (UCSSEL 1:ACLK, 2,3:SMCLK)
UCB1CTLW0 |= UCMODE_3 | UCMST | UCSSEL_3;
// Clock divider = 8 (SMCLK @ 1.048 MHz / 8 = 131 KHz)
//^ IS THIS WRONG!!***************************

UCB1BRW = 8;
// Exit the reset mode
UCB1CTLW0 &= ~UCSWRST;
}
