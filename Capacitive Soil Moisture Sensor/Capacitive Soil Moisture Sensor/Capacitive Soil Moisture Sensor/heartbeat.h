/*
 * heartbeat.h
 *
 * This is the header file for the heartbeat code file
 *
 * Created: 31/03/2024 8:39:42 PM
 *  Author: Greg
 */ 


#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

	// Initialize Timer 1 Program
	void init_timer1(void);
	
	ISR(TIMER1_COMPA_vect);


#endif /* HEARTBEAT_H_ */