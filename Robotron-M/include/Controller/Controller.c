// DEFINES & constants
/*#define DELAY_TIME_MS 35 // Set the sampling time in milliseconds
volatile uint8_t keyPressed = 0;


// Function prototypes
enum CarMode {Automatic, Slave, Manual};



ChangeMode() {

	switch (CarMode)
	{
	
	case Automatic
	
	break;
	
	case Slave
	break;
	
	case Manual
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

	break;
	
	default:
		if (debugMode) {
			usart0_transmit_str("No mode is selected\r\n")
			usart0_transmit_str("Restarting loop!\r\n")		
		}
	}
}
*/
#define go 1