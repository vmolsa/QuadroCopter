#include "barometric.h"

#define BMP085_DEVICE 0x77

//	OverSampling
//	0 = ultra low power
//	1 = standard
//	2 = high
//	3 = ultra high resolution

int bmp085 = 0;

byte over = 3;

int ac1 = 0, ac2 = 0, ac3 = 0, b1 = 0, b2 = 0, mb = 0, mc = 0, md = 0;
unsigned int ac4 = 0, ac5 = 0, ac6 = 0;

void initBaro() {
	I2C_writeByteTo(BMP085_DEVICE, 0xD0);

	if (I2C_readByteFrom(BMP085_DEVICE) == 0x55) {
		bmp085 = 1;

		I2C_writeByteTo(BMP085_DEVICE, 0xAA);
		I2C_RequestFrom(BMP085_DEVICE, 22);

		ac1 = I2C_readInt();
		ac2 = I2C_readInt();
		ac3 = I2C_readInt();
		ac4 = (unsigned int) I2C_readInt();
		ac5 = (unsigned int) I2C_readInt();
		ac6 = (unsigned int) I2C_readInt();
		b1 = I2C_readInt();
		b2 = I2C_readInt();
		mb = I2C_readInt();
		mc = I2C_readInt();
		md = I2C_readInt();
	}
}

void getBaro(int *altitude, int *pressure, int *temperature) {


}
