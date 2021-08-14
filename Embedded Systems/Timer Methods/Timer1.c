// Using Timer_A with 2 channels
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
convert_32khz();

// Configure Channel 0
TA0CCR0 = 3277-1; // @ 32 KHz --> 0.1 seconds
TA0CCTL0 |= CCIE;
TA0CCTL0 &= ~CCIFG;
// Configure Channel 1 (write 3 lines similar to above)
TA0CCR1 =16384-1; //@ 32,768 Hz--> 0.5s
TA0CCTL1 |= CCIE;
TA0CCTL1 &= ~CCIFG;

// Configure timer (ACLK) (divide by 1) (continuous mode)
TA0CTL = TASSEL_1 | ID_0 | MC_2;

// Engage a low-power mode, automatically enables GIE
_low_power_mode_3();
return;
}

// ISR of Channel 0 (A0 vector)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void T0A0_ISR() {
P1OUT ^= redLED; // Toggle the red LED
TA0CCR0 += 3277; // Schedule the next interrupt
// Hardware clears Channel 0 flag (CCIFG in TA0CCTL0)
}
// ISR of Channel 1 (A1 vector) ... fill the vector name below
#pragma vector = TIMER0_A1_VECTOR
__interrupt void T0A1_ISR() {
 // Toggle the green LED
    P9OUT ^= greenLED;
 // Schedule the next interrupt
    TA0CCR1 += 16384;
 TA0CCTL1 &= ~CCIFG;
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

