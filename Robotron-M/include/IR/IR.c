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

// Safety pin init function to set the pins to be input 
void initIrPins(void)
	{
		
		DDRC &= ~(1<<DDC1);
		DDRC &= ~(1<<DDC2);
	}
	

// Function to read IR sensor data
int irSensorRead(int sensor_select) // sensor_select parameter is used to select IR sensor to read from
{
	// branching to read the corresponding sensor based on the function's input value
	if (sensor_select == LEFT_IR_SENSOR) 
	{
		if (PINC & (1<<PINC1)) // 
		{
			return LINE_DETECTED; // LINE_DETECTED is a #define directive and it equals to 1. The return value is used in other sections of the code.
		}
		else
		{
			return LINE_NOT_DETECTED; // LINE_NOT_DETECTED is a #define directive and it equals to 0. The return value is used in other sections of the code.
		}
	}
	
	else if (sensor_select == RIGHT_IR_SENSOR)
	{
		if (PINC & (1<<PINC2))
		{
			return LINE_DETECTED;
		}
		else
		{
			return LINE_NOT_DETECTED;
		}
	}
}