#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include "./Controller/Controller.h"

int main(void) {
	initializeModules();

	while (1) {
		enum CarMode {Automatic, Slave, Manual};
		enum CarMode mode = Automatic;

		switch (mode) {
			case Automatic:
			automaticMode();
			// Break using interrupt

			case Slave:
			slaveMode();
			// Break using interrupt

			case Manual:
			// Break using interrupt
			manualMode();
			
			default:
			break;
		}
	}
}