#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "./Controller/Controller.h"
//Function prototype
void ultrasonicInit();
uint8_t ultrasonicRead();
volatile uint32_t startPulse1 = 0;
volatile  uint32_t endPulse1 = 0;
volatile uint32_t startPulse2 = 0;
volatile  uint32_t endPulse2 = 0;
bool ultracycle1 = false;
bool ultracycle2 = false;
volatile uint32_t pulse_time1 = 0;
volatile uint32_t pulse_time2 = 0;
uint8_t distance1;
uint8_t distance2;

// Adjust the size based on your requirements

void ultrasonicInit() {	
	DDRC |= (1<<PINC3);
	DDRD &= ~(1<<PIND2);
	EIMSK |= ((1<<INT0)); //| (1<<INT1));	// enable INT0 & INT1
	EICRA |= ((1<<ISC00)); //| (1<<ISC10));// setting interrupt trigger on any change
}

ISR(INT0_vect)
{
		if (((PIND & (1 << PIND2)) == 0) && ultracycle1) {
			endPulse1 = millis;
			pulse_time1 = (endPulse1 - startPulse1);
			} else if (!ultracycle1) {
			// echo changed change from LOW to HIGH
			startPulse1 = millis;
			ultracycle1 = true;
		}
}

ISR(INT1_vect)
{
	if (((PIND & (1 << PIND3)) == 0) && ultracycle2) {
		endPulse2 = millis;
		pulse_time2 = (endPulse2 - startPulse2);
		} else if (!ultracycle2) {
		// echo changed change from LOW to HIGH
		startPulse2 = millis;
		ultracycle2 = true;
	}
}


uint8_t ultrasonicRead() {  // Change return type to char*
	PORTC |= (1 << PINC3);
	_delay_us(15);             
     // trigger the ultrasonic module for 10usec
	PORTC &= ~(1 << PINC3);		
	while (pulse_time1 < 1 && pulse_time2){
		if (pulse_time1 > 1) {
			break;
		}
	}
	distance1 = (pulse_time1 * 0.0342 / 2); // calculate the distance
	distance2 = (pulse_time2 * 0.0342 / 2); // calculate the distance
	pulse_time1 = 0;
	startPulse1 = 0;
	endPulse1 = 0;
	ultracycle1 = false;
	pulse_time2 = 0;
	startPulse2 = 0;
	endPulse2 = 0;
	ultracycle2 = false;
	return distance1;
}