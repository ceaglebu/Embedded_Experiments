# compiler options
AVRPATH=C:\avr-gnu\avr8-gnu-toolchain-win32_x86_64\bin
CC=$(AVRPATH)\avr-gcc.exe
OBJCOPY=$(AVRPATH)\avr-objcopy.exe
AVRDUDE=C:\avr-gnu\avrdude\avrdude.exe
CFLAGS= -Os -DF_CPU=16000000UL -mmcu=atmega328p
# -Os optimizes for size, not speed. Default for microcontrollers
# -DF_CPU=16000000UL sets clock to 16MHz
# -mmcu=atmega328p chooses microprocessor out of available avr-gccs

LIBRARIES:= $(wildcard lib/*/)
LIB_SRC:= $(filter-out $(wildcard lib/*/test*.c), $(foreach lib, $(LIBRARIES), $(filter-out ($wildcard $(lib)test*.c), $(wildcard $(lib)*.c))))

%.out: %.c
	$(CC) $(CFLAGS) $(LIB_SRC) $< -o $@

%.hex: %.out
	$(OBJCOPY) -O ihex -R .eeprom $< $@

install.%: %.hex
	$(AVRDUDE) -F -V -c arduino -p m328p -P COM3 -b 115200 -U flash:w:$<

clean:
	rm -fo *.hex *.out