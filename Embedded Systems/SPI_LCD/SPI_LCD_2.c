// Code to print to the LCD pixel display on the Educational BoosterPack

#include <msp430fr6989.h>
#include "Grlib/grlib/grlib.h"          // Graphics library (grlib)
#include "LcdDriver/lcd_driver.h"       // LCD driver
#include <stdio.h>
#include <stdint.h>                    //Uint library

#define redLED BIT0
#define greenLED BIT7
#define button BIT1
#define TACTL TA0CTL    //necessary for timer config
#define TACCR0 TA0CCR0
#define BUT1 BIT1 //P1.1

void convert_32khz();  //convert to 32kHz function

volatile uint8_t n=0;  // timer/counter variable
static int state=0;  //current state for lcd
static int done=0;   //second toggle-for correctly counting
Graphics_Context g_sContext;        // Declare a graphic library context

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;     // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;         // Disable GPIO power-on default high-impedance mode

    P1DIR |= redLED;    P1OUT &= ~redLED;
    P9DIR |= greenLED;  P9OUT &= ~greenLED;
    P1DIR &= ~button; P1REN|=button; P1OUT|=button; // button, resistor, pullup
    P1IE |= BUT1; //1:enable interrupts
    P1IES |= BUT1; //1: interrupt on falling edge
    P1IFG &= ~BUT1; //0: Clear interrupt flags

//init backlight
            P2DIR |= BIT6;  //set as output and clear the rest
            P2SEL1 &= ~BIT6;
            P2SEL0 &= ~BIT6;
    //P2OUT &= ~BIT6; //select lowest brightness--OFF
    P2OUT |= BIT6; //select highest brightness--ON

    //Configure ACLK to 32kHz
    convert_32khz();
    //Configure Channel 0 for up mode with interrupt
    TACCR0= (32768-1); // 1 second period @ 32kHz
    TA0CCTL0 |= CCIE; //enable Channel 0 CCIE Bit
    TA0CCTL0 &= ~CCIFG; //clear channel 0 CCIFG Bit
    //Configure Timer_A: ACLK, Divide by 1, up Mode, clear TAR, enable interrupt for rollback event (leaves TAIE=0)
    TACTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
    TACTL &= ~TAIFG; //Clear flag at start

    //lastly enable GIE, avoid raising interrupts too soon
    _enable_interrupts();

    // Configure SMCLK to 8 MHz (used as SPI clock)
    CSCTL0 = CSKEY;                 // Unlock CS registers
    CSCTL3 &= ~(BIT4|BIT5|BIT6);    // DIVS=0
    CSCTL0_H = 0;                   // Relock the CS registers

    //set default state
    ////////////////////////////////////////////////////////////////////////////////////////////

      Crystalfontz128x128_Init();            // Initialize the display
      Crystalfontz128x128_SetOrientation(0); // Set the screen orientation
      Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128); // Initialize the context
      extern const tImage logo4BPP_UNCOMP;  //definition for image
//display  image
      Graphics_drawImage( &g_sContext, &logo4BPP_UNCOMP, 0,0);

while(1){} //infinite loop

}


#pragma vector = PORT1_VECTOR //Link the ISR to the Vector
__interrupt void P1_ISR()
{
if( (P1IFG&BUT1) !=0 ) {  //when button1 is pressed
    //toggle state flags
if( state ==0){ state=1;}




else if( state==1){ state=0;}

P1IFG &= ~BUT1; //clear flag (shared)
if( state==0  ) //first screen
    {
    done=0;  //secondtoggle, dont worry about counter

    P1OUT |= redLED;
    P9OUT &= ~greenLED;

    ////////////////////////////////////////////////////////////////////////////////////////////
        extern const tImage logo4BPP_UNCOMP;
//image stuff
       Graphics_drawImage( &g_sContext, &logo4BPP_UNCOMP, 0,0);
       state=0;

        return;
    }



else if( state==1)
    {
    done=1;

    P9OUT |= greenLED;
    P1OUT &= ~redLED;

    ////////////////////////////////////////////////////////////////////////////////////////////
               Graphics_Context g_sContext;        // Declare a graphic library context
               Crystalfontz128x128_Init();         // Initialize the display

               // Set the screen orientation
               Crystalfontz128x128_SetOrientation(0);

               // Initialize the context
               Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

               // Set background and foreground colors first before clearing the screen
                      Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_TURQUOISE);
                      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_CYAN    );

               // Clear the screen
               Graphics_clearDisplay(&g_sContext);
               ////////////////////////////////////////////////////////////////////////////////////////////
               state=1;
               char mystring[20];

               GrContextFontSet(&g_sContext, &g_sFontCmss12);
               Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);

               GrContextFontSet(&g_sContext, &g_sFontCmtt12);
               sprintf(mystring, "EEL 4742 Lab!");
               Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 55, OPAQUE_TEXT);


               //draw outline circle
               Graphics_drawCircle(&g_sContext, 10,10, 5); //x,y, radius

               Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_FIRE_BRICK    ); //change foreground before filled circle

               //draw filled circle
               Graphics_fillCircle(&g_sContext, 10, 20, 5);
               Graphics_setForegroundColor(&g_sContext,  GRAPHICS_COLOR_PINK); //change foreground before filled circle
               //draw unfilled rectangle
           struct Graphics_Rectangle R1 = {120, 10, 125, 20}; //struct rectangle for API
           Graphics_drawRectangle( &g_sContext, &R1 );

           struct Graphics_Rectangle R2 = {120, 20, 125, 30}; //struct rectangle for API
           //new color
           Graphics_setForegroundColor(&g_sContext,  GRAPHICS_COLOR_MEDIUM_ORCHID); //change foreground before filled circle
           Graphics_fillRectangle(&g_sContext, &R2);

           Graphics_setForegroundColor(&g_sContext,  GRAPHICS_COLOR_LINEN); //change foreground before filled circle
           Graphics_drawLineH(&g_sContext, 30, 100, 20);

           state=1;

           return;
    }
}

}




//**********ISR********//
//*********************//
#pragma vector = TIMER0_A0_VECTOR //Link the ISR to the Vector
__interrupt void T0A0_ISR()
{
//Toggle both LEDs



    if(  done==1){


        Graphics_Context g_sContext;        // Declare a graphic library context
        char mystring[20];
        Crystalfontz128x128_SetOrientation(0);
                       // Initialize the context
                       Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);


        if(n<10) {sprintf(mystring, "  %d", n);}

        else if(n<100) {sprintf(mystring, " %d", n);}

        else { sprintf(mystring, "%d", n);}

        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_TURQUOISE);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_CYAN    );


        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);


        Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 100, OPAQUE_TEXT);
        //done=0;

    }

    //print rectangle, print #
    n++;  //increment counter
P1OUT ^= BIT0;
P9OUT ^= BIT7;
//Hardware clears the flag (CCIFG in TA0CCTL0)
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
