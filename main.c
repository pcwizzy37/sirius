#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "button.h"

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

int main()
{
	uint8_t x, y;

	PORTA = 0xff;
	PORTB = 0x0f;
	PORTC = 0x00;
	PORTD = 0x30;
	PORTE = 0x40;
	PORTF = 0xff;

	DDRA = 0x3f;
	DDRB = 0xf0;
	DDRC = 0xff;
	DDRD = 0xf9;
	DDRE = 0x00;
	DDRF = 0x00;

	/* Timer 1 is used for the LCD backlight and beeper */
	TCCR1A = 0x21;
	TCCR1B = 0x0a;

	btn_init();

	lcd_set_backlight(255);
	lcd_init();
	lcd_clear();

	sei();

	draw_header(PSTR("Button Test"));

	x = 0;
	y = 1;
	btn_press = BTN_NONE;
	while (1)
	{
		while (btn_press == BTN_NONE)
			;

		if (x == 16)
		{
			x = 0;
			y++;
			if (y == 8)
			{
				y = 0;
				lcd_clear();
			}
		}
		lcd_gotoxy(x * 8, y);

		switch (btn_press)
		{
			case BTN_POWER:
				lcd_putch('P');
				PORTD ^= 1;
				break;
			case BTN_MEMORY:
				lcd_putch('M');
				break;
			case BTN_BAND:
				lcd_putch('B');
				break;
			case BTN_MENU:
				lcd_putch('?');
				break;
			case BTN_DISPLAY:
				lcd_putch('D');
				break;
			case BTN_SELECT:
				lcd_putch('*');
				break;
			case BTN_CAT_LEFT:
				lcd_putch('<');
				break;
			case BTN_CAT_RIGHT:
				lcd_putch('>');
				break;
			case BTN_ENC_LEFT:
				lcd_putch('{');
				break;
			case BTN_ENC_RIGHT:
				lcd_putch('}');
				break;
			default:
				lcd_putch(btn_press);
				break;
		}
		btn_press = BTN_NONE;

		x++;
	}

#if 0
	draw_frame();

	lcd_reverse_text(1);
	center_text(PSTR("LAUNCH"), 0);
#endif

	return 0;
}

