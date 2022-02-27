#include <msp430.h>
#include <math.h>
#include "msp.h"
#include "lcd.h"
#include "fft.h"


int adcbuff[128] = {};
float vpp;
float thd;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    InitADC();
    InitSystemClock();
    P1DIR |= BIT1+BIT2+BIT3+BIT4;
    P2DIR |= BIT0+BIT1+BIT2+BIT3+BIT4;

    int i;
    float temp;
    while(1)
    {
        for(i=0;i<128;i++) StartADCConvert();
        get_vpp();
        _nop();
        FFTR_SEQ();
        FFTR();
        THD();
        initial_lcd();
        clear_screen();
        display_GB2312_string(5,1,"THD:");
        display_float(5,32,temp);
        display_GB2312_string(5,75,"%");
        display_GB2312_string(7,1,"Vpp:");
        display_float(7,32,vpp);
        display_GB2312_string(7,75,"V");
        delay(20000);
        _nop();
    }
//  return 0;
}
