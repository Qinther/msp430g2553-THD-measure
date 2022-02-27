#include <msp430.h>
#include "msp.h"

extern int adcbuff[128];
void StartADCConvert(void)
{
      /*开始转换*/
      ADC10CTL0 |= ADC10SC|ENC;
      /*等待转换完成*/
      while(ADC10CTL1&ADC10BUSY);

}
/*ADC配置*/
void InitADC(void){

    /*设置ADC时钟MCLK*/
      ADC10CTL1 |= ADC10SSEL_0;
      /*ADC 2分频*/
      ADC10CTL1 |= ADC10DIV_7;
      /*设置ADC基准源*/
      ADC10CTL0 |= SREF_0;
      /*设置ADC采样保持时间16CLK*/
      ADC10CTL0 |= ADC10SHT_0;
      /*设置ADC采样率200k*/
      ADC10CTL0 |= ADC10SR;
      /*ADC基准选择2.5V*/
      ADC10CTL0 |= REF2_5V;
      /*开启基准*/
      ADC10CTL0 |= REFON;
      /*选择ADC输入通道A4*/
      ADC10CTL1 |= INCH_0;
      /*允许A4模拟输入*/
      ADC10AE0 |= 0;

      /*DTC传输模式*/
      ADC10DTC0 |= ADC10CT;
      /*传输次数*/
      ADC10DTC1 = 128;
      /*起始地址*/
      ADC10SA = (int)(adcbuff);

      /*开启ADC*/
      ADC10CTL0 |= ADC10ON;

      /*允许转换*/
      ADC10CTL0 |= ENC;
}

void InitSystemClock(void){
    /*配置DCO为1MHz*/
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    /*配置SMCLK的时钟源为DCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK的分频系数置为1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}

