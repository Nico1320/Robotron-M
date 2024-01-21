#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

//Function prototype
void ultrasonicInit();
uint8_t ultrasonicRead();
static volatile int pulse_time1 = 0;
static volatile int pulse_time2 = 0;
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
	if ((PIND & (1<<PIND2)) == 0)
	{
		TCCR1B = 0;		//disabling counter
		pulse_time1 = TCNT1;
		TCNT1 = 0;		//resetting the counter memory
		// echo changed change from LOW to HIGH
		TCCR1B |= (1<<CS10);//enabling counter
	}
}

/*
ISR(INT1_vect)
{
	if ((PIND & (1<<PIND3)) == 0)
	{
		TCCR1B = 0;		//disabling counter
		pulse_time2 = TCNT1;
		TCNT1 = 0;		//resetting the counter memory
		} else {
		// echo changed change from LOW to HIGH
		TCCR1B |= (1<<CS10);//enabling counter
	}
}*/




uint8_t ultrasonicRead() {  // Change return type to char*
	PORTC |= (1 << PINC3);
	_delay_us(15);             
     // trigger the ultrasonic module for 10usec
	PORTC &= ~(1 << PINC3);	
	distance1 = (pulse_time1 * 0.00343 / 2); // calculate the distance
	if (distance1 < 50) 
	{
		distance1 = distance1;
	}
	else {
		distance1 = 20;
	}
	return distance1;
}