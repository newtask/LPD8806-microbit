/*
 * Port from  https://github.com/adafruit/LPD8806
 */


/* 
 * File:   LPD8806.h
 * Author: taske
 *
 * Created on 27. Oktober 2016, 23:06
 */

#ifndef LPD8806_H
#define LPD8806_H

#include "MicroBit.h"

class LPD8806 {
public:
    LPD8806(uint16_t n, PinName dpin, PinName cpin); // Configurable pins
    LPD8806(uint16_t n); // Use SPI hardware; specific pins only
    LPD8806(void); // Empty constructor; init pins & strip length later
    
    void 
        begin(void),
        setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
        setPixelColor(uint16_t n, uint32_t c),
        show(void),
        updatePins(PinName dpin, PinName cpin), // Change pins, configurable
        updatePins(void),                       // Change pins, hardware SPI
        updateLength(uint16_t n);               // Change strip length
    uint16_t
        numPixels(void);
    uint32_t
        Color(size_t r, size_t g, size_t b),
        getPixelColor(uint16_t n);
    
    // test stuff
    void draw(u_int color);
private:
    void 
        init(int numLeds),
        writeZeroes(),
        spi_out(int value),
        startSPI(void);
    uint16_t 
        numLEDs,
        numBytes;
    uint8_t 
        * pixels;
    PinName
        datapin = MICROBIT_PIN_P1,
        clkpin = MICROBIT_PIN_P0;
      
    bool begun;

    SPI *spi;
};

#endif /* LPD8806_H */

