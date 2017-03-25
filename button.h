#ifndef _BUTTON_H_
#define _BUTTON_H_

enum
{
	BTN_NONE = 0,
	BTN_POWER,
	BTN_MEMORY,
	BTN_BAND,
	BTN_MENU,
	BTN_DISPLAY,
	BTN_SELECT,
	BTN_CAT_LEFT,
	BTN_CAT_RIGHT,
	BTN_ENC_LEFT,
	BTN_ENC_RIGHT

	/* Number buttons are '0' through '9' */
};

/* The most recently pressed button.
 * The button code never clears this.  Set it to BTN_NONE when the
 * button press has been handled.
 */
extern volatile char btn_press;

/* The button that is currently down.
 * Some buttons take priority over others, but the order is undefined.
 * This is BTN_NONE if no button is being held down.
 */
extern volatile char btn_down;

/* Encoder ticks.  You may set this with interrupts disabled.
 * It is automatically changed as the encoder is turned.
 */
extern volatile char encoder_count;

void btn_init(void);

#endif /* _BUTTON_H_ */

