#include <msp430.h>
#include "lcd.h"

unsigned long fontaddr=0;

void initial_lcd()
{
    RESET_L; //低电平复位
    delay(100);
    RESET_H; //复位完毕
    delay(100);
    transfer_command_lcd(0xe2); //软复位
    delay(5);
    transfer_command_lcd(0x2c); //升压步聚 1
    delay(50);
    transfer_command_lcd(0x2e); //升压步聚 2
    delay(50);
    transfer_command_lcd(0x2f); //升压步聚 3
    delay(5);
    transfer_command_lcd(0x23); //粗调对比度，可设置范围 0x20～0x27
    transfer_command_lcd(0x81); //微调对比度
    transfer_command_lcd(0x28); //微调对比度的值，可设置范围 0x00～0x3f
    transfer_command_lcd(0xa2); //1/9 偏压比（bias）
    transfer_command_lcd(0xc8); //行扫描顺序：从上到下
    transfer_command_lcd(0xa0); //列扫描顺序：从左到右
    transfer_command_lcd(0x40); //起始行：第一行开始
    transfer_command_lcd(0xaf); //开显示
}
void delay(int n_ms)
{
    int j,k;
    for(j=0;j<n_ms;j++)
    for(k=0;k<110;k++);
}
void delay_us(int n_us)
{
    int j,k;
    for(j=0;j<n_us;j++)
    for(k=0;k<1;k++);
}
void transfer_command_lcd(int data1)
{
    char i;
    CS_L;
    RS_L;
    for(i=0;i<8;i++)
    {
        SCLK_L;
        //delay_us(10); //加少量延时
        if(data1&0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        SCLK_H;
        //delay_us(10); //加少量延时
        data1=data1<<=1;
    }
    CS_H;
}
void transfer_data_lcd(int data1)
{
    char i;
    CS_L;
    RS_H;
    for(i=0;i<8;i++)
    {
        SCLK_L;
        if(data1&0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        SCLK_H;
        data1=data1<<=1;
    }
    CS_H;
}
void clear_screen()
{
    unsigned char i,j;
    for(i=0;i<9;i++)
    {
        transfer_command_lcd(0xb0+i);
        transfer_command_lcd(0x10);
        transfer_command_lcd(0x00);
        for(j=0;j<132;j++)
        {
            transfer_data_lcd(0x00);
        }
    }
}
void lcd_address(unsigned int page,unsigned int column)
{
    column=column-0x01;
    transfer_command_lcd(0xb0+page-1); //设置页地址，每 8 行为一页，全屏共 64 行，被分成 8 页
    transfer_command_lcd(0x10+(column>>4&0x0f)); //设置列地址的高 4 位
    transfer_command_lcd(column&0x0f); //设置列地址的低 4 位
}
void display_128x64(unsigned char *dp)
{
    unsigned int i,j;
    for(j=0;j<8;j++)
    {
        lcd_address(j+1,1);
        for (i=0;i<128;i++)
        {
            transfer_data_lcd(*dp); //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1
            dp++;
        }
    }
}
void display_GB2312_string(unsigned char page,unsigned char column,unsigned char *text)
{
    unsigned char i= 0;
    while((text[i]>0x00))
    {
        if(((text[i]>=0xb0) && (text[i]<=0xf7)) && (text[i+1]>=0xa1))
        {
            fontaddr = (text[i]- 0xb0)*94;
            fontaddr += (text[i+1]-0xa1)+846;
            fontaddr = (unsigned long)(fontaddr*32);
            get_and_write_16x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
            i+=2;
            column+=16;
        }
        else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
        {
            fontaddr = (text[i]- 0xa1)*94;
            fontaddr += (text[i+1]-0xa1);
            fontaddr = (unsigned long)(fontaddr*32);

            get_and_write_16x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
            i+=2;
            column+=16;
        }
        else if((text[i]>=0x20) &&(text[i]<=0x7e))
        {
            fontaddr = (text[i]- 0x20);
            fontaddr = (unsigned long)(fontaddr*16);
            fontaddr = (unsigned long)(fontaddr+0x3cf80);
            get_and_write_8x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
            i+=1;
            column+=8;
        }
        else
            i++;
    }
}
void display_string_5x8(unsigned char page,unsigned char column,unsigned char *text)
{
    unsigned char i= 0;
    while((text[i]>0x00))
    {
        if((text[i]>=0x20) &&(text[i]<=0x7e))
        {
            fontaddr = (text[i]- 0x20);
            fontaddr = (unsigned long)(fontaddr*8);
            fontaddr = (unsigned long)(fontaddr+0x3bfc0);
            get_and_write_5x8(page,column);
            i+=1;
            column+=6;
        }
        else
            i++;
    }
}
void get_and_write_5x8(unsigned char page,unsigned char column)
{
    unsigned char i,disp_data;
    ROM_CS_L;
    send_command_to_ROM(0x03);
    send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高 8 位,共 24 位
    send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中 8 位,共 24 位
    send_command_to_ROM(fontaddr&0xff); //地址的低 8 位,共 24 位
    lcd_address(page,column);
    for(i=0; i<5; i++ )
    {
        disp_data=get_data_from_ROM();
        transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
    }
    ROM_CS_H;
}
void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp)
{
    unsigned int i,j;
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<16;i++)
        {
            transfer_data_lcd(*dp); //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1
            dp++;
        }
    }
}
void get_and_write_16x16(unsigned long fontaddr,unsigned char page,unsigned char column)
{
    unsigned char i,j,disp_data;
    ROM_CS_L;
    send_command_to_ROM(0x03);
    send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高 8 位,共 24 位
    send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中 8 位,共 24 位
    send_command_to_ROM(fontaddr&0xff); //地址的低 8 位,共 24 位
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for(i=0; i<16; i++ )
        {
            disp_data=get_data_from_ROM();
            transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
        }
    }
    ROM_CS_H;
}
void get_and_write_8x16(unsigned long fontaddr,unsigned char page,unsigned char column)
{
    unsigned char i,j,disp_data;
    ROM_CS_L;
    send_command_to_ROM(0x03);
    send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高 8 位,共 24 位
    send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中 8 位,共 24 位
    send_command_to_ROM(fontaddr&0xff); //地址的低 8 位,共 24 位
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for(i=0; i<8; i++ )
        {
            disp_data=get_data_from_ROM();
            transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
        }
    }
    ROM_CS_H;
}
static unsigned char get_data_from_ROM()
{
    unsigned char i;
    unsigned char ret_data=0;
    for(i=0;i<8;i++)
    {
        ROM_OUT_H;
        ROM_SCK_L;
        //delay_us(1);
        ret_data=ret_data<<1;
        if(P1IN&0x04) ret_data=ret_data+1;
        else ret_data=ret_data+0;
        ROM_SCK_H;
        //delay_us(1);
    }
    return(ret_data);
}
void send_command_to_ROM( unsigned char datu )
{
    unsigned char i;
    for(i=0;i<8;i++ )
    {
        ROM_SCK_L;
        delay_us(10);
        if(datu&0x80){ROM_IN_H;}
        else{ROM_IN_L;}
        datu = datu<<1;
        ROM_SCK_H;
        delay_us(10);
    }
}

void display_float(unsigned int page, unsigned int column, float num)
{
    int temp,i;
    for(i=0;i<5;i++)
    {

        if(i == 2)
        {
            display_GB2312_string(page,i*8 + 1 + column,".");
        }
        else
        {
            temp = num / 10;
            num = (num - temp*10) * 10;
            if(i != 0 || temp != 0)
            {
                switch (temp)
                {
                case 0: display_GB2312_string(page,i*8 + 1 + column,"0");break;
                case 1: display_GB2312_string(page,i*8 + 1 + column,"1");break;
                case 2: display_GB2312_string(page,i*8 + 1 + column,"2");break;
                case 3: display_GB2312_string(page,i*8 + 1 + column,"3");break;
                case 4: display_GB2312_string(page,i*8 + 1 + column,"4");break;
                case 5: display_GB2312_string(page,i*8 + 1 + column,"5");break;
                case 6: display_GB2312_string(page,i*8 + 1 + column,"6");break;
                case 7: display_GB2312_string(page,i*8 + 1 + column,"7");break;
                case 8: display_GB2312_string(page,i*8 + 1 + column,"8");break;
                case 9: display_GB2312_string(page,i*8 + 1 + column,"9");break;
                }
            }
        }

    }
}
