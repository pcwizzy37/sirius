#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "lcd.h"
#include "button.h"
void let(char let){};
void sym(char sym){};
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
char let = ' ';
char sym = ' ';

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

	lcd_set_backlight(128);
	lcd_init();
	lcd_clear();

	sei();

	draw_header(PSTR("Nathaniel"));
	

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
			lcd_reverse_text(0);
			case BTN_POWER:
			draw_header(PSTR("Nathaniel"));
			x = 0;
			y = 1;
				break;
			case BTN_MEMORY:
				lcd_putch('*');
				x++;
				break;
			case BTN_BAND:
				if (sym == ' '){sym = '.';}
					else{if (sym == '.') {sym = ',';}
					else{if (sym == ','){sym = '?';}
						if (sym == '?'){sym = '!';}
						else{
						if (sym == '!'){sym = '(';}
						else{if (sym == '(') {sym = ')';}
							else{if (sym == ')'){sym = '$';}
							else{if (sym == '$') {sym = ' ';}}}}}}}
								lcd_putch(sym);
				break;
			case BTN_MENU:
				lcd_clear();
				break;
			case BTN_DISPLAY:
				draw_bar(0, 0, 132);
				draw_bar(0, 1, 132);
				draw_bar(0, 2, 132);
				draw_bar(0, 3, 132);
				draw_bar(0, 4, 132);
				draw_bar(0, 5, 132);
				draw_bar(0, 6, 132);
				draw_bar(0, 7, 132);
				y = 0;
				x = 0;
				break;
			case BTN_SELECT:
				if (let == ' ') {lcd_putch(sym);}
					else{lcd_putch(let);}
				x++;
				let = '|';
				lcd_putch(let);
				let = ' ';
				sym = ' ';
				
				break;
			case BTN_CAT_LEFT:
			x++;
			break;
			case BTN_CAT_RIGHT:
				if (x == 0)
				{if (y == 0) {}
					else {y--; x = 16;}
					}
				else {x--;}
				break;
			case BTN_ENC_LEFT:
				if (let == ' '){let = 'A';
					}
			else{if (let == 'A'){let = 'B';}
			else{
			if (let == 'B'){let = 'C';
			}
				else{if (let == 'C'){let = 'D';
				}
				else{if (let == 'D'){let = 'E';
					}else{
					if (let == 'E'){let = 'F';
					}
				else{
				if (let == 'F'){let = 'G';
				}
				else{if (let == 'G'){let = 'H';
					}else{
					if (let == 'H'){let = 'I';
					}
				else{
				if (let == 'I'){let = 'J';
				}
				else{if (let == 'J'){let = 'K';
					}else{
					if (let == 'K'){let = 'L';
					}
				else{if (let == 'L'){let = 'M';
					}else{
					if (let == 'M'){let = 'N';
					}else{
					if (let == 'N'){let = 'O';
					
					}else{if (let == 'O'){let = 'P';
						}else{
						if (let == 'P'){let = 'Q';
						}
						else{
						if (let == 'Q'){let = 'R';
						}
						else{if (let == 'R'){let = 'S';
							}else{
							if (let == 'S'){let = 'T';
							}
							else{
							if (let == 'T'){let = 'U';
							}
							else{if (let == 'U'){let = 'V';
								}else{
								if (let == 'V'){let = 'W';
								}
						else{
				if (let == 'W'){let = 'X';
				}
				else{if (let == 'X'){let = 'Y';
					}else{
					if (let == 'Y'){let = 'Z';}
					else{if (let == 'Z'){let = ' ';}
					}}}}}}}}}}}}}}}}}}}}}}}}}}
					

			lcd_putch(let);

				break;
			case BTN_ENC_RIGHT:
				if (let == ' '){let = 'Z';
				}
				else{if (let == 'Z'){let = 'Y';}
				else{
					if (let == 'Y'){let = 'X';
					}
					else{if (let == 'X'){let = 'W';
					}
					else{if (let == 'W'){let = 'V';
						}else{
						if (let == 'V'){let = 'U';
						}
						else{
							if (let == 'U'){let = 'T';
							}
							else{if (let == 'T'){let = 'S';
								}else{
								if (let == 'S'){let = 'R';
								}
								else{
									if (let == 'R'){let = 'Q';
									}
									else{if (let == 'Q'){let = 'P';
										}else{
										if (let == 'P'){let = 'O';
										}
										else{if (let == 'O'){let = 'N';
											}else{
											if (let == 'N'){let = 'M';
												}else{
												if (let == 'M'){let = 'L';
													
													}else{if (let == 'L'){let = 'K';
													}else{
													if (let == 'K'){let = 'J';
													}
													else{
														if (let == 'J'){let = 'I';
														}
														else{if (let == 'I'){let = 'H';
															}else{
															if (let == 'H'){let = 'G';
															}
															else{
																if (let == 'G'){let = 'F';
																}
																else{if (let == 'F'){let = 'E';
																	}else{
																	if (let == 'E'){let = 'D';
																	}
																	else{
																		if (let == 'D'){let = 'C';
																		}
																		else{if (let == 'C'){let = 'B';
																			}else{
																			if (let == 'B'){let = 'A';}
																			else{if (let == 'A'){let = ' ';}
																		}}}}}}}}}}}}}}}}}}}}}}}}}}
																		lcd_putch(let);

				break;
			default:
				lcd_putch(btn_press);
				x++;
				break;
		}
		btn_press = BTN_NONE;

	
	}

#if 0
	draw_frame();

	lcd_reverse_text(1);
	center_text(PSTR("LAUNCH"), 0);
#endif

	return 0;
}

