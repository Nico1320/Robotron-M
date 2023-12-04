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

/*
int irReadArray [3];
int leftIrSensorRead(void);
int middleIrSensorRead(void);
int rightIrSensorRead(void);
*/

/*
int main(void)
{
	
	
// Only for testing
    while (1) 
    {
		irReadArray [0] = leftIrSensorRead();
		irReadArray [1] = middleIrSensorRead();
		irReadArray [2] = rightIrSensorRead();
		break;
    }

	return 0;
}
*/

void initIrPins(void)
	{
		// pin set to input (security check)
		DDRB &= ~(1<<DDB0);
		DDRB &= ~(1<<DDB1);
		DDRB &= ~(1<<DDB2);
	}
	
// Returns left IR sensor reading
int leftIrSensorRead(void)
	{
		if ((PINB & (1<<PINB0)) == 1)
		{
			return LINE_DETECTED;
		}
		else 
		{
			return LINE_NOT_DETECTED;
		}
	}
	
// Returns middle IR sensor reading
int middleIrSensorRead(void)
{
	if ((PINB & (1<<PINB1)) == 1)
	{
		return LINE_DETECTED;
	}
	else
	{
		return LINE_NOT_DETECTED;
	}
}

// Returns right IR sensor reading
int rightIrSensorRead(void)
{
	if ((PINB & (1<<PINB2)) == 1)
	{
		return LINE_DETECTED;
	}
	else
	{
		return LINE_NOT_DETECTED;
	}
}
