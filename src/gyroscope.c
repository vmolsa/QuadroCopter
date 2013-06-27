#include "utils.h"
#include "config.h"
#include "gyroscope.h"

int l3g4200d = 0;

void initGyro() {
	I2C_writeByteTo(L3G4200D_DEVICE, 0x0f);

	if (I2C_readByteFrom(L3G4200D_DEVICE) == 0xD3) {
		l3g4200d = 1;

		I2C_setRegister(L3G4200D_DEVICE, 0x20, 0x9F);		// Enable x, y, z and turn power off
		delay(5);
		I2C_setRegister(L3G4200D_DEVICE, 0x21, 0x00);		// HPF Enable
		delay(5);
		I2C_setRegister(L3G4200D_DEVICE, 0x23, 0xB0);		// Enable capture rate 2000
		delay(5);
		I2C_setRegister(L3G4200D_DEVICE, 0x24, 0x02);		// Enable High Pass filter
	}
}

void getGyro(int *x, int *y, int *z) {
	I2C_writeByteTo(L3G4200D_DEVICE, (0x80 | 0x28));		// Auto Increment, Request 6 bytes from device
	I2C_RequestFrom(L3G4200D_DEVICE, 6);

	(*x) = I2C_readIntReverse();
	(*y) = I2C_readIntReverse();
	(*z) = I2C_readIntReverse();
}
