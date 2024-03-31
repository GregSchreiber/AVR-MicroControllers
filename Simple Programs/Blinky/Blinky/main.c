/*
 * Blinky.c
 *
 * Created: 29/03/2024 5:00:21 PM
 * Author : Greg Schreiber
 */ 

// Standard AVR Include Files
#include <avr/io.h>

// Set for the on board 16 MHz Crystal Oscillator
#define F_CPU 16000000UL

// Delay Include File
#include <util/delay.h>

/* 
 * This function is the main run function
 *
 * Return: It always returns an int output
 * Arguments: Void (nil)
 */
int main(void){
	
	// On board LED is connected to Pin B5
	DDRB = (1<<5);
	
    /* Application Code */
    while (1){
		
		// Write Pin B5 High and Wait 1.0 seconds
		PORTB = (1<<5);
		_delay_ms(1000);
		
		// Write Pin B5 Low and Wait 1.0 seconds
		PORTB = (0<<5);
		_delay_ms(1000);
    }
}

