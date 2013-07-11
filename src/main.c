#include "config.h"

int main() {
	int i;

//	initADXL345();
	initBMP085();
	initHMC6352();
	initL3G4200D();

	// LOOP

	for (i = 0; i < 100; i++) {
//		getADXL345();
		getBMP085();
		getHMC6352();
		getL3G4200D();
	}

//	freeADXL345();
        freeBMP085();
        freeHMC6352();
        freeL3G4200D();

	return 0;
}
