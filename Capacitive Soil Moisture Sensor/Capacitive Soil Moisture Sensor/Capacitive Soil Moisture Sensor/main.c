/*
 * Capacitive Soil Moisture Sensor.c
 *
 * Created: 31/03/2024 8:33:29 PM
 * Author : Greg
 */ 

#include "program.h"
#include "heartbeat.h"


int main(void) {
	
	// Enable Timer 1 for heartbeat LEd
	init_timer1();
	
	// Turn on global interrupts
	sei();
	
    // Application Code
    while (1) {
		
    }
}

