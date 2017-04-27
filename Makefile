TARGET=sirius
OBJS=main.o lcd.o font.o button.o
DEVICE=atmega128
PROGTOOL=avrdude
ISPFLAGS=-p $(DEVICE) -c usbasp

AS=avr-as
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -Wall -mmcu=$(DEVICE) -DF_CPU=7.3728e6
LDFLAGS=-mmcu=$(DEVICE)


all:		$(TARGET).hex

clean:
		rm -f $(TARGET).hex $(TARGET).elf $(OBJS) makefont font.c

prog:		$(TARGET).hex
		$(PROGTOOL) $(ISPFLAGS) -U flash:w:sirius.hex

prog_config:
		$(PROGTOOL) $(ISPFLAGS) -U hfuse:w:0xc9:m -U lfuse:w:0xef:m

read_config:
		$(PROGTOOL) $(ISPFLAGS) -n

# This looks convoluted, making the elf and then the hex.
# But if ld generates a hex, it will include bss, which will overwrite text
# (because hex files don't have sections), causing weird bugs.  objcopy
# creates a hex file suitable for programming.  This could probably be considered
# a bug in ld.

$(TARGET).elf:	$(OBJS)
		$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET).elf

$(TARGET).hex:	$(TARGET).elf
		$(OBJCOPY) -O ihex $< $@

makefont:	makefont.c
		gcc $< -O2 -Wall -g3 -o $@ -lpng

font.c:		8x8font.png makefont
		./makefont $< $@

