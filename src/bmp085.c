#include "config.h"

#define BMP085_ENABLE		"bmp085 0x77"
#define BMP085_DISABLE		"0x77"

#define BMP085_PRESSURE         "/sys/bus/i2c/devices/i2c-1/1-0077/pressure0_input"
#define BMP085_TEMPERATURE      "/sys/bus/i2c/devices/i2c-1/1-0077/temp0_input"

static int fd_p = -1;
static int fd_t = -1;

static int temperature = 0;
static int pressure = 0;
static float p0 = 101325;
static float altitude = 0;

int initBMP085() {
        if (enableDev(BMP085_ENABLE) < 0) {
                return -1;
        }

	fd_p = openFile(BMP085_PRESSURE);
	fd_t = openFile(BMP085_TEMPERATURE);

	return 0;
}

int freeBMP085() {
        if (disableDev(BMP085_DISABLE) < 0) {
                return -1;
        }

	closeFile(fd_p);
	closeFile(fd_t);

        return 0;
}

int getBMP085() {
	pressure = readInt(fd_p);
	temperature = readInt(fd_t);
	altitude = (float)44330 * (1 - pow(((float) pressure / p0), 0.190295));

	LOG("Barometric: Altitude(%.2fm), Pressure(%d), Temperature(%.2f)\n", altitude, pressure, (float)(temperature / 10));

	return 0;
}
