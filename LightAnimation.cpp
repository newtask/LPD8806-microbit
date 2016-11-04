/*
 * Port from  https://github.com/adafruit/LPD8806
 */

/* 
 * File:   LightAnimation.cpp
 * Author: taske
 * 
 * Created on 4. November 2016, 22:23
 */

#include "LightAnimation.h"

LightAnimation::LightAnimation(LPD8806 *strip, MicroBit *uBit) {
    this->strip = strip;
    this->uBit = uBit;
}

void LightAnimation::colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<this->strip->numPixels(); i++) 
      this->strip->setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i < this->strip->numPixels(); i++) {
    this->strip->setPixelColor(i, c); // Set new pixel 'on'
    this->strip->show();              // Refresh LED states
    this->strip->setPixelColor(i, 0); // Erase pixel, but don't refresh!
    this->uBit->sleep(wait);
  }

  this->strip->show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights.
void LightAnimation::theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < this->strip->numPixels(); i=i+3) {
        this->strip->setPixelColor(i+q, c);    //turn every third pixel on
      }
      this->strip->show();
     
      this->uBit->sleep(wait);
     
      for (int i=0; i < this->strip->numPixels(); i=i+3) {
        this->strip->setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void LightAnimation::theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < this->strip->numPixels(); i=i+3) {
          this->strip->setPixelColor(i+q,this-> Wheel( (i+j) % 384));    //turn every third pixel on
        }
        this->strip->show();
       
        this->uBit->sleep(wait);
       
        for (int i=0; i < this->strip->numPixels(); i=i+3) {
          this->strip->setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Fill the dots progressively along the strip.
void LightAnimation::colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < this->strip->numPixels(); i++) {
      this->strip->setPixelColor(i, c);
      this->strip->show();
      this->uBit->sleep(wait);
  }
}

void LightAnimation::rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < this->strip->numPixels(); i++) {
      this->strip->setPixelColor(i, this->Wheel( (i + j) % 384));
    }  
    this->strip->show();   // write all the pixels out
    this->uBit->sleep(wait);
  }
}

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t LightAnimation::Wheel(uint16_t WheelPos)
{
  size_t r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(this->strip->Color(r,g,b));
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void LightAnimation::rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < this->strip->numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      this->strip->setPixelColor(i, this->Wheel( ((i * 384 / this->strip->numPixels()) + j) % 384) );
    }  
    this->strip->show();   // write all the pixels out
    this->uBit->sleep(wait);
  }
}