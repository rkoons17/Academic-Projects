//Created by Ryan Koons and Dean Pickett

//Function: Program that can convert a string of characters into Morse code.
//Project Implementation: TI MSP430GX Microcontroller
	//LED1 and LED2 are used to represent morse code.


#include <msp430g2553.h>    // header for MSP430 G2553
unsigned int unit=20000;  // counter as a global variable

void blink_fun(int);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  // stop the watchdog timer
    P1DIR |= 0x41;  // set the direction register for LED1 and LED2
    P1OUT &= 0xBE;  // initialize LED1 and LED2 to off

    int a,i,j;

    char string[] = "Aaa";

    int Morse[26][6] = {
                        {1,3,2,0,0,0},
                        {3,1,1,1,2,0},
                        {3,1,3,1,2,0},
                        {3,1,1,2,0,0},
                        {1,2,0,0,0,0},
                        {1,1,3,1,2,0},
                        {3,3,1,2,0,0},
                        {1,1,1,1,2,0},
                        {1,1,2,0,0,0},
                        {1,3,3,3,2,0},
                        {3,1,3,2,0,0},
                        {1,3,1,1,2,0},
                        {3,3,2,0,0,0},
                        {3,1,2,0,0,0},
                        {3,3,3,2,0,0},
                        {1,3,3,1,2,0},
                        {3,3,1,3,2,0},
                        {1,3,1,2,0,0},
                        {1,1,1,2,0,0},
                        {3,2,0,0,0,0},
                        {1,1,3,2,0,0},
                        {1,1,1,3,2,0},
                        {1,3,3,2,0,0},
                        {3,1,1,3,2,0},
                        {3,1,3,3,2,0},
                        {3,3,1,1,2,0}
    };

    for (;;) { //empty for loop is an infinite loop
        i=0;

        do{

            if((string[i] >= 'A') && (string[i] <= 'Z')){
                a = 32 + (int)string[i];
                j=0;

                while(Morse[a-97][j] != 0){
                    blink_fun(Morse[a-97][j]);
                    j++;
                }
            }else if(string[i] >= 'a' && string[i] <= 'z'){
                a = (int)string[i];
                j=0;

                while(Morse[a-97][j] != 0){
                    blink_fun(Morse[a-97][j]);
                    j++;
                }
            }else if(string[i] == ' '){
                blink_fun(4);
            }
            i++;

        }
        while(string[i] != 0);

       blink_fun(4);
       blink_fun(69);
       blink_fun(2);

    }

}

void blink_fun(int key)
{
    int i;

    if(key == 2 || key == 4)
    {
        for(i=1; i< key*unit; i++)  // create a delay between toggles
                    ;          // empty statement, do nothing
    }
    else if(key == 69)
    {
        P1OUT ^= 0x40;  // invert the state of LED2
        for(i=1; i< unit; i++)  // create a delay between toggles
                    ;          // empty statement, do nothing
        P1OUT ^= 0x40;  // invert the state of LED2
    }
    else
    {
        P1OUT ^= 0x01;// invert the state of LED1
        for(i=1; i< key*unit; i++)  // create a delay between toggles
                    ;          // empty statement, do nothing
        P1OUT ^= 0x01;// invert the state of LED1
        for(i=1; i< unit; i++)  // create a delay between toggles
                            ;          // empty statement, do nothing
    }
return;
}
