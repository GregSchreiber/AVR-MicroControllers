/*
 * adc.h
 *
 * The header file for the Analogue to Digital Conversion Code
 *
 * Created: 1/04/2024 12:01:56 AM
 *  Author: Greg Schreiber
 */ 


#ifndef ADC_H_
#define ADC_H_

	// Initialize the ADC
	void init_adc(void);
	
	// Read the value from the ADC and return a long
	uint16_t ADC_Read_Value(void);
	
	// Scale the long to a percentage
	int para_scaling(uint16_t raw_value);
	
	/* ADC Returns a Raw value of:
	 * 462 Dry
	 * 200 Wet
	 */

#endif /* ADC_H_ */