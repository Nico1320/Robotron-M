/*
 * Robotron-M.c
 *
 * Created: 11/27/2023 7:03:58 PM
 * Author : brand
 */ 

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include "Baremetal/Baremetal.h"
#define _BV(bit) (1 << (bit)) //Set bit

/*
// Serial stuff
//#define F_CPU 16000000UL
#define FOSC F_CPU
#define BAUDRATE 9600
#define myUBRR (uint16_t)((F_CPU / 16UL / BAUDRATE) - 1)
*/

// Function prototypes
void initializeHbridge();
void setupTimer();
void setPWM(uint8_t, uint8_t);
void zeroRadii();
void goForward();
void goBackward();
void clearPrevious();

// Prints configuration of pins and timers related to this library
uint8_t debugMode = 0;

void initializeHbridge() {
    // Motor control initialization
    DDRB |= (_BV(PINB0) | _BV(PINB1) | _BV(PINB2));
	//		PB0- IN2	  PB1 - IN3    PB2 - IN4     
    DDRD |= (_BV(PIND5) | _BV(PIND6) | _BV(PIND7));
}	//		PD5 - ENB     PD6 - ENA	   PD7 - IN1


void setPWM(uint8_t dutyCycleA, uint8_t dutyCycleB) {
    // Set the OCR0A/B value for a 15.6 kHz frequency at 50% duty cycle
    OCR0A = dutyCycleA;  // PD6
    OCR0B = dutyCycleB;  // PD5
}

void zeroRadii(int CW) {
	if (!CW) {
		clearPrevious();
		PORTD |= _BV(PIND7); // IN1
		PORTB |= _BV(PINB2);  // IN4
	} 
	else if (CW) {
		clearPrevious();
		PORTB |= (_BV(PINB0) | _BV(PINB1));  // IN2 and IN4
	}
}

void goForward() {
	clearPrevious();
    PORTD |= (_BV(PIND7)); 
    PORTB |= (_BV(PINB1)); 
}

void goBackward() {
	clearPrevious();
	PORTB |= (_BV(PINB0) | _BV(PINB2));
}

void clearPrevious() {
	PORTB &= ~(_BV(PINB0) | _BV(PINB1) | _BV(PINB2));
	PORTD &= ~(_BV(PIND7));
}
