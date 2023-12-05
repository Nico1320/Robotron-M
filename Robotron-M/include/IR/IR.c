/*
 * IR.c
 *
 * Created: 12/2/2023 2:13:00 PM
 * Author : Marcell Danajka
 */ 

#include <avr/io.h>
#include "IR.h"

#define F_CPU (16000000UL)
#define LINE_DETECTED 1
#define LINE_NOT_DETECTED 0
#define LEFT_IR_SENSOR 0
#define MIDDLE_IR_SENSOR 1
#define RIGHT_IR_SENSOR 2


/*
int irReadArray [3];
int leftIrSensorRead(void);
int middleIrSensorRead(void);
int rightIrSensorRead(void);
*/

/*
int main(void)
{
	initIrPins();
	
// Only for testing
    while (1) 
    {
		irReadArray [0] = irSensorRead(LEFT_IR_SENSOR);
		irReadArray [1] = irSensorRead(MIDDLE_IR_SENSOR);
		irReadArray [2] = irSensorRead(RIGHT_IR_SENSOR);
		break;
    }

	return 0;
}
*/

// Security pin init function to set the pins to be input 
void initIrPins(void)
	{
		
		DDRB &= ~(1<<DDB0);
		DDRB &= ~(1<<DDB1);
		DDRB &= ~(1<<DDB2);
	}
	

// Function to read IR sensor data
int irSensorRead(int sensor_select)
{
	if (sensor_select == LEFT_IR_SENSOR)
	{
		if (PINB & (1<<PINB0))
		{
			return LINE_DETECTED;
		}
		else
		{
			return LINE_NOT_DETECTED;
		}
	}
	
	else if (sensor_select == MIDDLE_IR_SENSOR)
	{
		if (PINB & (1<<PINB1))
		{
			return LINE_DETECTED;
		}
		else
		{
			return LINE_NOT_DETECTED;
		}
	}
	
	else if (sensor_select == RIGHT_IR_SENSOR)
	{
		if (PINB & (1<<PINB2))
		{
			return LINE_DETECTED;
		}
		else
		{
			return LINE_NOT_DETECTED;
		}
	}
}