#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "button.h"

volatile char btn_press;
volatile char btn_down;
volatile char encoder_count;

static uint8_t scan_column, all_rows, enc_state;

static prog_uchar btn_row_a[] = {'1', '2', '3', '4', '5', '6'};
static prog_uchar btn_row_b[] = {'7', '8', '9', '0', BTN_CAT_LEFT, BTN_CAT_RIGHT};
static prog_uchar btn_row_c[] =
{
	BTN_POWER,
	BTN_MEMORY,
	BTN_BAND,
	BTN_MENU,
	BTN_DISPLAY,
	BTN_SELECT
};

ISR(TIMER0_OVF_vect)
{
	uint8_t row_bits;
	char down;

	/* Get row inputs */
	row_bits = PINF;
	all_rows &= row_bits;

	/* Determine which button is down, if any */
	down = BTN_NONE;
	if (!(row_bits & 0x80))
	{
		down = __LPM(btn_row_b + scan_column);
	} else if (!(row_bits & 0x40))
	{
		down = __LPM(btn_row_a + scan_column);
	} else if (!(row_bits & 0x20))
	{
		down = __LPM(btn_row_c + scan_column);
	}

	if (down != BTN_NONE)
	{
		/* Set btn_press if no button was down on the previous scan */
		if (btn_down == BTN_NONE)
		{
			btn_press = down;
		}

		/* Set btn_down if a button is down in this column */
		btn_down = down;
	}

	/* Go to the next column */
	if (scan_column == 5)
	{
		if ((all_rows & 0xe0) == 0xe0)
		{
			/* No buttons were pressed during this scan */
			btn_down = BTN_NONE;
		}

		all_rows = 0xe0;

		scan_column = 0;
	} else {
		scan_column++;
	}

	/* Keep pullups for encoder inputs on.
	 * Drive the current column low and all other columns high.
	 */
	PORTA = 0xff ^ (1 << scan_column);

	/* Check the encoder */
	enc_state >>= 2;
	enc_state |= PINA & 0xc0;
	enc_state &= 0xf0;

	if (enc_state == 0xe0)
	{
		encoder_count--;
		btn_press = BTN_ENC_LEFT;
	} else if (enc_state == 0xd0)
	{
		encoder_count++;
		btn_press = BTN_ENC_RIGHT;
	}
}

void btn_init()
{
	btn_press = BTN_NONE;
	btn_down = BTN_NONE;
	encoder_count = 0;
	all_rows = 0xe0;

	/* Drive the first column */
	scan_column = 1;
	PORTA = 0xfe;

	/* Timer 0 is used for column scanning */
	TCCR0 = 0x05;

	/* Turn on the timer 0 overflow interrupt */
	TIMSK |= 1 << TOIE0;
}

