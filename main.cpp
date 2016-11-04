#include "MicroBit.h"
#include <LPD8806.h>
#include <LightAnimation.h>

MicroBit uBit;
LPD8806 strip(6);
LightAnimation animation(&strip, &uBit);

int main() {
    uBit.init();
    strip.begin();
    
    int speed = 50;
        
    while(true) {
        
        // Send a simple pixel chase in...
        animation.colorChase(strip.Color(127, 127, 127), speed); // White
        animation.colorChase(strip.Color(127,   0,   0), speed); // Red
        animation.colorChase(strip.Color(127, 127,   0), speed); // Yellow
        animation.colorChase(strip.Color(  0, 127,   0), speed); // Green
        animation.colorChase(strip.Color(  0, 127, 127), speed); // Cyan
        animation.colorChase(strip.Color(  0,   0, 127), speed); // Blue
        animation.colorChase(strip.Color(127,   0, 127), speed); // Violet
        
        // Send a theater pixel chase in...
        animation.theaterChase(strip.Color(127, 127, 127), speed); // White
        animation.theaterChase(strip.Color(127,   0,   0), speed); // Red
        animation.theaterChase(strip.Color(127, 127,   0), speed); // Yellow
        animation.theaterChase(strip.Color(  0, 127,   0), speed); // Green
        animation.theaterChase(strip.Color(  0, 127, 127), speed); // Cyan
        animation.theaterChase(strip.Color(  0,   0, 127), speed); // Blue
        animation.theaterChase(strip.Color(127,   0, 127), speed); // Violet
        
        // Fill the entire strip with...
        animation.colorWipe(strip.Color(127, 127, 127), speed); // White
        animation.colorWipe(strip.Color(127,   0,   0), speed); // Red
        animation.colorWipe(strip.Color(127, 127,   0), speed); // Yellow
        animation.colorWipe(strip.Color(  0, 127,   0), speed); // Green
        animation.colorWipe(strip.Color(  0, 127, 127), speed); // Cyan
        animation.colorWipe(strip.Color(  0,   0, 127), speed); // Blue
        animation.colorWipe(strip.Color(127,   0, 127), speed); // Violet
        
        animation.rainbow(10);
       
        animation.rainbowCycle(0);  
        animation.theaterChaseRainbow(speed);
    }
    
    release_fiber();
}