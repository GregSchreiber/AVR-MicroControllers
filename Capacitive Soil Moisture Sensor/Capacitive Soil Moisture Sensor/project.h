/*
 * project.h
 *
 * This header file will include all the additional header files and defines required for the project
 *
 * Created: 31/03/2024 11:10:17 PM
 *  Author: Greg Schreiber
 */ 


#ifndef PROJECT_H_
#define PROJECT_H_

	// ATMega 328P Header
	#include <avr/io.h>
	
	// Set for the on board 16 MHz Crystal Oscillator
	#define F_CPU 16000000UL
	
	// Include the interrupt header file
	#include <avr/interrupt.h>

	// Additional Includes
	#include <util/delay.h>
	#include <stdio.h>
	#include <stdint.h>

#endif /* PROJECT_H_ */