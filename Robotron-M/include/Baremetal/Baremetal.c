#include "./Controller/Controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include <math.h>


//Defines -- ADC logic
#define BUTTONS 3
#define RESOLUTION 1023


void initFreerunningADC() {
	ADMUX |= (1 << REFS0); /* reference voltage on AVCC */
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); /* ADC clock prescaler /8 */
	ADCSRA |= (1 << ADEN); /* enable ADC */
	ADCSRA |= (1 << ADATE); /* auto-trigger enable */
	ADCSRA |= (1 << ADIE);
	ADCSRA |= (1 << ADIF);
	ADCSRB = 0;
}

// Deactivate interrupt when modes are active
ISR(ADC_vect) {
	float avg = (float)RESOLUTION / (float)BUTTONS;  // Cast RESOLUTION and BUTTONS to float before the division
	uint16_t val = ADC;


	if (val > (BUTTONS - 0.5) * avg) {
		return;
	}
	
	for (int i = 0; i < BUTTONS; i++) {
		if (val < round((i + 0.5) * avg) && !ModeActive) {
			mode = Manual;
			ModeActive = true;
			return;
		}
		else if (ModeActive && ButtonPressed){
			ModeActive = false;
			ButtonPressed = false; 
		}
	}

	ADCSRA |= (1 << ADIF);
	return;
}