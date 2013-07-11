#include "config.h"

static char tmp_buffer[1024];

int enableDev(char *str) {
        int fd = -1;

        if ((fd = open(ENABLEI2CDEV, O_WRONLY)) < 0) {
                LOG("Unable to enable i2c device (%s)\n", str);
                return -1;
        }

        if (write(fd, str, strlen(str)) != strlen(str)) {
                LOG("Unable to write enable string to i2c! (%s)\n", str);
                return -1;
        }

        close(fd);
        return 0;
}

int disableDev(char *str) {
        int fd = -1;

        if ((fd = open(DISABLEI2CDEV, O_WRONLY)) < 0) {
                LOG("Unable to disable i2c device (%s)\n", str);
                return -1;
        }

        if (write(fd, str, strlen(str)) != strlen(str)) {
                LOG("Unable to write disable string to i2c! (%s)\n", str);
                return -1;
        }

        close(fd);
        return 0;
}

int openFile(char *path) {
	int fd = -1;

        if ((fd = open(path, O_RDONLY)) < 0) {
                LOG("Unable to open file(%s)!\n", path);
        }

	return fd;
}

void closeFile(int fd) {
	if (fd != -1) {
		close(fd);
	}
}

ssize_t readBuffer(int fd, char *buffer, size_t size) {
	int ret = -1;
	memset(buffer, 0, size);

	if ((ret = read(fd, buffer, size)) < 0) {
		LOG("Unable to read file!\n");
	}

	return ret;
}

int readInt(int fd) {
	int ret = 0;

	if (readBuffer(fd, tmp_buffer, sizeof(tmp_buffer)) > 0) {
		ret = atoi(tmp_buffer);
	}

	return ret;
}

float readFloat(int fd) {
	float ret = 0;

	if (readBuffer(fd, tmp_buffer, sizeof(tmp_buffer)) > 0) {
		ret = atof(tmp_buffer);
	}

	return ret;
}

unsigned long getTime() {
	unsigned long ret = 0;
	static struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	ret = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

	return ret;
}
