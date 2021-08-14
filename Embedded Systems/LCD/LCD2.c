//Lab 5.2: Timer---Up Mode, 0-65535
//After 65535, roll back to 0
#include <msp430.h> 
#define redLED BIT0    //red LED at P1.0
#define greenLED BIT7  //green LED at P9.7

void Initialize_LCD();

void display_num_lcd(unsigned int n);

//Function Prototype for 32Khz ACLK
void convert_32khz();

//array of digit shapes, defined globally---0 through 9
const unsigned char LCD_Num[10] = { 0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE0, 0xFF, 0xE7};


int main(void)
{
    //int for display LCD function: 16 Bit unsigned int
    volatile unsigned int n=1;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

    P1DIR |= redLED; //set as output
    P9DIR |= greenLED;
    P1OUT &= ~redLED;    //red ON
    P9OUT &= ~greenLED; //green OFF

    //Set timer Period
             TA0CCR0 = (32768-1); // @32Khz, 1 second
             //Configure Timer_A: ACLK, Divide by 1, Up Mode, clear TAR
             TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
             TA0CTL &= ~TAIFG; //Clear flag at start


    //Initialize Timer
    //Configure ACLK to 32kHz
      convert_32khz();


    //initialize LCD_C module--prepare LCD controller within uC
    Initialize_LCD();


         P1OUT |= redLED;    //red ON
         P9OUT &= ~greenLED; //green OFF

    //flash the red and green LEDs
    for(;;){

        while((TA0CTL&TAIFG) == 0){} //delay loop
        TA0CTL &= ~TAIFG; //dont forget to clear flag!!!
        display_num_lcd(n);
        P1OUT ^=redLED;
        P9OUT ^= greenLED;
        if(n==65535)
        {
            n=0; //reset and then increment will make it 1 again
        }
         n++;

    }

    return 0;
}


void display_num_lcd(unsigned int n)
{
    int  i=0;
   unsigned char * ptr [6] = { &LCDM8, &LCDM15, &LCDM19 , &LCDM4, &LCDM6, &LCDM10};
    int digit;

    do{
        digit = n%10;
        *ptr[i] =LCD_Num[digit];
        n=n/10;
        i++;
    } while (n>0);

    while (i<8)
    {
        *ptr[i] = 0;
        i++;
        }
}

//Function for Converting ACLK to 32Khz
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



// Initializes the LCD_C module
// *** Source: Function obtained from MSP430FR6989’s Sample Code ***
void Initialize_LCD() {
    PJSEL0 = BIT4 | BIT5; // For LFXT

// Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

// Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8; // Unlock CS registers
    CSCTL4 &= ~LFXTOFF; // Enable LFXT

    do {
    CSCTL5 &= ~LFXTOFFG; // Clear LFXT fault flag
    SFRIFG1 &= ~OFIFG;
        }while (SFRIFG1 & OFIFG); // Test oscillator fault flag

    CSCTL0_H = 0; // Lock CS registers

// Initialize LCD_C
// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

// VLCD generated internally,
// V2-V4 generated internally, v5 to ground
// Set VLCD voltage to 2.60v
// Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
    LCDCCPCTL = LCDCPCLKSYNC; // Clock synchronization enabled
    LCDCMEMCTL = LCDCLRM; // Clear LCD memory

    //Turn LCD on
    LCDCCTL0 |= LCDON;
return;
}
