#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"

#define SAMPLING_TIME_MS 800 // Set the sampling time in milliseconds

// Global variable to track the state of key press
volatile uint8_t keyPressed = 0;

void clearUSART() {
	// Disable USART
	UCSR0B = 0;

	// Clear USART registers
	UCSR0A = 0;
	UCSR0C = 0;
	UBRR0H = 0;
	UBRR0L = 0;
}

int main(void) {
	usart0_init();
	sei();
	initializeHbridge();
	setupTimer();
	usart0_transmit_str("\r\nMain loop running!\r\n");

	while (1)
	{
		if (usart0_nUnread() > 0)
		{
			char data = usart0_receive();
			switch (data)
			{
				case 'w':
				setPWM(200, 200);
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
				usart0_transmit_str("No input detected\r\n");
				clearPrevious();
				break;
			}

			// Set the flag to indicate a key was pressed
			keyPressed = 1;

			// Introduce a delay for sampling time
			_delay_ms(SAMPLING_TIME_MS);
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

	return 0; // This is just for completeness; main() should not reach here.
}
