#ifndef _UTILS_H
#define _UTILS_H

#ifdef __GNUC__
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

typedef unsigned char byte;
typedef unsigned short int word;

#define delay(x) usleep(x*1000);
#define LOG(...) printf(__VA_ARGS__)

#endif
#endif
