/*
 * adc.c
 *
 * A file for separate Analogue to Digital Conversion Code
 *
 * Created: 1/04/2024 12:01:23 AM
 *  Author: Greg Schreiber
 */ 

// Include the project header
#include "project.h"

// Function Prototypes
void init_adc(void);


/*
 * A function for initializing the ADC
 */
void init_adc(void){
	
	// Were going to use the external AVCC reference
	/*
	 * Bit 6 : 1 - AVCC with external capacitor at AREF pin
	 */ 
	ADMUX = (1<<REFS0);
	
	// Set Up the ADC
	/*
	 * Bit 7 : 1 - ADC Enable
	 * Bit 2 : 1 - Prescaler
	 * Bit 1 : 1 - Prescaler
	 * Bit 0 : 1 - Prescaler
	 * (The prescaler is set to a div factor of 128
	 *
	 */
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
}

