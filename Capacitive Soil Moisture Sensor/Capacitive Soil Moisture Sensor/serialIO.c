/*
 * serialIO.c
 *
 * This is the setup for the serial IO UART communication
 * This file is not written by me, it allows for simple printf functionality.
 *
 * Created: 31/03/2024 11:07:53 PM
 *  Author: Greg Schreiber
 */ 

// Include the Program header file
#include "project.h"

// Global Output Variables
#define OUTPUT_BUFFER_SIZE 128
volatile char out_buffer[OUTPUT_BUFFER_SIZE];
volatile uint8_t out_insert_pos;
volatile uint8_t bytes_in_out_buffer;

// Global Input Variables
#define INPUT_BUFFER_SIZE 16
volatile char input_buffer[INPUT_BUFFER_SIZE];
volatile uint8_t input_insert_pos;
volatile uint8_t bytes_in_input_buffer;
volatile uint8_t input_overrun;

// Function Prototypes
void init_uart(void);
static int uart_put_char(char, FILE*);
static int uart_get_char(FILE*);

static FILE myStream = FDEV_SETUP_STREAM(uart_put_char, uart_get_char,
_FDEV_SETUP_RW);

/*
 * A function for initializing UART communication
 */
void init_uart(void){
	
	// Set the baud rate to 19200
	UBRR0 = 51;
	
	// Initialize our buffers
	out_insert_pos = 0;
	bytes_in_out_buffer = 0;
	input_insert_pos = 0;
	bytes_in_input_buffer = 0;
	input_overrun = 0;
	
	/*
	 * Enable the following bits
	 * Bit 3 - TXEN0 - Transmit Enable
	 * Bit 4 - RXEN0 - Receive Enable
	 * Bit 7 - RXCIE0 - Receive Complete Interrupt
	 */
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	stdout = &myStream;
	stdin = &myStream;
}

/*
 * A function for putting characters in the buffer
 */
static int uart_put_char(char c, FILE* stream) {
	uint8_t interrupts_enabled;
	
	/* Add the character to the buffer for transmission (if there 
	 * is space to do so). If not we wait until the buffer has space.
	 * If the character is \n, we output \r (carriage return)
	 * also.
	*/
	if(c == '\n') {
		uart_put_char('\r', stream);
	}
	
	/* If the buffer is full and interrupts are disabled then we
	 * abort - we don't output the character since the buffer will
	 * never be emptied if interrupts are disabled. If the buffer is full
	 * and interrupts are enabled then we loop until the buffer has 
	 * enough space. The bytes_in_buffer variable will get modified by the
	 * ISR which extracts bytes from the buffer.
	*/
	interrupts_enabled = bit_is_set(SREG, SREG_I);
	while(bytes_in_out_buffer >= OUTPUT_BUFFER_SIZE) {
		if(!interrupts_enabled) {
			return 1;
		}		
		/* else do nothing */
	}
	
	/* Add the character to the buffer for transmission if there
	 * is space to do so. We advance the insert_pos to the next
	 * character position. If this is beyond the end of the buffer
	 * we wrap around back to the beginning of the buffer 
	 * NOTE: we disable interrupts before modifying the buffer. This
	 * prevents the ISR from modifying the buffer at the same time.
	 * We reenable them if they were enabled when we entered the
	 * function.
	*/	
	cli();
	out_buffer[out_insert_pos++] = c;
	bytes_in_out_buffer++;
	if(out_insert_pos == OUTPUT_BUFFER_SIZE) {
		/* Wrap around buffer pointer if necessary */
		out_insert_pos = 0;
	}
	/* Reenable interrupts (UDR Empty interrupt may have been
	 * disabled) */
	UCSR0B |= (1 << UDRIE0);
	if(interrupts_enabled) {
		sei();
	}
	return 0;
}

/*
 * A function for getting characters from the buffer
 */
int uart_get_char(FILE* stream) {
	/* Wait until we've received a character */
	while(bytes_in_input_buffer == 0) {
		/* do nothing */
	}
	
	/*
	 * Turn interrupts off and remove a character from the input
	 * buffer. We reenable interrupts if they were on.
	 * The pending character is the one which is byte_in_input_buffer
	 * characters before the insert position (taking into account
	 * that we may need to wrap around).
	 */
	uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
	cli();
	char c;
	if(input_insert_pos - bytes_in_input_buffer < 0) {
		/* Need to wrap around */
		c = input_buffer[input_insert_pos - bytes_in_input_buffer
				+ INPUT_BUFFER_SIZE];
	} else {
		c = input_buffer[input_insert_pos - bytes_in_input_buffer];
	}
	
	/* Decrement our count of bytes in the input buffer */
	bytes_in_input_buffer--;
	if(interrupts_enabled) {
		sei();
	}	
	return c;
}

/*
 * Define the interrupt handler for UART Data Register Empty (i.e. 
 * another character can be taken from our buffer and written out)
 */
ISR(USART_UDRE_vect) 
{
	/* Check if we have data in our buffer */
	if(bytes_in_out_buffer > 0) {
		/* Yes we do - remove the pending byte and output it
		 * via the UART. The pending byte (character) is the
		 * one which is "bytes_in_buffer" characters before the 
		 * insert_pos (taking into account that we may 
		 * need to wrap around to the end of the buffer).
		 */
		char c;
		if(out_insert_pos - bytes_in_out_buffer < 0) {
			/* Need to wrap around */
			c = out_buffer[out_insert_pos - bytes_in_out_buffer
				+ OUTPUT_BUFFER_SIZE];
		} else {
			c = out_buffer[out_insert_pos - bytes_in_out_buffer];
		}
		/* Decrement our count of the number of bytes in the 
		 * buffer 
		 */
		bytes_in_out_buffer--;
		
		/* Output the character via the UART */
		UDR0 = c;
	} else {
		/* No data in the buffer. We disable the UART Data
		 * Register Empty interrupt because otherwise it 
		 * will trigger again immediately this ISR exits. 
		 * The interrupt is reenabled when a character is
		 * placed in the buffer.
		 */
		UCSR0B &= ~(1<<UDRIE0);
	}
}

/*
 * Define the interrupt handler for UART Receive Complete (i.e. 
 * we can read a character. The character is read and placed in
 * the input buffer.
 */
ISR(USART_RX_vect) 
{	
	/* Read the character */
	char c;
	c = UDR0;
		
	if(bytes_in_out_buffer < OUTPUT_BUFFER_SIZE) {
		/* If echoing is enabled and there is output buffer
		 * space, echo the received character back to the UART.
		 * (If there is no output buffer space, characters
		 * will be lost.)
		 */
		uart_put_char(c, 0);
	}
	
	/* 
	 * Check if we have space in our buffer. If not, set the overrun
	 * flag and throw away the character. (We never clear the 
	 * overrun flag - it's up to the programmer to check/clear
	 * this flag if desired.)
	 */
	if(bytes_in_input_buffer >= INPUT_BUFFER_SIZE) {
		input_overrun = 1;
	} else {
		/* If the character is a carriage return, turn it into a
		 * linefeed 
		*/
		if (c == '\r') {
			c = '\n';
		}
		
		/* 
		 * There is room in the input buffer 
		 */
		input_buffer[input_insert_pos++] = c;
		bytes_in_input_buffer++;
		if(input_insert_pos == INPUT_BUFFER_SIZE) {
			/* Wrap around buffer pointer if necessary */
			input_insert_pos = 0;
		}
	}
}