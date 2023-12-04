#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"
//#include "./Baremetal/Baremetal.h"
//#include "./Bit_bang/ShiftRegister.h"

#define DELAY_TIME_MS 35 // Set the sampling time in milliseconds

// Global variable to track the state of key press
volatile uint8_t keyPressed = 0;


int main(void) {
	usart0_init();
	sei();
	initializeHbridge();
	setupTimer();
	usart0_transmit_str("Main loop running!\r\n");

	while (1)
	{
		if (usart0_nUnread() > 0)
		{
			char data = usart0_receive();
			switch (data)
			{
				case 'w':
				setPWM(255, 255);
				goForward();
				break;

				case 's':
				setPWM(200, 200);
				goBackward();
				break;

				case 'a':
				setPWM(200, 200);
				zeroRadii(0);
				break;

				case 'd':
				setPWM(200, 200);
				zeroRadii(1);
				break;

				default:
				usart0_transmit_str("Invalid input\r\n");
				break;
			}

			// Set the flag to indicate a key was pressed
			keyPressed = 1;

			// Introduce a delay for sampling time
			_delay_ms(DELAY_TIME_MS);
		}
		else
		{
			// Check if no keys are pressed after a key press
			if (keyPressed)
			{
				clearPrevious();
				keyPressed = 0; // Reset the flag
			}
		}
	}

	// Clear USART registers after the loop	
	clearUSART();
	return 0;
}
