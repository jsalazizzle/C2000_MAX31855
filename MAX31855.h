#ifndef MAX31855_H
#define MAX31855_H

#include "device.h"

void InitSPI();
double MAX31855_readCelsius(void);
double MAX31855_readFarenheit(void);
double MAX31855_readInternal(void);
uint32_t spi_read();


#endif
