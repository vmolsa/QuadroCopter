#include "spi.h"

void initSPI() {
	pinMode(SPI_SS, OUTPUT);		// Set device to master
	pinMode(SPI_MOSI, OUTPUT);
	pinMode(SPI_MISO, INPUT);
	pinMode(SPI_SCLK, OUTPUT);

	SPCR = (1 << SPE) | (1 << MSTR); 	// SPCR = 01010000
	SPSR;
	SPDR;

	delay(10);
}
