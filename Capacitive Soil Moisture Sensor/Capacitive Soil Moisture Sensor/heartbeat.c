/*
 * heartbeat.c
 *
 * This files includes all the programs required to perform the heartbeat blink for the on board LED.
 *
 * Created: 31/03/2024 8:37:28 PM
 *  Author: Greg Schreiber
 */ 

// Include the Program header file
#include "project.h"

// Global for LED state, 0=off/1=on
volatile int LED_State;

// Function Prototypes
void init_timer1(void);
ISR(TIMER1_COMPA_vect);



/*
 * This program is to set up timer 1 as a CTC
 */
void init_timer1(void){
	
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