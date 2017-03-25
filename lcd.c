/* The LCD controller is a Samsung S6B1713.
 * The screen is 132x65.
 *
 * Pixels are arranged in pages.  Each page is 8 lines.  The 9th page has only one line.
 * Writing a byte to display RAM writes 8 pixels *vertically* in the current page.
 * This is convenient for writing text.
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "font.h"

/* The port that contains all LCD control lines */
#define LCD_PORT	PORTD

/* Active low reset */
#define LCD_RESET	0x80

/* Register select: 0=Command, 1=Data */
#define LCD_RS		0x40

/* Active low write */
#define LCD_WR		0x20

/* Active low read */
#define LCD_RD		0x10

/* This is XOR'd with all bytes written by lcd_putch,
 * allowing text to be printed in reverse video.
 */
static uint8_t text_reverse = 0;

static uint8_t text_x, text_y;

static int lcd_putchar(char ch, FILE *stream)
{
	switch ((unsigned char)ch)
	{
		case '\n':
			text_y++;
			if (text_y == 8)
				text_y = 0;

			lcd_gotoxy(text_x, text_y);
			break;
		case CH_REVERSE:
			lcd_reverse_text(1);
			break;
		case CH_NORMAL:
			lcd_reverse_text(0);
			break;
		default:
			lcd_putch(ch);
			break;
	}

	return 0;
}

static FILE lcd_stdout = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

void lcd_reset()
{
	LCD_PORT &= ~LCD_RESET;
	_delay_us(20);
	LCD_PORT |= LCD_RESET;

	/* Wait for reset status to clear */
	PORTC = 0xff;
	DDRC = 0x00;

	LCD_PORT &= ~LCD_RS;
	LCD_PORT &= ~LCD_RD;
	loop_until_bit_is_clear(PINC, 4);

	LCD_PORT |= LCD_RD;

	DDRC = 0xff;
}

void lcd_write_command(unsigned char ch)
{
	/* Drive RS and /WR low */
	LCD_PORT &= ~LCD_RS;
	LCD_PORT &= ~LCD_WR;

	/* Drive data */
	PORTC = ch;
	__asm__ __volatile__ ("nop;nop");

	/* Drive /WR high */
	LCD_PORT |= LCD_WR;

	__asm__ __volatile__ ("nop;nop");
}

uint8_t lcd_read_status()
{
	uint8_t value;

	/* Turn off data bus drivers */
	PORTC = 0xff;
	DDRC = 0x00;

	/* Drive RS and /RD low */
	LCD_PORT &= ~LCD_RS;
	LCD_PORT &= ~LCD_RD;

	__asm__ __volatile__ ("nop;nop");
	value = PINC;

	/* Drive /RD high to end the read */
	LCD_PORT |= LCD_RD;

	/* Turn data drivers back on */
	DDRC = 0xff;

	return value;
}

void lcd_write_data(unsigned char ch)
{
	/* Drive RS high and /WR low */
	LCD_PORT |= LCD_RS;
	LCD_PORT &= ~LCD_WR;

	/* Drive data */
	PORTC = ch;
	__asm__ __volatile__ ("nop;nop");

	/* Drive /WR high */
	LCD_PORT |= LCD_WR;
	__asm__ __volatile__ ("nop;nop");
}

void lcd_init()
{
	stdout = &lcd_stdout;

	lcd_reset();

	/* Set initial display line to 0 */
	lcd_write_command(0x40);

	/* Turn on display (FIXME - this has no effect?) */
	lcd_write_command(0xaf);

	/* Turn off EON */
	lcd_write_command(0xa4);

	/* ADC select: Segment driver direction is reversed (132 -> 1) */
	lcd_write_command(0xa1);

	/* Normal display (not reversed) */
	lcd_write_command(0xa6);

	/* Power control: turn on all internal power circuits */
	lcd_write_command(0x2f);

	/* Regulator resistor select */
	lcd_write_command(0x26);

	/* FIXME - Delay? */

	/* Set reference voltage: FIXME - what does this mean? */
	lcd_write_command(0x81);
	lcd_write_command(0x25);

	/* FIXME - Delay? */

	/* Select LCD bias */
	lcd_write_command(0xa2);

	/* SHL select: Common driver direction is reversed (64 -> 1) */
	lcd_write_command(0xc8);

	/* Set initial display line (again?!) */
	lcd_write_command(0x40);
}


void lcd_set_backlight(uint8_t level)
{
	OCR1BH = 0;
	OCR1BL = level;
}

void lcd_clear()
{
	uint8_t x, y;

	/* Clear 8 lines (one page) at a time */
	for (y = 0; y < 9; y++)
	{
		lcd_write_command(0xb0 + y);
		lcd_write_command(0x10);
		lcd_write_command(0x00);

		for (x = 0; x < 132; x++)
			lcd_write_data(0x00);
	}
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	text_x = x;
	text_y = y;

	lcd_write_command(0xb0 + y);
	lcd_write_command(0x10 + (x >> 4));
	lcd_write_command(x & 0x0f);
}

void lcd_reverse_text(uint8_t enable)
{
	if (enable)
	{
		text_reverse = 0xff;
	} else {
		text_reverse = 0;
	}
}

void lcd_putch(char ch)
{
	uint8_t i;
	uint16_t offset = ch * 8;

	for (i = 0; i < 8; i++)
	{
		lcd_write_data(pgm_read_byte_near(font_data + offset) ^ text_reverse);

		offset++;
	}
}

void lcd_print(PGM_P str)
{
	char ch;

	while (1)
	{
		ch = pgm_read_byte_near(str);
		if (!ch)
			return;

		lcd_putch(ch);
		str++;
	}
}

