default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o build/program.elf src/program.c src/gpio.h
	avr-objcopy -O ihex -R .eeprom build/program.elf build/program.hex

.PHONY: upload clean

upload:
	avrdude -c arduino -p ATMEGA328P -P /dev/ttyACM1 -b 115200 -U flash:w:build/program.hex

clean:
	rm -f build/*