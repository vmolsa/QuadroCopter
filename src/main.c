#include "accelerometer.h"
#include "analog.h"
#include "barometric.h"
#include "battery.h"
#include "compass.h"
#include "config.h"
#include "esc.h"
#include "gpio.h"
#include "gps.h"
#include "gyroscope.h"
#include "i2c.h"
#include "sonar.h"
#include "utils.h"

int main() {
	initI2C();

#ifdef HAVE_LOG
	initLog();
#endif

#ifdef HAVE_ACCELL
	initAccel();
#endif

#ifdef HAVE_BARO
	initBaro();
#endif

#ifdef HAVE_BATTERY
	initBattery();
#endif

#ifdef HAVE_COMPASS
	initCompass();
#endif

#ifdef HAVE_ESC
	initEsc();
#endif

#ifdef HAVE_GPS
	initGps();
#endif

#ifdef HAVE_GYRO
	initGyro();
#endif

#ifdef HAVE_SONAR
	initSonar();
#endif
	return 0;
}
