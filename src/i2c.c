#include <Wire.h>
#include "i2c.h"

void initI2C() {
	I2c.begin();
}

void I2C_RequestFrom(int address, int count) {
	Wire.requestFrom(address, count);
}

byte I2C_readByte() {
	return Wire.read();
}

byte I2C_readByteFrom(int address) {
	Wire.requestFrom(address, 1);
	return Wire.read();
}

int I2C_readInt() {
	return ((Wire.read() << 8) | Wire.read());
}

int I2C_readIntReverse() {
	return (Wire.read() | (Wire.read() << 8));
}

int I2C_readIntFrom(int address) {
	Wire.requestFrom(address, 2);
	return ((Wire.read() << 8) | Wire.read());
}

int I2C_readIntFromReverse(int address) {
	Wire.requestFrom(address, 2);
	return (Wire.read() | (Wire.read() << 8));
}

int I2C_readIntWait(int address) {
	unsigned char msb, lsb;

	Wire.requestFrom(address, 2);

	while(!Wire.available());
	msb = Wire.read();

	while(!Wire.available());
	lsb = Wire.read();

	return (((int)msb << 8) | ((int) lsb));
}

void I2C_writeByteTo(int address, byte value) {
	Wire.beginTransmission(address);
	Wire.write(value);
	Wire.endTransmission();
}

byte I2C_WhoIAm(int address) {
	Wire.beginTransmission(address);
	Wire.write((byte)0);
	Wire.endTransmission();

	delay(100);
	
	Wire.requestFrom(address, 1);
	return Wire.read();
}

void I2C_setRegister(int address, byte registerAddr, byte registerValue) {
	Wire.beginTransmission(address);
	Wire.write(registerAddr);
	Wire.write(registerValue);
	Wire.endTransmission();
}

