#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include <avr/pgmspace.h>

/* Special characters */
#define CH_NORMAL	0x80
#define CH_REVERSE	0x81

#define STR_NORMAL	"\x80"
#define STR_REVERSE	"\x81"

/* Initializes the screen */
void lcd_init(void);

/* Read the datasheet before using these. */
void lcd_write_command(unsigned char ch);
uint8_t lcd_read_status(void);

/* Writes one byte to display RAM */
void lcd_write_data(unsigned char ch);

/* Sets the backlight brightness.
 * level is 0-255.
 */
void lcd_set_backlight(uint8_t level);

/* Clears the screen */
void lcd_clear(void);

/* Sets the display address.
 *
 * x is a column number (0-131).
 * y is a page number (0-8, except page 8 only has one pixel).
 */
void lcd_gotoxy(uint8_t x, uint8_t y);

/* Turns reverse-video text on/off.
 *
 * If <enable> is nonzero, then text is printed in reverse video (all pixels inverted).
 * If <enable> is zero, then text is printed normally.
 */
void lcd_reverse_text(uint8_t enable);

/* Prints one character at the current location */
void lcd_putch(char ch);

/* Prints a string at the current location */
void lcd_print(PGM_P str);

#endif /* _LCD_H_ */

