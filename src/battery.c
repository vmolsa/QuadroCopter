#include "utils.h"
#include "config.h"
#include "battery.h"

#ifdef __GNUC__
#define BATTERY_PIN 0
#else
#define BATTERY_PIN A0
#endif

#define BATTERY_FULL_CELL 4.2
#define BATTERY_STD_CELL 3.7
#define BATTERY_LOW_CELL 3.4
#define BATTERY_CELL_COUNT 6
#define BATTERY_CALIB 4.91
#define BATTERY_FULL_VOLTAGE (BATTERY_FULL_CELL * BATTERY_CELL_COUNT)
#define BATTERY_LOW_VOLTAGE (BATTERY_LOW_CELL * BATTERY_CELL_COUNT)

void initBattery() {

}

void getBattery(float *voltage, int *percent) {
#ifdef __GNUC__
	int val = 0;
#else
	int val = analogRead(BATTERY_PIN);
#endif

	(*voltage) = (float) (((BATTERY_CALIB * val) / 1023) * BATTERY_CELL_COUNT);
	(*percent) = (((((BATTERY_CALIB * val) / 1023) - BATTERY_LOW_VOLTAGE) / (BATTERY_FULL_VOLTAGE - BATTERY_LOW_VOLTAGE)) * 100);
}

