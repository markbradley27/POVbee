#include <avr/io.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr) {
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)) );

	// Put data into buffer, sends the data
	UDR0 = data;
}

void USART_Transmit_String(unsigned char* string, unsigned int length) {
	int i;
	for(i = 0; i < length; ++i) {
		USART_Transmit(string[i]);
	}
}

int main(void) {
	// Initialize serial
	USART_Init(MYUBRR);
	while(1) {
		USART_Transmit('8');
	}
}
