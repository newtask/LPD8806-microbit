/*
 * Port from  https://github.com/adafruit/LPD8806
 */

/* 
 * File:   LightAnimation.h
 * Author: taske
 *
 * Created on 4. November 2016, 22:23
 */

#ifndef LIGHTANIMATION_H
#define LIGHTANIMATION_H

#include <LPD8806.h>
#include "MicroBit.h"

class LightAnimation {
public:
    LightAnimation(LPD8806 *strip, MicroBit *uBit);
    void colorChase(uint32_t c, uint8_t wait);
    void theaterChase(uint32_t c, uint8_t wait);
    void colorWipe(uint32_t c, uint8_t wait);
    void rainbow(uint8_t wait);
    void rainbowCycle(uint8_t wait);
    void theaterChaseRainbow(uint8_t wait);
private:
    uint32_t Wheel(uint16_t WheelPos);
    LPD8806 *strip;
    MicroBit *uBit;
};

#endif /* LIGHTANIMATION_H */

