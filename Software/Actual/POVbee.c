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
 
/********************************************************************************
Macros and Defines
********************************************************************************/
#define F_CPU 16000000
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
 
/********************************************************************************
Function Prototypes
********************************************************************************/
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

/********************************************************************************
Main
********************************************************************************/
int main(void) {
    // Initialize USART
    usart_init(MYUBRR);
    stdout = &mystdout;

    // Turn on green light
    DDRC |= (1 << PC4);
    PORTC &= ~(1 << PC4);

    // Main loop
    while (true) {
        printf("Hi %s!\r\n", "world from printf");
    }
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