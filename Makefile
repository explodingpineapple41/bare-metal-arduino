CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

PROGRAMMER = arduino
PORT = /dev/ttyACM0
BAUD = 115200

MCU = atmega328p
F_CPU = 16000000UL

COMMON_FLAGS = -mmcu=$(MCU) \
               -DF_CPU=$(F_CPU) \
               -Os \
               -Wall \
               -ffunction-sections \
               -fdata-sections \
               -Isrc

CFLAGS = $(COMMON_FLAGS)

CXXFLAGS = $(COMMON_FLAGS) \
           -std=c++17 \
           -fno-exceptions \
           -fno-threadsafe-statics

LDFLAGS = -Wl,--gc-sections

BUILDDIR = build
SRCDIR = src

C = $(wildcard $(SRCDIR)/*.c)
CPP = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(C)) $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(CPP))

TARGET = $(BUILDDIR)/program

.PHONY: upload clean

all: $(TARGET).hex

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -D -U flash:w:$<

clean:
	rm -f build/*
