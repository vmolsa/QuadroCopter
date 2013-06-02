#ifndef _I2C_H
#define _I2C_H

void initI2C();
void I2C_RequestFrom(int address, int count);

byte I2C_readByte();
byte I2C_readByteFrom(int address);

int I2C_readInt();
int I2C_readIntReverse();
int I2C_readIntFrom(int address);
int I2C_readIntFromReverse(int address);
int I2C_readIntWait(int address);

void I2C_writeByteTo(int address, byte value);
byte I2C_WhoIAm(int address);

void I2C_setRegister(int address, byte registerAddr, byte registerValue);

#endif
