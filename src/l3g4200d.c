#include "config.h"

#define L3G4200D_ENABLE		"l3g4200d 0x68"
#define L3G4200D_DISABLE	"0x68"

#define L3G4200D_X              "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_x_raw"
#define L3G4200D_X_SCALE        "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_x_scale"
#define L3G4200D_Y              "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_y_raw"
#define L3G4200D_Y_SCALE        "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_y_scale"
#define L3G4200D_Z              "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_z_raw"
#define L3G4200D_Z_SCALE        "/sys/bus/i2c/devices/i2c-1/1-0068/iio:device0/in_anglvel_z_scale"

int invert = 1;

float x = 0, y = 0, z = 0;
int raw_x = 0, raw_y = 0, raw_z = 0;
float scale_x = 0, scale_y = 0, scale_z =0;
float heading = 0;

unsigned long curtime = 0, lasttime = 0;

int fd_x = -1, fd_x_scale = -1;
int fd_y = -1, fd_y_scale = -1;
int fd_z = -1, fd_z_scale = -1;

int initL3G4200D() {
        if (enableDev(L3G4200D_ENABLE) < 0) {
                return -1;
        }

	fd_x = openFile(L3G4200D_X);
	fd_x_scale = openFile(L3G4200D_X_SCALE);

	fd_y = openFile(L3G4200D_Y);
	fd_y_scale = openFile(L3G4200D_Y_SCALE);

	fd_z = openFile(L3G4200D_Z);
	fd_z_scale = openFile(L3G4200D_Z_SCALE);

	scale_x = readFloat(fd_x_scale);
	scale_y = readFloat(fd_y_scale);
	scale_z = readFloat(fd_z_scale);

	closeFile(fd_x_scale);
	closeFile(fd_y_scale);
	closeFile(fd_z_scale);

	lasttime = getTime();

	return 0;
}

int freeL3G4200D() {
        if (disableDev(L3G4200D_DISABLE) < 0) {
                return -1;
        }

	closeFile(fd_x);
	closeFile(fd_y);
	closeFile(fd_z);

        return 0;
}

int getL3G4200D() {
	raw_x = readInt(fd_x);
	raw_y = readInt(fd_y);
	raw_z = readInt(fd_z);

	x += (((invert) ? -1 : 1) * raw_x * scale_x);
	y += (((invert) ? -1 : 1) * raw_y * scale_y);
	z += (((invert) ? -1 : 1) * raw_z * scale_z);

	curtime = getTime();

	if (z > (1.0 * 3.141592653589793 / 180.0) || z < (-1.0 * 3.141592653589793 / 180.0)) {
		heading += z * ((curtime - lasttime) / 1000000.0);
	}

	lasttime = curtime;

	LOG("Gyro: Heading(%f), X(Scale=%f, Raw=%d, Value=%f), Y(Scale=%f, Raw=%d, Value=%f), Z(Scale=%f, Raw=%d, Value=%f)\n", heading, scale_x, raw_x, x, scale_y, raw_y, y, scale_z, raw_z, z);

	return 0;
}
