#include "utils.h"
#include "config.h"
#include "i2c.h"
#include "accelerometer.h"

int adxl345 = 0;

void initAccel() {
	if (I2C_WhoIAm(ADXL345_DEVICE) == 0xE5) {
		adxl345 = 1;

		I2C_setRegister(ADXL345_DEVICE, 0x2D, (1 << 3));	// Set device to measure mode!
		I2C_setRegister(ADXL345_DEVICE, 0x31, 0x09);		// Set device to full range!
		I2C_setRegister(ADXL345_DEVICE, 0x2C, (11)); 		// 200hz Sampling

		delay(10);
	}
}

void getAccel(int *x, int *y, int *z) {
	I2C_writeByteTo(ADXL345_DEVICE, 0x32);				// Request 6 bytes from device
	I2C_RequestFrom(ADXL345_DEVICE, 6);

	(*x) = I2C_readIntReverse();
	(*y) = I2C_readIntReverse();
	(*z) = I2C_readIntReverse();
}
