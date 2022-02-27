#include <msp430.h>
#include "msp.h"

extern int adcbuff[128];
void StartADCConvert(void)
{
      /*��ʼת��*/
      ADC10CTL0 |= ADC10SC|ENC;
      /*�ȴ�ת�����*/
      while(ADC10CTL1&ADC10BUSY);

}
/*ADC����*/
void InitADC(void){

    /*����ADCʱ��MCLK*/
      ADC10CTL1 |= ADC10SSEL_0;
      /*ADC 2��Ƶ*/
      ADC10CTL1 |= ADC10DIV_7;
      /*����ADC��׼Դ*/
      ADC10CTL0 |= SREF_0;
      /*����ADC��������ʱ��16CLK*/
      ADC10CTL0 |= ADC10SHT_0;
      /*����ADC������200k*/
      ADC10CTL0 |= ADC10SR;
      /*ADC��׼ѡ��2.5V*/
      ADC10CTL0 |= REF2_5V;
      /*������׼*/
      ADC10CTL0 |= REFON;
      /*ѡ��ADC����ͨ��A4*/
      ADC10CTL1 |= INCH_0;
      /*����A4ģ������*/
      ADC10AE0 |= 0;

      /*DTC����ģʽ*/
      ADC10DTC0 |= ADC10CT;
      /*�������*/
      ADC10DTC1 = 128;
      /*��ʼ��ַ*/
      ADC10SA = (int)(adcbuff);

      /*����ADC*/
      ADC10CTL0 |= ADC10ON;

      /*����ת��*/
      ADC10CTL0 |= ENC;
}

void InitSystemClock(void){
    /*����DCOΪ1MHz*/
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    /*����SMCLK��ʱ��ԴΪDCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK�ķ�Ƶϵ����Ϊ1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}

