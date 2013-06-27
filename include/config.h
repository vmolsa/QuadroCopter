#ifndef _CONFIG_H
#define _CONFIG_H

// 	Global Config

#define HAVE_DEBUG 1
//#define HAVE_LOG 1
//#define HAVE_BATTERY 1
#define HAVE_ACCELL 1
#define HAVE_GYRO 1
#define HAVE_COMPASS 1
//#define HAVE_ESC 1
//#define HAVE_GPS 1
#define HAVE_BARO 1
//#define HAVE_SONAR 1

//	I2C Config

#define ADXL345_DEVICE 0x53
#define BMP085_DEVICE 0x77
#define HMC6352_DEVICE 0x21
#define L3G4200D_DEVICE 0x68

//	Linux Config

#ifdef __GNUC__

#define I2CDEV "/dev/i2c-1"

#endif
#endif
