#ifndef LCD_H_
#define LCD_H_

#define ROM_IN_H P1OUT|=BIT1;
#define ROM_IN_L P1OUT&=~BIT1;
#define ROM_OUT_H P1OUT|=BIT2;
#define ROM_OUT_L P1OUT&=~BIT2;
#define ROM_SCK_H P1OUT|=BIT3;
#define ROM_SCK_L P1OUT&=~BIT3;
#define ROM_CS_H P1OUT|=BIT4;
#define ROM_CS_L P1OUT&=~BIT4;
#define SCLK_H P2OUT|=BIT0;
#define SCLK_L P2OUT&=~BIT0;
#define SDA_H P2OUT|=BIT1;
#define SDA_L P2OUT&=~BIT1;
#define RS_H P2OUT|=BIT2;
#define RS_L P2OUT&=~BIT2;
#define RESET_H P2OUT|=BIT3;
#define RESET_L P2OUT&=~BIT3;
#define CS_H P2OUT|=BIT4;
#define CS_L P2OUT&=~BIT4;

void initial_lcd();
void delay(int n_ms);
void delay_us(int n_us);
void transfer_command_lcd(int data1);
void transfer_data_lcd(int data1);
void clear_screen();
void lcd_address(unsigned int page,unsigned int column);
void display_128x64(unsigned char *dp);
void display_GB2312_string(unsigned char page,unsigned char column,unsigned char *text);
void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp);
void get_and_write_16x16(unsigned long fontaddr,unsigned char page,unsigned char column);
void get_and_write_8x16(unsigned long fontaddr,unsigned char page,unsigned char column);
static unsigned char get_data_from_ROM();
void send_command_to_ROM( unsigned char datu );
void display_string_5x8(unsigned char page,unsigned char column,unsigned char *text);
void get_and_write_5x8(unsigned char page,unsigned char column);
void display_float(unsigned int page, unsigned int column, float num);

#endif /* LCD_H_ */
