//3.3--Stop Watch for set interval (2 settings) using Continuous Mode
//Feature: Add two different amounts of time to the   timer (kinda like a reset)
#include <msp430.h> 
#define redLED BIT0   //red LED at P1.0
#define greenLED BIT7 //green LED at  P9.7
#define BUT1 BIT1     //Button 1 at P1.1
#define BUT2 BIT2     //BUtton 2 at P1.2


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //clear lock and allow new code re-flash/enable GPIO Pins

    P1DIR |= redLED;       //direct pin as output
    P9DIR |= greenLED;     //direct pin as output

    P1DIR &= ~(BUT1|BUT2);            //direct pins as inputs
    P1REN |= (BUT1 | BUT2);           //enable built-in resistor for P1.1 and P1.2
    P1OUT |= (BUT1 | BUT2);           //set resistor as pull-up to Vcc for P1.1 and P1.2

    P1OUT &= ~redLED;  //turn LED OFF
    P9OUT &= ~greenLED;//turn LED OFF

    //Configure ACLK to 32kHz
    //convert_32khz();

    //Configure Timer_A: SMCLK, Divide by 8, Continuous Mode, clear TAR
    TA0CTL = TASSEL_2 | ID_3 | MC_2 | TACLR;
    TA0CTL &= ~TAIFG; //Clear flag at start
                      //put desired low value in parantheses for timer_1
    unsigned int timer_1 = 2*(10); //set stopwatch low duration (+1 to desired value, takes a second to start the stopwatch) (each two toggles is one second)
    unsigned int timer_2 = 2*(timer_1); //set stopwatch high duration for 2*(low duration)
    unsigned int i=0; //counter for loops
    unsigned int j=0; //counter for wait loops


    for(;;){
            P1OUT &= ~redLED;  //turn LED OFF
            P9OUT &= ~greenLED;//turn LED OFF

        //if only switch 1 is pressed, activate low duration
        if((P1IN&(BUT1|BUT2)) == BUT2 )
                {
            P1OUT |= redLED;  //turn LED ON
                for(i=0; i< timer_1; i++)
                    {
                    //wait for flag, wait 2 cycles.
                    for(j=0; j<2; j++){ while( (TA0CTL&TAIFG) ==0 ){} }

                    TA0CTL &= ~TAIFG; //clr flag
                    if(i==(timer_1)-20) //2cycles
                    {
                        P9OUT ^= greenLED;
                        for(j=0; j<4; j++){ while( (TA0CTL&TAIFG)==0){} }
                        P9OUT ^= greenLED;
                    }

                    if((P1IN&BUT2)==0)
                     {
                      //add 10 seconds to timer
                      timer_1+=20; //2 cycles per second
                      P9OUT ^= greenLED;
                      while((TA0CTL&TAIFG)==0){}
                      P9OUT ^=greenLED;
                     }

                    }
                P1OUT &= ~redLED;
                P9OUT |= greenLED;
                for(i=0; i<5; i++){
                    //wait for flag
                    for(j=0; j<2; j++){ while( (TA0CTL&TAIFG) ==0 ){} }
                }

                }
        //Button 2 only is pressed
        if((P1IN&(BUT1|BUT2)) == BUT1 )
                        {
                    P1OUT |= redLED;  //turn LED ON
                        for(i=0; i< timer_2; i++)
                            {

                            //wait for flag, 2 cycles
                            for(j=0; j<2; j++){ while( (TA0CTL&TAIFG) ==0 ){} }

                            TA0CTL &= ~TAIFG; //clr flag
                            if(i==(timer_2)-20) //2cycles
                              {
                                P9OUT ^= greenLED;
                                for(j=0; j<4; j++){ while( (TA0CTL&TAIFG)==0){} }
                                P9OUT ^= greenLED;
                               }

                            if((P1IN&BUT1)==0)
                               {
                                //add 10 seconds to timer
                                timer_2+=20; //2 cycles per second
                                P9OUT ^= greenLED;
                                while((TA0CTL&TAIFG)==0){}
                                P9OUT ^=greenLED;
                               }

                            }
                        P1OUT &= ~redLED;
                        P9OUT |= greenLED;
                        for(i=0; i<5; i++){
                            //wait for flag
                          for( j=0; j<2; j++){ while( (TA0CTL&TAIFG) ==0 ){} }
                        }

                        }
    }

    return 0;
}
