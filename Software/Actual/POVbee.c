/********************************************************************************
Includes
********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "WS2811.h"
 
/********************************************************************************
Macros and Defines
********************************************************************************/
#define F_CPU 16000000UL
#define BAUD 9600UL
#define MYUBRR F_CPU/16/BAUD-1
DEFINE_WS2811_FN(WS2811RGB, PORTC, 2)
 
/********************************************************************************
Function Prototypes
********************************************************************************/
int16_t get_rotation_gyro(void);
void accel_init(void);
int16_t accel_read_axis(uint8_t);
int16_t accel_read_smoothed_axis(uint8_t, int16_t);
void port_init(void);
void adc_init(void);
void spim_init(void);
void spi_tx(char data);
uint16_t analog_read(uint8_t channel);
void usart_init( uint16_t ubrr);
void usart_putchar(char data);
char usart_getchar(void);
unsigned char usart_kbhit(void);
void usart_pstr(char *s);
int usart_putchar_printf(char var, FILE *stream);

/********************************************************************************
Global Variables
********************************************************************************/
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
RGB_t rgb[4] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

/********************************************************************************
Main
********************************************************************************/
int main(void) {
    // AVR init
    port_init();
    adc_init();
    spim_init();
    usart_init(MYUBRR);
    stdout = &mystdout;

    // Device init
    accel_init();

    // Variable declaration
    int16_t rotation = 0;
    int16_t accel_x = 0;
    int16_t accel_y = 0;
    int16_t accel_z = 0;

    // Main loop
    while (true) {
        rotation = get_rotation_gyro();
        accel_x = accel_read_smoothed_axis('x', accel_x);
        accel_y = accel_read_axis('y');
        accel_z = accel_read_axis('z');

        printf("Rotation: %3d\t", rotation);
        printf("Accel: %3d,\t%3d,\t%3d\r\n", accel_x, accel_y, accel_z);

        uint8_t i;
        if (rotation < -15) {
            for (i = 0; i < 4; ++i) {
                rgb[i].g = 255;
                rgb[i].b = 0;
                rgb[i].r = 0;
            }
        }
        else if (rotation > 15) {
            for (i = 0; i < 4; ++i) {
                rgb[i].g = 0;
                rgb[i].b = 255;
                rgb[i].r = 0;
            }
        }
        else {
            for (i = 0; i < 4; ++i) {
                rgb[i].g = 0;
                rgb[i].b = 0;
                rgb[i].r = 255;
            }
        }

        WS2811RGB(rgb, ARRAYLEN(rgb));
    }
}
/********************************************************************************
Device helpers
********************************************************************************/
// Gyro
int16_t get_rotation_gyro(void) {
    return analog_read(7) - analog_read(6);
}

// Accel
void accel_init(void) {
    // Set to standby mode
    PORTB &= ~(1<<PB2);
    spi_tx(0x2D);
    spi_tx(0x00);
    PORTB |= (1<<PB2);

    // Set to measure mode
    PORTB &= ~(1<<PB2);
    spi_tx(0x2D);
    spi_tx(0x08);
    PORTB |= (1<<PB2);

    // Set to +/- 16g, full-resolution
    PORTB &= ~(1<<PB2);
    spi_tx(0x31);
    spi_tx(0x0B);
    PORTB |= (1<<PB2);
}
int16_t accel_read_axis(uint8_t axis) {
    int16_t axis_accel = 0x00;
    switch(axis) {
        case 'x': axis = 0x32; break;
        case 'y': axis = 0x34; break;
        case 'z': axis = 0x36; break;
        default: return 0;
    }

    // Pull CS low
    PORTB &= ~(1<<PB2);

    // Read x data registers
    spi_tx(0x80 | 0x40 | axis);
    spi_tx(0x00);
    axis_accel |= SPDR;
    spi_tx(0x00);
    axis_accel |= SPDR << 8;

    // Drive CS high
    PORTB |= (1<<PB2);

    return axis_accel;
}

/********************************************************************************
Device initialization
********************************************************************************/
void port_init(void) {
    // Turn on green light
    PORTC &= ~(1 << PC4);
    DDRC |= (1 << PC4);

    // Gyro pins
    PORTC &= ~(1 << PC0);
    PORTC &= ~(1 << PC1);
    DDRC |= 0x03;

    // Pixel pin
    DDRC |= (1<<PC2);
    PORTC &= ~(1<<PC2);
}

void adc_init(void) {
    ADMUX &= ~0xE0;         // AREF reference voltage, right adjusted result
    // TODO: Try speeding this back up
    ADCSRA = 0x87;          // ADC enabled, no auto-trigger, aclock = sysclock/128
}

void spim_init(void) {
    DDRB = (1<<PB2)|(1<<PB3)|(1<<PB5);     // Set CS, MOSI, and SCK output, all others input
    PORTB |= (1<<PB2);                     // Set CS high initially
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);    // Enable SPI, Master, set clock rate fck/128
}

/********************************************************************************
ADC Related
********************************************************************************/
uint16_t analog_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | channel;
    ADCSRA |= 0x40;
    while(!(ADCSRA & 0x10));
    ADCSRA |= 0x10;
    return ADC;
}

/********************************************************************************
SPI Related
********************************************************************************/
void spi_tx(char data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

/********************************************************************************
usart Related
********************************************************************************/
void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8data, 1stop bit
    //UCSR0C = (1<<URSEL)|(3<<UCSZ0);
    UCSR0C = (3<<UCSZ00);
}
void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while ( !(UCSR0A & (_BV(UDRE0))) );
    // Start transmission
    UDR0 = data; 
}
char usart_getchar(void) {
    // Wait for incoming data
    while ( !(UCSR0A & (_BV(RXC0))) );
    // Return the data
    return UDR0;
}
unsigned char usart_kbhit(void) {
    //return nonzero if char waiting polled version
    unsigned char b;
    b=0;
    if(UCSR0A & (1<<RXC0)) b=1;
    return b;
}
void usart_pstr(char *s) {
    // loop through entire string
    while (*s) { 
        usart_putchar(*s);
        s++;
    }
}
int usart_putchar_printf(char var, FILE *stream) {
    usart_putchar(var);
    return 0;
}
