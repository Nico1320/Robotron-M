#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include <math.h>
uint16_t val = 0;


void initFreerunningADC() {
    ADMUX |= (1 << REFS0); /* reference voltage on AVCC */
    //ADMUX |= (1 << MUX0); /* select ADC input channel PC0 */
     ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN); /* enable ADC */
    //ADCSRA |= (1 << ADATE); /* auto-trigger enable */
    ADCSRA |= (1 << ADIE);
    ADCSRA |= (1 << ADIF);
    ADCSRB = 0;
}


// Deactivate interrupt when modes are active
ISR(ADC_vect) {
	val = ADC;
}

/*
else if (ModeActive && ButtonPressed){
ModeActive = false;
ButtonPressed = false;
}
}*/