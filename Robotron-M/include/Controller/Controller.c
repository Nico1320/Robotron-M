#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include <math.h>
#include "./Controller/Controller.h"

float inputRatio = 0.8;
bool serialFlag;
bool ModeActive = false;
bool slaveModeActive = true;
bool ScreenHome = false;
bool ButtonPressed;

CarMode_t mode;

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
#define DELAY_TIME_MS 800 // Set the sampling time in milliseconds
// IR defines for selecting the sensor to read
#define SENSOR_LEFT 0
#define SENSOR_MIDDLE 1
#define SENSOR_RIGHT 2
// Ultrasonic defines
#define MINIMUM_DISTANCE 12


void Serialcheck() {
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
}

void manualMode() {
	//Serialcheck();
	// Manual mode loop
	ScreenHome = false;
	if (usart0_nUnread() > 0) {
	char data = usart0_receive();
	
		switch (data) {
			case 'F':
			setPWM(255, 255);
			goForward();
			break;

			case 'G':
			setPWM(200, 200);
			goBackward();
			break;

			case 'L':
			setPWM(200, 200);
			zeroRadii(0);
			break;

			case 'R':
			setPWM(200, 200);
			zeroRadii(1);
			break;

			default:
			usart0_transmit_str("Invalid input\r\n");
			clearPrevious(0);
			break;
		}
	}

}


void automaticMode() {
	ScreenHome = false;
		//implement later
}

void slaveMode() {
ScreenHome = false;
slaveModeActive = true;

DDRB &= ~(1 << PINB7);

	while (!slaveModeActive) {
		if (!(PINB & (1 << PINB7))) { // Added a closing parenthesis here
			slaveModeActive = true;
		}
	}
	
	while (slaveModeActive) {
		char ultrares[5];
		volatile uint8_t ulsRead = ultrasonicRead();
		int leftIrRead = irSensorRead(SENSOR_LEFT);
		int rightIrRead = irSensorRead(SENSOR_RIGHT);
		usart0_transmit_str(dtostrf(ulsRead, 0, 0, ultrares));
		usart0_transmit_str("\r\n");
		
		
		if (ulsRead > MINIMUM_DISTANCE) {
			// Right turn
			if (leftIrRead) {
				goForward();
				//setPWM(150, 60);
				turningRatio(0.5, 150);
			}
			// Go straight
			else if (!leftIrRead && !rightIrRead) {
				goForward(); // possible setPWM refactor also
				//setPWM(100, 100);
				turningRatio(1, 150);
			}
			// Left turn
			else if (rightIrRead) {
				goForward();
				//setPWM(60,150);
				turningRatio(1.5, 150); // discuss the exact value and use #define + possible setPWM() refactor
				} else {
				
				zeroRadii(false);
				//setPWM(100, 100);
				turningRatio(1, 100);
				clearPrevious(false);
			}
			_delay_ms(DELAY_TIME_MS);
		}
	}
}


void initializeModules() {
	usart0_init();

	// Display init
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);

	usart0_transmit_str("Main loop running!\r\n");
	ultrasonicInit();
	initFreerunningADC();
	initializeHbridge();
	setupTimer();
	sei();
}




void Modeselect() {
	while (1) {
		if (!ModeActive && !ScreenHome) {
			mode = NoMode;
			systemDataPrint("Modeselect","\0");
			ScreenHome = true;			
		}
		
		else if (ModeActive && !ScreenHome) {
			systemDataPrint("Telemetrics", "\0");
		}
		ADCSRA |= (1 << ADSC); /* start first conversion */
		switch (mode) {
			case Automatic:
			automaticMode();
			break;
			case Slave:
			slaveMode();
			break;
			case Manual:
			manualMode();
			case NoMode:
			break;
		}
	}
}

void systemDataPrint(char *pdisplayMode, char *pInputString){
	// Store telemetrics data internally
	char storedString[10] = "  ";
	strcat(storedString,pInputString); // TBD with a function concatenation
	
	if(strcmp(pdisplayMode,"Telemetrics") == 0){
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("  Dir  SysT  Dist");
		lcd_gotoxy(0, 1);
		lcd_puts("pInputString");
	}
	
	else if(strcmp(pdisplayMode,"SerialNotDet") == 0){
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(" Serial not ");
		lcd_gotoxy(0, 1);
		lcd_puts(" Detected");
		serialFlag = usart0_receive();
	}
	
	else if(strcmp(pdisplayMode,"SerialDet") == 0){
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts(" Serial");
		lcd_gotoxy(0, 1);
		lcd_puts("Detected!");
	}
	
	else if(strcmp(pdisplayMode,"Modeselect") == 0){
		//ModeSelectmenu
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("  Mode Selection");
		lcd_gotoxy(0, 1);
		lcd_puts("  A |  B |  C ");
	}
	
}