/*
 * CFile1.c
 *
 * Created: 12/1/2023 2:24:51 PM
 *  Author: brand
 */ 
#define _BV(bit) (1 << (bit)) //Set bit
//Function prototypes
void setupTimer();

void setupTimer() {
	
	// Sets up Timer/Counter0 for FAST PWM mode with a prescaler of n/1024
	TCCR0A |= (_BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1));
	TCCR0B |= (_BV(CS02)  | _BV(CS00));
/*	if (debugMode == 1) {
	}
	else {}*/
}