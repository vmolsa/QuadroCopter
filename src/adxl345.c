#include "config.h"

#define ADXL345_ENABLE "adxl34x 0x53"
#define ADXL345_DISABLE "0x53"

int initADXL345() {
	if (enableDev(ADXL345_ENABLE) < 0) {
		return -1;
	}

	return 0;
}

int freeADXL345() {
	if (disableDev(ADXL345_DISABLE) < 0) {
                return -1;
        }

        return 0;

}

int getADXL345() {


	return 0;
}
