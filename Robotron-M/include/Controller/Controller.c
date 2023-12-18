#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>

// Include modules
#include "./H_bridge/HBridge.h"
#include "./Soft_serial/SoftSerial.h"
#include "./Ultrasonic/Ultrasonic.h"
#include "./Display/lcd.h"
#include "./LEDrgb/ws28xx.h"
#include "./IR/IR.h"
//#include "./Baremetal/Baremetal.h"
//#include "./Bit_bang/ShiftRegister.h"

// Defines
#define DELAY_TIME_MS 35 // Set the sampling time in milliseconds

// IR defines for selecting the sensor to read
#define SENSOR_LEFT 0
#define SENSOR_MIDDLE 1
#define SENSOR_RIGHT 2

// Ultrasonic defines
#define MINIMUM_DISTANCE 10

// global variable init
bool serialFlag;
bool keyPressed = false;
bool debugMode;
float inputRatio = 0.8;
// Slave mode interruption flag set by ISR
bool slaveModeActive = false;

//Function prototypes
void manualMode();
void automaticMode();
void slaveMode();
void initializeModules();

void manualMode() {
	// Wait for USART data to become available
	while (!serialFlag) {
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(" Serial not ");
		lcd_gotoxy(0, 1);
		lcd_puts(" Detected");
		serialFlag = usart0_receive();
		if (serialFlag) {
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts(" Serial");
			lcd_gotoxy(0, 1);
			lcd_puts("Detected!");
			usart0_transmit_str("Receive ready!\r\n");
			_delay_ms(3000);
			serialFlag = false;
			break;
		}
	}

	// Manual mode loop
	while (!serialFlag) {
		// Check for available USART data
		if (usart0_nUnread() > 0) {
			char data = usart0_receive();
			switch (data) {
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

			// Set flag and introduce delay
			keyPressed = true;
			_delay_ms(DELAY_TIME_MS);
			} else {
			// Clear previous state if flag is set
			if (keyPressed) {
				clearPrevious();
				keyPressed = false; // Reset the flag
			}
		}
	}

	// Clear USART registers after the loop
	clearUSART();
}


void automaticMode() {
		turningRatio(inputRatio);
		char ratioinput[8]; /*destination array size defined*/
		char Lreslt[8];
		char Rresult[8];
		float Lspeed = OCR0A;
		float Rspeed = OCR0B;
		dtostrf(inputRatio, 5, 1, ratioinput); /*Destination string is printed*/

		dtostrf(Lspeed, 5, 0, Lreslt); /*Destination string is printed*/
		dtostrf(Rspeed, 5, 0, Rresult); /*Destination string is printed*/

		usart0_transmit_str(ratioinput);
		usart0_transmit_str("\r\n");
		usart0_transmit_str(Lreslt);
		usart0_transmit_str("\r\n");
		usart0_transmit_str(Rresult);
		usart0_transmit_str("\r\n");

}

void slaveMode() {
	
	slaveModeActive = true;
	
	while(slaveModeActive)
	{
		// Reading the 3 IR sensors
		int leftIrRead = irSensorRead(SENSOR_LEFT);
		int middleIrRead = irSensorRead(SENSOR_MIDDLE);
		int rightIrRead = irSensorRead(SENSOR_RIGHT);
		
		// Ultrasonic read
		int ulsRead = (int)ultrasonicRead();
		
		// stops the car until distance > MINIMUM_DISTANCE
		if(ulsRead < MINIMUM_DISTANCE)
		{
			// stopfunction(); has to be implemented
		}
		
		// In all cases, MINIMUM_DISTANCE should be reviewed and replaced (if needed) to handle a predecessor which is getting closer but not within the minimum range
		// Right turn
		else if(ulsRead > MINIMUM_DISTANCE && (leftIrRead || (leftIrRead && middleIrRead)))
		{	
			// discuss the exact value and use #define + possible setPWM() refactor
			turningRatio(2.0);
		}
		
		// Go straight
		else if(ulsRead > MINIMUM_DISTANCE && middleIrRead)
		{
				goForward(); // possible setPWM refactor also
		}
			
		// Left turn
		else if(ulsRead > MINIMUM_DISTANCE && (rightIrRead || (rightIrRead && middleIrRead)))
		{
			// discuss the exact value and use #define + possible setPWM() refactor
			turningRatio(0.6);
		}
		
		// Delay is needed here to minimize the frequency of direction change
	}
	
}

void initializeModules() {
	usart0_init();

	initializeHbridge();
	setupTimer();

	// Display init
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);

	usart0_transmit_str("Main loop running!\r\n");
//	ultrasonicInit();
	sei();
}

void rgbtest() {
	  while(1) {
		  led[0].r=255;led[0].g=00;led[0].b=0;    // Write red to array
		  ws2812_setleds(led,1);
		  _delay_ms(500);                         // wait for 500ms.

		  led[0].r=0;led[0].g=255;led[0].b=0;			// green
		  ws2812_setleds(led,1);
		  _delay_ms(500);

		  led[0].r=0;led[0].g=00;led[0].b=255;		// blue
		  ws2812_setleds(led,1);
		  _delay_ms(500);
	  }
}

