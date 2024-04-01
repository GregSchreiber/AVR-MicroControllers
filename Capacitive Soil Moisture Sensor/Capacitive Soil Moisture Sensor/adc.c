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
uint16_t ADC_Read_Value(void);
int para_scaling(uint16_t raw_value);


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

/*
 * A function for reading the ADC value
 */
uint16_t ADC_Read_Value(void){
	// Create Variable for raw ADC value
	uint16_t value;
	
	// Start the ADC conversion
	ADCSRA |= (1<<ADSC);
	
	// Wait until conversion finished
	while(ADCSRA & (1<<ADSC)) {
		;
	}
	
	// Set the raw value to the ADC output	
	value = ADC;
	
	// Return the raw value
	return value;
}

/*
 * A function for para scaling the raw ADC value to a percentage
 */
int para_scaling(uint16_t raw_value){
	// Variables
	uint32_t temp1;
	float temp2;
	int engineering_value;
	
	// The following process forms the para scaling required for the percentage conversion
	// Square the raw adc value
	temp1 = (uint32_t)raw_value * (uint32_t)raw_value;
	// 1 div squared adc value - para scaling
	temp2 = 1 / (float) temp1; 
	// The scaling factor here is based on the maximum resistance which equates to 0%.
	engineering_value = (temp2 *  3562520) - 16; 
	// This scaling factor here is based on the minimum resistance which equates to 100%.
	engineering_value = engineering_value * 1.369; 
	
	// Return the percentage value
	return engineering_value;
}







