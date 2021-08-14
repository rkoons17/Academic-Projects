//Lab 5.1--Print 16 bit unsigned int  to LCD
//Goal: Design a program that toggles the R/G LEDs and prints up to a 5  digit (16bit) number on the screen, no leading zeroes
#include <msp430.h> 
#define redLED BIT0    //red LED at P1.0
#define greenLED BIT7  //green LED at P9.7

void Initialize_LCD();

void display_num_lcd(unsigned int n); //function that correctly displays a passed int

//array of digit shapes, defined globally---0 through 9
const unsigned char LCD_Num[10] = { 0xFC, 0x60, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE0, 0xFF, 0xE7};


int main(void)
{
    //int for display LCD function: 16 Bit unsigned int, modify this variable to change LCD number
    volatile unsigned int n=65535;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       //enable GPIO Pins

	P1DIR |= redLED; //set as output
	P9DIR |= greenLED;
	P1OUT |= redLED;    //red ON
	P9OUT &= ~greenLED; //green OFF
	
	//initialize LCD_C module--prepare LCD controller within uC
	Initialize_LCD();

	//Line below can be used to clear all segments
	//LCDCMEMCTL= LCDCLRM;   //clear all segments

	//Display 430 on rightmost digits
	//LCDM19 = LCD_Num[4];  //display 4 at Leftmost (of 3 used) digits
	//LCDM15 = LCD_Num[3];
	//LCDM8 = LCD_Num[0];

	//call function to print int n on LCD
	display_num_lcd(n);

	//flash the red and green LEDs
	for(;;){
	    for(n=0; n<= 50000; n++){} //delay loop
	    P1OUT ^=redLED; //LEDs toggle
	    P9OUT ^= greenLED;
	}

	return 0;
}

//function called after LCD init. prints int n to LCD
void display_num_lcd(unsigned int n)
{
    int  i=0;  //ptr index for parsing through
   unsigned char * ptr [6] = { &LCDM8, &LCDM15, &LCDM19 , &LCDM4, &LCDM6, &LCDM10}; //ptr for parsing
    int digit; //current digit to pass to display
//do, while, need at least one iteration through (0 case)
    do{
        digit = n%10; //grab rightmost digit
        *ptr[i] =LCD_Num[digit]; //print digit to display
        n=n/10; //cut off rightmost digit
        i++; //move to next ptr for addressing LCD
    } while (n>0);

    //turn all other digits off, i keeps incrementing thorugh the display digits
    while (i<8)
    {
        ptr[i] = 0;
        i++;
        }
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
