/*
 * main.c
 *
 *  Created on: Jun 13, 2017
 *      Author: Mina G. Sadek
 */
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TRIG_PIN 3
#define ECHO_PIN 2
#define TRIG_PORT PORTC
#define TRIG_DDR  DDRC
#define ECHO_IN   PIND
#define ECHO_DDR  DDRD

//Macros -- to be updated
#define SET_BIT(PORT,BIT) PORT |= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT &= ~(1<<BIT)
#define TGL_BIT(PORT,BIT) PORT ^= (1<<BIT)
#define READ_BIT(PORT,BIT) ((PORT & (1<<BIT))>>BIT)
#define SET_PORT(PORT) PORT = 0xFF
#define CLR_PORT(PORT) PORT = 0x00
#define TGL_PORT(PORT) PORT = ~PORT
#define SET_NBLE(PORT,LEAST_HIGH) (LEAST_HIGH? (PORT |= 0xF0) : (PORT |= 0x0f))
#define CLR_NBLE(PORT,LEAST_HIGH) (LEAST_HIGH? (PORT &= ~0xF0) : (PORT &= ~0x0f))

//Function prototype
void ultrasonicInit();
float ultrasonicRead();
void floatToString(float value, char* buffer, int precision);
static volatile int pulse_time = 0;
  // Adjust the size based on your requirements

ultrasonicInit(void) {
	TRIG_DDR |= (1<<TRIG_PIN);
	ECHO_DDR &= (1<<ECHO_PIN);

	EIMSK |= (1<<INT0);	// enable INT0
	EICRA |= (1<<ISC00);// setting interrupt trigger on any change
}

ISR(INT0_vect)
{
	if ((ECHO_IN & (1<<ECHO_PIN)) == 0)
	{
		TCCR1B = 0;		//disabling counter
		pulse_time = TCNT1;
		TCNT1 = 0;		//resetting the counter memory
	} else {
		// echo changed change from LOW to HIGH
		TCCR1B |= (1<<CS10);//enabling counter
	}
}



float ultrasonicRead() {  // Change return type to char*
	float distance;
	TRIG_PORT |= (1 << TRIG_PIN);
	_delay_us(15);                     // trigger the ultrasonic module for 15usec
	TRIG_PORT &= ~(1 << TRIG_PIN);
	distance = pulse_time * 0.00343 / 2; // calculate the distance
	return distance;
}