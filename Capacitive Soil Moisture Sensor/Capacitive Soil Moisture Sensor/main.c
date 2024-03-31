/*
 * Capacitive Soil Moisture Sensor.c
 *
 * A program to read the Capacitive Soil Moisture Sensor and read it to serial IO
 *
 * Created: 31/03/2024 8:33:29 PM
 * Author : Greg Schreiber
 */ 

#include "project.h"
#include "heartbeat.h"
#include "serialIO.h"
#include "adc.h"

#include <util/delay.h>

static uint16_t ADC_Read_Value(void);
static uint16_t para_scaling(uint16_t raw_value);

int main(void) {
	// Create Variable for ADC value
	uint16_t ADC_value;
	uint16_t moisture_percentage;
	
	// Enable Timer 1 for heartbeat LEd
	init_timer1();
	
	// Set UART
	init_uart();
	
	// Initialize the ADC
	init_adc();
	
	// Turn on global interrupts
	sei();
	
    // Application Code
    while (1) {
		
		// Read the ADC value
		ADC_value = ADC_Read_Value();
		
		// Print the value to the serial window
		printf("The raw value is %3d\n", ADC_value);
		moisture_percentage = para_scaling(ADC_value);
		printf("The percentage value is %u %% \n", moisture_percentage);
		_delay_ms(500);	
    }
}

/*
 * A function for reading the ADC value
 */
static uint16_t ADC_Read_Value(void){
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
static uint16_t para_scaling(uint16_t raw_value){
	uint16_t engineering_value;
	
	engineering_value = ((1/(raw_value^4)) * 136200000000);
	
	return engineering_value;
}

