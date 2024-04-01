/*
 * Capacitive Soil Moisture Sensor.c
 *
 * A program to read the Capacitive Soil Moisture Sensor and read it to serial IO
 *
 * Created: 31/03/2024 8:33:29 PM
 * Author : Greg Schreiber
 */ 

// Project Includes
#include "project.h"
#include "heartbeat.h"
#include "serialIO.h"
#include "adc.h"


int main(void) {
	// Create Variable for ADC value & Moisture Percentage
	uint16_t ADC_value;
	int moisture_percentage;
	
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
		
		// Print the value to the serial window (DEBUG)
		//printf("The raw value is %u \n", ADC_value);
		
		// Scale the ADC value to a percentage & output to the serial window
		moisture_percentage = para_scaling(ADC_value);
		printf("The percentage value is %d %% \n", moisture_percentage);
		
		// Delay 2.5 seconds and repeat
		_delay_ms(2500);	
    }
}

