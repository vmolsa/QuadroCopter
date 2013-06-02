#ifndef _SPI_H
#define _SPI_H

#if defined(__AVR_ATmega2560__)

#define SPI_MOSI 51
#define	SPI_MISO 50
#define SPI_SCLK 52
#define SPI_SS 53

#endif 

void initSPI();

#endif
