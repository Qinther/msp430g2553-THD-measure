#include <msp430.h>
#include "msp.h"

extern int adcbuff[128];


// 开始ADC转换
void StartADCConvert(void)
{
      ADC10CTL0 |= ADC10SC|ENC; // 开始转换
      while(ADC10CTL1&ADC10BUSY); // 等待转换完成
}


// ADC配置
void InitADC(void)
{
      ADC10CTL1 |= ADC10SSEL_0; // ADC时钟源选择ADC10OSC
      ADC10CTL1 |= ADC10DIV_7; // 8分频
      ADC10CTL0 |= SREF_0; // 基准源选择
      ADC10CTL0 |= ADC10SHT_0; // 采样保持4clk
      ADC10CTL0 |= ADC10SR; // 采样率200k
      ADC10CTL0 |= REF2_5V; // 2.5V基准
      ADC10CTL0 |= REFON; // 开启2.5V基准
      ADC10CTL1 |= INCH_0; // 输入通道选择P1.0
      ADC10AE0 |= 0; // 允许模拟输入
      ADC10DTC0 |= ADC10CT; // DTC传输模式
      ADC10DTC1 = 128; // 传输次数
      ADC10SA = (int)(adcbuff); // 数组起始地址

      ADC10CTL0 |= ADC10ON; // 开启ADC
      ADC10CTL0 |= ENC; // 允许转换
}


// 系统时钟配置
void InitSystemClock(void)
{
    DCOCTL = CALDCO_8MHZ; // 配置DCO为8MHz
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL2 &= ~SELS; // SMCLK选择DCO
    BCSCTL2 &= ~(DIVS0 | DIVS1); // 1分频
}
