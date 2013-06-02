#include "compass.h"

#define HMC6352_DEVICE 0x21

void initCompass() {

}

void getCompass(int *degree) {
	unsigned char low, high;

	I2C_writeByteTo(ADXL345_DEVICE, 'A');

	delay(10);

	I2C_RequestFrom(ADXL345_DEVICE, 2);

	low = I2C_readByte();
	high = I2C_readByte();

	degree = ((low * 256) + high); 
}
