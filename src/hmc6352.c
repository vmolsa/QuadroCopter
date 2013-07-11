#include "config.h"

#define HMC6352_ENABLE	"hmc6352 0x21"
#define HMC6352_DISABLE	"0x21"

#define HMC6352_HEADING	"/sys/bus/i2c/devices/i2c-1/1-0021/hmc6352/heading0_input"

int fd = -1;
int direction = 0;

int initHMC6352() {
        if (enableDev(HMC6352_ENABLE) < 0) {
                return -1;
        }

        fd = openFile(HMC6352_HEADING);

	return 0;
}

int freeHMC6352() {
        if (disableDev(HMC6352_DISABLE) < 0) {
                return -1;
        }

	closeFile(fd);

        return 0;
}

int getHMC6352() {
        direction = readInt(fd);

	LOG("Compass: Direction(%d)\n", direction);

	return 0;
}
