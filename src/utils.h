#ifndef _UTILS_H
#define _UTILS_H

#define LOG(...) printf(__VA_ARGS__);

int enableDev(char *str);
int disableDev(char *str);

int openFile(char *path);
void closeFile(int fd);

ssize_t readBuffer(int fd, char *buffer, size_t size);
int readInt(int fd);
float readFloat(int fd);

unsigned long getTime();

#endif
