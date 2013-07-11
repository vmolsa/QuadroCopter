#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"

#define I2CBUS
#define ENABLEI2CDEV		"/sys/bus/i2c/devices/i2c-1/new_device"
#define DISABLEI2CDEV		"/sys/bus/i2c/devices/i2c-1/delete_device"

#endif
