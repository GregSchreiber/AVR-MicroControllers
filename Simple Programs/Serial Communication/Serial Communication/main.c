/*
 * Serial Communication.c
 *
 * Created: 31/03/2024 4:01:32 PM
 * Author : Greg Schreiber
 */ 

// AVR Include Header Files
#include <avr/io.h>

// Set for the on board 16 MHz Crystal Oscillator
#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <util/delay.h>
#include <ctype.h>


/*
 * Main Program
 *
 * Returns: Main always returns an integer input
 * Arguments: Nil
 */
int main(void) {
	
	// Set the baud rate to 19200
	UBRR0 = 51;
	
	// On board LED is connected to Pin B5
	DDRB = (1<<5);
	
	/*
	 * Enable the following bits
	 * Bit 3 - TXEN0 - Transmit Enable
	 * Bit 4 - RXEN0 - Receive Enable
	 * Bit 7 - RXCIE0 - Receive Complete Interrupt
	 */
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	// Enable Interrupts
	sei();
		
    // Application Code
    while (1) {
		
		// Heartbeat
		PORTB = (1<<5);
		_delay_ms(1000);
		PORTB = (0<<5);
		_delay_ms(1000);
    }
}

/*
 * Interrupt Handler for UART Receive Complete
 *
 * This interrupt writes it back in an upper case format
 *
 * Interrupt Vector Initialized above as a character has arrived in the UART Data Register
 */
ISR(USART_RX_vect) {
	
	// Input Character (No need to declare this as volatile as its not a global variable)
	char input;

	// Move the character from the input register
	input = UDR0;

	// Convert character to upper case if it is lower case
	input = toupper(input);

	// Send the character back over the serial connection
	UDR0 = input;
}

