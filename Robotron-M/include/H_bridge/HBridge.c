/*
 * Robotron-M.c
 *
 * Created: 11/27/2023 7:03:58 PM
 * Author : brand
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <avr/sfr_defs.h>
#include <math.h>
//#include "./Baremetal/Baremetal.h"
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
void zeroRadii(bool CW);
void goForward();
void goBackward();
void clearPrevious(bool fast);
void turningRatio(float ratio, uint8_t maxPWM);

void initializeHbridge() {
    // Motor control initialization
    DDRB |= (_BV(PINB0) | _BV(PINB1) | _BV(PINB2));
	//		PB0- IN2	  PB1 - IN3    PB2 - IN4     
    DDRD |= (_BV(PIND5) | _BV(PIND6) | _BV(PIND7));
}	//		PD5 - ENB     PD6 - ENA	   PD7 - IN1

void setupTimer() {
	
	// Sets up Timer/Counter0 for FAST PWM mode with a prescaler of n/1024
	TCCR0A |= (_BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1));
	TCCR0B |= (_BV(CS02)  | _BV(CS00));

}

void setPWM(uint8_t dutyCycleA, uint8_t dutyCycleB) {
    // Set the OCR0A/B value for a 15.6 kHz frequency at 50% duty cycle
    OCR0A = dutyCycleA;  // PD6
    OCR0B = dutyCycleB;  // PD5
}

void zeroRadii(bool CW) {
	if (!CW) {
		clearPrevious(false);
		PORTD |= _BV(PIND7); // IN1
		PORTB |= _BV(PINB2);  // IN4
	} 
	else if (CW) {
		clearPrevious(false);
		PORTB |= (_BV(PINB0) | _BV(PINB1));  // IN2 and IN4
	}
}

void goForward() {
	clearPrevious(false);
    PORTD |= (_BV(PIND7)); 
    PORTB |= (_BV(PINB1)); 
}

void goBackward() {
	clearPrevious(false);
	PORTB |= (_BV(PINB0) | _BV(PINB2));
}

void clearPrevious(bool fast) {
	
	if (fast) {
	//goBackward();
	}
	else {
	PORTB &= ~(_BV(PINB0) | _BV(PINB1) | _BV(PINB2));
	PORTD &= ~(_BV(PIND7));	
	}
	

}

void turningRatio(float ratio, uint8_t maxPWM) {
	// Ensure that ratio is within the range [0, 2]
	ratio = fmax(0.0f, fmin(2.0f, ratio));

	if (ratio < 1) {
		// Exponential behavior for OCR0A
		OCR0A = (uint8_t)(maxPWM * pow(ratio, 2));
		OCR0B = maxPWM;
		} else if (ratio > 1) {
		OCR0A = maxPWM;
		// Exponential behavior for OCR0B
		OCR0B = (uint8_t)(maxPWM / pow(ratio, 2));
		} else {
		OCR0A = maxPWM;
		OCR0B = maxPWM;
	}
}