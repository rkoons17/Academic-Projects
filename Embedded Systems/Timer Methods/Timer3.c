// Using Timer_A with 2 channels, PWM on LED
// Using ACLK @ 32 KHz (undivided)
// Channel 0 toggles the red LED every 0.1 seconds
// Channel 1 toggles the green LED every 0.5 seconds
#include <msp430fr6989.h>
#define redLED BIT0 // Red at P1.0
#define greenLED BIT7 // Green at P9.7

void convert_32khz();

void main(void)
{
WDTCTL = WDTPW | WDTHOLD; // Stop WDT
PM5CTL0 &= ~LOCKLPM5; // Enable GPIO pins
P1DIR |= redLED;

P9DIR |= greenLED;
P1OUT &= ~redLED;
P9OUT &= ~greenLED;

//Pin diversion for Red LED--PWM
P1DIR |= BIT0;
P1SEL1 &= ~BIT0;
P1SEL0 |= BIT0;
convert_32khz();

//Start Timer in up mode
TA0CCR0= 33-1; // @32Khz, 0.001 seconds (1000 Hz)
//ACLK, Divide by 1,
TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

//Configure Channel 1 for PWM
TA0CCTL1 |= OUTMOD_7; //Reset Set Output Pattern
TA0CCR1 = 2;  //change brightness of LED, 0-32
while(1){}
return;
}


//Function for Converting ACLK to 32Khz
void convert_32khz()
{
//By default, ACLK runs on LFMODCLK at 5Mhz/128= 39 Khz
//Reroute pins to LFXIN/LFXOUT functionality
PJSEL1 &= ~BIT4;
PJSEL0 |= BIT4;
//wait until oscillator fault flags are cleared
CSCTL0 = CSKEY; //unlock CS registers
do {
CSCTL5 &= ~LFXTOFFG; //local fault flag
SFRIFG1 &= ~OFIFG; //global fault flag
} while((CSCTL5 & LFXTOFFG) != 0 );
CSCTL0_H = 0; //Lock CS registers
return;
}

