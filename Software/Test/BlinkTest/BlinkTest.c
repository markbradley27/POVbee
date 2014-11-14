// this is the header file that tells the compiler what pins and ports, etc.
// are available on this chip.
#include <avr/io.h>

// define what pins the LEDs are connected to.
// in reality, PD6 is really just '6'
#define F_CPU 16000000UL
#define LED1 PC3
#define LED2 PC4
#define LED3 PC5
#define LED4 PD7

// Some macros that make the code more readable
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

// this is just a program that 'kills time' in a calibrated method
void delay_ms(uint8_t ms) {
  uint16_t delay_count = F_CPU / 17500;
  volatile uint16_t i;

  while (ms != 0) {
    for (i=0; i != delay_count; i++);
    ms--;
  }
}

int main(void) {
  // initialize the direction of PORTD #6 to be an output
  set_output(DDRC, LED1);  
  set_output(DDRC, LED2);
  set_output(DDRC, LED3);
  set_output(DDRD, LED4);

  while (1) {
    output_high(PORTC, LED1);
    delay_ms(200);
    output_high(PORTC, LED2);
    delay_ms(200);
    output_high(PORTC, LED3);
    delay_ms(200);
    output_high(PORTD, LED4);
    delay_ms(200);

    output_low(PORTC, LED1);
    delay_ms(200);
    output_low(PORTC, LED2);
    delay_ms(200);
    output_low(PORTC, LED3);
    delay_ms(200);
    output_low(PORTD, LED4);
    delay_ms(200);
  }
}
