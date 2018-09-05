/*
 * uc1701_demo.cpp
 *
 * Created: 05.09.2018 23:51:04
 * Author : Heorenmaru
 */ 
#define F_CPU 8000000

typedef unsigned char byte;

#include <avr/io.h>
#include <util/delay.h>

#define lcd_ddr DDRC
#define lcd_port PORTC
#define lcd_sck 0
#define lcd_sda 1
#define lcd_rs 2
#define lcd_rst 3
#define lcd_cs 4

#define lcd_hddr DDRB
#define lcd_hport PORTB
#define lcd_pwr 0
#define lcd_led 1

void lcd_data()
{
	lcd_port |= (1<<lcd_rs);
	lcd_port &= ~(1<<lcd_cs);
}
void lcd_cmd()
{
	lcd_port &= ~(1<<lcd_rs);
	lcd_port &= ~(1<<lcd_cs);
}

void lcd_end()
{
	lcd_port |= (1<<lcd_cs);
}

void lcd_write(byte lcd_d)
{
	for (byte i = 8; i>0; i--)
	{
		lcd_port &= ~(1<<lcd_sck);
		if ((lcd_d & (1<<(i-1)))>0)
		{
			lcd_port |= (1<<lcd_sda);
		} 
		else
		{
			lcd_port &= ~(1<<lcd_sda);	
		}
		_delay_ms(1);
		lcd_port |= (1<<lcd_sck);
		_delay_ms(1);
	}
}
 
void lcd_init()
{
	lcd_ddr |= (1<<lcd_sck)|(1<<lcd_sda)|(1<<lcd_rs)|(1<<lcd_rst)|(1<<lcd_cs);
	lcd_hddr |= (1<<lcd_pwr)|(1<<lcd_led);
	lcd_hport |= (1<<lcd_pwr)|(1<<lcd_led);
	
	lcd_port |= (1<<lcd_sck);
	lcd_port &= ~(1<<lcd_rst);
	_delay_ms(10);
	lcd_port |= (1<<lcd_rst);
	_delay_ms(10);
	lcd_port |= (1<<lcd_cs);
	_delay_ms(1);
	lcd_port &= ~(1<<lcd_cs);
	
	
	_delay_ms(50);
	lcd_cmd();
	
	lcd_write(0xE2);              // Reset
	lcd_write(0x40);              // Set display start line to 0
	lcd_write(0xA1);              // Set SEG direction
	lcd_write(0xC0);              // Set COM direction
	lcd_write(0xA2);              // Set bias = 1/9
	lcd_write(0x2C);              // Boost ON
	lcd_write(0x2E);              // Voltage regulator on
	lcd_write(0x2F);              // Voltage follower on
	lcd_write(0xF8);              // Set booster ratio
	lcd_write(0x00);
	lcd_write(0x23);              // Set resistor ratio
	lcd_write(0x81);              // Set contrast to
	lcd_write(0x28);              // 40
	lcd_write(0xEE);              // Set cursor update -> after write, column cursor will be updated (rows will not!)
	lcd_write(0xAC);              // Disable static indicator
	lcd_write(0x00);
	lcd_write(0xA6);              // Disable inverse
	lcd_write(0xAF);              // Display enable
	lcd_write(0xA5);            // display all points
	lcd_write(0xA4);              // clear
	
	lcd_end();
}



// void UC1701::setContrast(char contrast)
// {
// 	_lcd_cs->write(0);              // enable SPI
// 	_lcd_cd->write(0);              // command mode
// 	_lcd->write(0x81);              // command to set contrast
// 	_lcd->write(contrast);          // set contrast
// 	_lcd_cs->write(1);
// 	_lcd_cd->write(1);
// }
// 
 void lcd_setCursor(byte column, byte line)
 {
 	int i, j;
 	column = column+4;
 
 	i=(column&0xF0)>>4;
 	j=column&0x0F;
 	lcd_cmd();
 	lcd_write(0xb0+line);
 	lcd_write(0x10+i);
 	lcd_write(j);
 	lcd_end();
 }
// 
// void UC1701::clear()
// {
// 	_lcd_cs->write(0);
// 	for(unsigned short j = 0; j < 8; j++) {
// 		UC1701::setCursor(0, j);
// 		for(unsigned short i = 0; i < 128 ; i++) {
// 			_lcd->write(0x00);
// 		}
// 	}
// 
// 	UC1701::setCursor(0, 0);
// 	_lcd_cs->write(1);
// }

int main(void)
{
	lcd_init();
    /* Replace with your application code */
	
	lcd_setCursor(0,0);
	lcd_data();
	for (int i=0;i<128;i++)
	{
		lcd_write(0xFF);	
	}
	lcd_end();
	lcd_setCursor(0,1);
	lcd_data();
	for (int i=0;i<128;i++)
	{
		lcd_write(0xEF);
	}
	lcd_end();
	
	_delay_ms(5000);
	lcd_hport &= ~ (1<<lcd_led);
    while (1) 
    {
    }
}

