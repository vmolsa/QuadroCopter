#ifndef _I2C_H
#define _I2C_H

#include "utils.h"

void initI2C();
void I2C_RequestFrom(byte address, int count);

byte I2C_readByte();
byte I2C_readByteFrom(byte address);

int I2C_readInt();
int I2C_readIntReverse();
int I2C_readIntFrom(byte address);
int I2C_readIntFromReverse(byte address);
void I2C_writeByteTo(byte address, byte value);
byte I2C_WhoIAm(byte address);

void I2C_setRegister(byte address, byte registerAddr, byte registerValue);

#endif
