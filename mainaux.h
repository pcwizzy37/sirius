#ifndef _MAINAUX_H_
#define _MAINAUX_H_
#include "lcd.h"


void draw_bar(uint8_t x, uint8_t y, uint8_t w)
{
	uint8_t byte;

	lcd_gotoxy(x, y);

	x &= 7;
	byte = 0x0f << x;
	byte |= 0x0f >> (8 - x);

	do
	{
		lcd_write_data(byte);

		if (byte & 1)
		{
			byte >>= 1;
			byte |= 0x80;
		} else {
			byte >>= 1;
		}
	} while (--w);
}





void draw_frame()
{
	uint8_t y;

	draw_bar(0, 0, 132);

	for (y = 1; y < 7; y++)
	{
		draw_bar(0, y, 8);
		draw_bar(124, y, 8);
	}

	draw_bar(0, 7, 132);
}

void center_text(PGM_P str, uint8_t y)
{
	uint8_t x;

	x = 132 / 2 - strlen_P(str) * 4;
	lcd_gotoxy(x, y);
	lcd_print(str);
}

void draw_header(PGM_P str)
{
	lcd_gotoxy(0, 0);
	draw_bar(0, 0, 132);
	lcd_reverse_text(1);
	center_text(str, 0);
	lcd_reverse_text(0);
}
#endif
