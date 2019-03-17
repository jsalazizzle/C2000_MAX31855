#include "device.h"
#include "math.h"

#include "uart_if.h"

#define CS_LOW                      GPIO_writePin(11, 0)
#define CS_HIGH                     GPIO_writePin(11, 1)

void InitSPI()
{
    //
    // Must put SPI into reset before configuring it
    //
    SPI_disableModule(SPIA_BASE);

    //
    // SPI configuration. Use a 4MHz SPICLK and 16-bit word size.
    //
    SPI_setConfig(SPIA_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_MASTER, 4000000, 16);

    SPI_setEmulationMode(SPIA_BASE, SPI_EMULATION_STOP_AFTER_TRANSMIT);

    //SPI Pin Configuration
    GPIO_setPadConfig(16,GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_16_SPISIMOA);
    GPIO_setQualificationMode(16,GPIO_QUAL_ASYNC);

    GPIO_setPadConfig(56,GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_56_SPICLKA);
    GPIO_setQualificationMode(56,GPIO_QUAL_ASYNC);

    GPIO_setPadConfig(17,GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_17_SPISOMIA);
    GPIO_setQualificationMode(17,GPIO_QUAL_ASYNC);

    GPIO_setPadConfig(11,GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_11_SPISTEA);
    GPIO_setDirectionMode(11,GPIO_DIR_MODE_OUT);

    //
    // Configuration complete. Enable the module.
    //
    SPI_enableModule(SPIA_BASE);
}

uint32_t spi_read() {
    uint16_t sData = 0;                  // Send data
    uint32_t rData = 0;                  // Receive data

    while(SPI_isBusy(SPIA_BASE));        // Wait until SPI bus is ready

    CS_LOW;

    // Transmit data
    SPI_writeDataBlockingNonFIFO(SPIA_BASE, sData);
    SPI_writeDataBlockingNonFIFO(SPIA_BASE, sData);

    // Block until data is received and then return it
    rData = SPI_readDataBlockingNonFIFO(SPIA_BASE);
    rData <<= 16;
    rData |= SPI_readDataBlockingNonFIFO(SPIA_BASE);

    CS_HIGH;
    return rData;
}

double MAX31855_readCelsius(){
    uint32_t v;

    v = spi_read();

    if (v & 0x7) {
        if(v & 0x04)
            Message("ERROR: short to VCC");
        if(v & 0x02)
            Message("ERROR: short to GND");
        if(v & 0x01)
            Message("ERROR: open circuit");
        return NAN;
    }

    if (v & 0x80000000) {
        v = 0xFFFFC000 | ((v >> 18) & 0x00003FFFF);
    } else {
        v >>= 18;
    }

    double centigrade = v;
    centigrade *= 0.25;

    return centigrade;
}

double MAX31855_readFarenheit(void) {
    float f = MAX31855_readCelsius();
    f *= 9.0;
    f /= 5.0;
    f += 32;
    return f;
}

double MAX31855_readInternal(void){
    uint32_t v;

    v = spi_read();
    v >>= 4;

    float internal = v & 0x7FF;
    if (v & 0x800) {
        int16_t tmp = 0xF800 | (v & 0x7FF);
        internal = tmp;
    }
    internal *= 0.0625;
    return internal;
}

