#include "utils.h"
#include "config.h"
#include "i2c.h"

#ifdef __GNUC__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

static int fd = -1, pos = 0, avail = 0;
static unsigned char buffer[256];

void initI2C() {
	memset(buffer, 0, sizeof(buffer));

	if ((fd = open(I2CDEV, O_RDWR)) < 0) {
		LOG("Unable to open i2c device!\n");
		exit(0);
	}
}

void I2C_address(byte address) {
	if (ioctl(fd, I2C_SLAVE, address) < 0) {
		LOG("Unable to set i2c device slave address(%c)\n", address);
	}
}

void I2C_RequestFrom(byte address, int count) {
	if (count > sizeof(buffer)) {
		count = sizeof(buffer);
	}

	I2C_address(address);

	pos = 0;

	if ((avail = read(fd, buffer, count)) != count) {
		LOG("Unable to read data from i2c slave address(%c) requested(%d)\n", address, count);
	}
}

byte I2C_readByte() {
	byte ret = 0;

	if (pos < avail) {
		ret = buffer[pos];
		pos++;
	}

	return ret;
}

byte I2C_readByteFrom(byte address) {
	byte ret = 0;
	I2C_address(address);

	if (read(fd, &ret, sizeof(byte)) != sizeof(byte)) {
		LOG("Unable to read data from i2c slave address(%c)\n", address);
	}
}

int I2C_readInt() {
	return ((I2C_readByte() << 8) | I2C_readByte());
}

int I2C_readIntReverse() {
	return (I2C_readByte() | (I2C_readByte() << 8));
}

int I2C_readIntFrom(byte address) {
	int ret = 0;
        I2C_address(address);

        if (read(fd, &ret, sizeof(int)) != sizeof(int)) {
                LOG("Unable to read data from i2c slave address(%c)\n", address);
        }
}

int I2C_readIntFromReverse(byte address) {
	I2C_RequestFrom(address, 2);
	return (I2C_readByte() | (I2C_readByte() << 8));
}

void I2C_writeByteTo(byte address, byte value) {
	I2C_address(address);

	if (write(fd, &value, sizeof(byte)) != sizeof(byte)) {
		LOG("Unable to write data to i2c slave address(%c)\n", address);
	}
}

byte I2C_WhoIAm(byte address) {
	byte val = 0;

	I2C_writeByteTo(address, val);

	delay(100);

	return I2C_readByteFrom(address);
}

void I2C_setRegister(byte address, byte registerAddr, byte registerValue) {
	unsigned int value = (unsigned int) ((registerAddr << 8) | registerValue);
	I2C_address(address);

	if (write(fd, &value, sizeof(byte)) != sizeof(byte)) {
		LOG("Unable to write data to i2c slave address(%c)\n", address);
	}
}

#else
#include <Wire.h>

void initI2C() {
	I2c.begin();
}

void I2C_RequestFrom(byte address, int count) {
	Wire.requestFrom(address, count);
}

byte I2C_readByte() {
	return Wire.read();
}

byte I2C_readByteFrom(byte address) {
	Wire.requestFrom(address, 1);
	return Wire.read();
}

int I2C_readInt() {
	return ((Wire.read() << 8) | Wire.read());
}

int I2C_readIntReverse() {
	return (Wire.read() | (Wire.read() << 8));
}

int I2C_readIntFrom(byte address) {
	Wire.requestFrom(address, 2);
	return ((Wire.read() << 8) | Wire.read());
}

int I2C_readIntFromReverse(byte address) {
	Wire.requestFrom(address, 2);
	return (Wire.read() | (Wire.read() << 8));
}

void I2C_writeByteTo(byte address, byte value) {
	Wire.beginTransmission(address);
	Wire.write(value);
	Wire.endTransmission();
}

byte I2C_WhoIAm(byte address) {
	Wire.beginTransmission(address);
	Wire.write((byte)0);
	Wire.endTransmission();

	delay(100);

	Wire.requestFrom(address, 1);
	return Wire.read();
}

void I2C_setRegister(byte address, byte registerAddr, byte registerValue) {
	Wire.beginTransmission(address);
	Wire.write(registerAddr);
	Wire.write(registerValue);
	Wire.endTransmission();
}

#endif
