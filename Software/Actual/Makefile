CC=avr-gcc
MEGA=328p
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega$(MEGA)
OBJ2HEX=/usr/bin/avr-objcopy 
PROG=/usr/bin/avrdude
TARGET=POVbee

program : $(TARGET).hex
	$(PROG) -c avrispv2 -p m$(MEGA) -P /dev/ttyUSB0 -e
	$(PROG) -c avrispv2 -p m$(MEGA) -P /dev/ttyUSB0 -U flash:w:$(TARGET).hex

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o
