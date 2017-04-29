#ifndef _STAR_H_
#define _STAR_H_
#include "lcd.h"

void draw_star(uint8_t x, uint8_t y, uint8_t w)
{
	uint8_t byte;

	lcd_gotoxy(x, y);
	byte = 0x84;
	lcd_write_data(byte);
	x++;
	byte = 0xCC;
	lcd_write_data(byte);
	x++;
	byte = 0xFC;
	lcd_write_data(byte);
	x++;
	byte = 0x7E;
	lcd_write_data(byte);
	x++;
	byte = 0x3F;
	lcd_write_data(byte);
	x++;
	byte = 0x7E;
	lcd_write_data(byte);
	x++;
	byte = 0xFC;
	lcd_write_data(byte);
	x++;
	byte = 0xCC;
	lcd_write_data(byte);
	x++;
	byte = 0x84;
	lcd_write_data(byte);

while (--w);
}
#endif
