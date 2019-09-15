/*************************************************** 
  This is a library for the Adafruit Thermocouple Sensor w/MAX31855K
  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269
  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef MAX31855_H
#define MAX31855_H

#include "device.h"

void InitSPI();
double MAX31855_readCelsius(void);
double MAX31855_readFarenheit(void);
double MAX31855_readInternal(void);
uint32_t spi_read();


#endif
