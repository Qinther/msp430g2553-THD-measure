#include <msp430.h>
#include <math.h>
#include "msp.h"
#include "lcd.h"
#include "fft.h"


int adcbuff[128] = {}; // 采样结果数组
float vpp;
float thd;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    InitADC(); // 初始化ADC10采样
    InitSystemClock(); // 初始化系统时钟
    P1DIR |= BIT1+BIT2+BIT3+BIT4;
    P2DIR |= BIT0+BIT1+BIT2+BIT3+BIT4; // LCD引脚设置
    initial_lcd(); // 初始化LCD
    int i;
    while(1)
    {
        for(i=0;i<128;i++) StartADCConvert(); // 开始采样
        get_vpp(); // 计算峰峰值
        FFTR_SEQ();
        FFTR(); // 傅里叶变换结束
        THD(); // 计算失真度
        clear_screen(); // 清屏
        display_GB2312_string(5,1,"THD:");
        display_float(5,32,thd);
        display_GB2312_string(5,75,"%");
        display_GB2312_string(7,1,"Vpp:");
        display_float(7,32,vpp);
        display_GB2312_string(7,75,"V");
        delay(20000);
    }
//  return 0;
}
