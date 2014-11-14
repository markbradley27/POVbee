avr-gcc -g -Os -Wall -mcall-prologues -std=c99 -mmcu=atmega328p -c -o %1.o %1.c
avr-gcc -g -Os -Wall -mcall-prologues -std=c99 -mmcu=atmega328p %1.o -o %1.obj
avr-objcopy -R .eeprom -O ihex %1.obj %1.hex