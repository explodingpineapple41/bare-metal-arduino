default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o build/blink.o src/blink.c
	avr-gcc -o build/blink.bin build/blink.o
	avr-objcopy -O ihex -R .eeprom build/blink.bin build/blink.hex

.PHONY: upload clean

upload:
	avrdude -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:build/blink.hex

clean:
	rm -f build/*