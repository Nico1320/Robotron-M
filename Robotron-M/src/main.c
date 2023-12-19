#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include "./Controller/Controller.h"

int main(void) {
	initializeModules();
	Modeselect();
}