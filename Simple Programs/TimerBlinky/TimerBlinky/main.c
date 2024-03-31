/*
 * TimerBlinky.c
 *
 * Created: 31/03/2024 7:27:53 PM
 * Author : Greg Schreiber
 */ 

// 328P Include
#include <avr/io.h>
// Set for the on board 16 MHz Crystal Oscillator
#define F_CPU 16000000UL

// Interrupt Include File
#include <avr/interrupt.h>

// Global for LED state, 0=off/1=on
volatile int LED_State;

/* 
 * This function is the main run function
 *
 * Return: It always returns an int output
 * Arguments: Void (nil)
 */
int main(void) {
	
	// On board LED is connected to Pin B5
	DDRB = (1<<5);
	// Set initial LED state to off
	LED_State = 0;
	
	/* 
	 * Set Up Timer/Counter 1 to produce the interrupt
	 */
	// Set compare value
	OCR1A = 62500;
	/* Set Timer Compare Mode (CTC Mode)
	 *
	 * TCCR1A No bits required
	 * TCCR1B WGM12 for CTC mode, CS12 for Clk/256 (Prescaler)
	 */
	TCCR1A = 0;
	TCCR1B = (1<<WGM12)|(1<<CS12);
	
	// Enable Compare Interrupt (On output compare match)
	TIMSK1 = (1<<OCIE1A);
	
	// Ensure interrupt flag is cleared
	TIFR1 = (1<<OCF1A);

	// Turn on global interrupts
	sei();
	
    // Application Code
    while (1) {
		
    }
}

/*
 * Interrupt for Timer 1 Compare
 *
 * This Interrupt Service Routine toggles the state of the LED pin
 */ 
ISR(TIMER1_COMPA_vect) {

	// Use a switch statement for the LED state (This keeps the ISR as short as possible).
	switch (LED_State) {
		// If LED off turn on
		case 0:
			PORTB = (1<<5);
			LED_State = 1;
			break;
		// If LED on turn off
		case 1:
			PORTB = (0<<5);
			LED_State = 0;
			break;
	}
}