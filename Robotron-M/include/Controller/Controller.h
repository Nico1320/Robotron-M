/*
 * Controller.h
 *
 * Created: 12/11/2023 5:19:52 PM
 *  Author: brand
 */ 


#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include <math.h>

extern bool ButtonPressed;
extern bool ModeActive;
extern bool serialFlag;
extern bool debugMode;
extern bool ScreenHome;

typedef enum {
	Automatic,
	Slave,
	Manual,
	NoMode
} CarMode_t;

extern CarMode_t mode;

void manualMode();
void automaticMode();
void slaveMode();
void initializeModules();
void Modeselect();
void initFreerunningADC();
void systemDataPrint(char *pdisplayMode, char *pInputString);


#endif /* CONTROLLER_H_ */