/*
 * Port from  https://github.com/adafruit/LPD8806
 */

/* 
 * File:   LPD8806.cpp
 * Author: taske
 * 
 * Created on 27. Oktober 2016, 23:06
 */

#include "LPD8806.h"


LPD8806::LPD8806(uint16_t n, PinName dpin, PinName cpin){
    this->pixels = NULL;
    this->begun  = false;
    this->updateLength(n);
    this->updatePins(dpin, cpin);
}

LPD8806::LPD8806(uint16_t n) {
    this->pixels = NULL;
    this->begun  = false;
    this->updateLength(n);
    this->updatePins();
}

LPD8806::LPD8806() {
    this->numLEDs = this->numBytes = 0;
    this->pixels  = NULL;
    this->begun   = false;
    this->updatePins(); // Must assume hardware SPI until pins are set
}

void LPD8806::begin(){

    
    this->startSPI();
    
    // Do we need this?
    for( int i = 0; i < (this->numLEDs * 3 * 2) ; ++i){
        this->spi_out(0);
    }

    this->begun = true;
}

// Change pin assignments post-constructor, switching to hardware SPI:
void LPD8806::updatePins(void) {
    this->datapin = MICROBIT_PIN_P1;
    this->clkpin = MICROBIT_PIN_P0;
    
    if(this->begun == true) this->startSPI();
    // Otherwise, SPI is NOT initted until begin() is explicitly called.
}

// Change pin assignments post-constructor, using arbitrary pins:
void LPD8806::updatePins(PinName dpin, PinName cpin) {
  if(this->begun == true) { // If begin() was previously invoked...
    // If previously using hardware SPI, turn that off:
      //SPI.end();
  }
  this->datapin     = dpin;
  this->clkpin      = cpin;

  // If previously begun, enable 'soft' SPI outputs now
  // TODO check this if(this->begun == true) startBitbang();

}

// Enable SPI hardware and set up protocol details:
void LPD8806::startSPI(void) {
    this->spi = new SPI(this->datapin, MICROBIT_PIN_P14, this->clkpin); // mosi, miso, sclk
    this->spi->format(8,0);
    this->spi->frequency(10000);

    // Issue initial latch/reset to strip:
    for(uint16_t i=((this->numLEDs+31)/32); i>0; i--) this->spi_out(0);
}

// Change strip length (see notes with empty constructor, above):
void LPD8806::updateLength(uint16_t n) {
    uint8_t  latchBytes;
    uint16_t dataBytes, totalBytes;

    this->numLEDs = this->numBytes = 0;
    if(this->pixels) free(pixels); // Free existing data (if any)

    dataBytes  = n * 3;
    latchBytes = (n + 31) / 32;
    totalBytes = dataBytes + latchBytes;
    if((this->pixels = (uint8_t *)malloc(totalBytes))) { // Alloc new data
        this->numLEDs  = n;
        this->numBytes = totalBytes;
        memset( this->pixels           , 0x80, dataBytes);  // Init to RGB 'off' state
        memset(&this->pixels[dataBytes], 0   , latchBytes); // Clear latch bytes
    }
    // 'begun' state does not change -- pins retain prior modes
}

uint16_t LPD8806::numPixels(void) {
  return this->numLEDs;
}

void LPD8806::show(void) {
  uint8_t  *ptr = this->pixels;
  uint16_t i    = this->numBytes;

  while(i--) this->spi_out(*ptr++);
 
}

// Convert separate R,G,B into combined 32-bit GRB color:
uint32_t LPD8806::Color(size_t r, size_t g, size_t b) {
    return ((uint32_t)(g | 0x80) << 16) |
           ((uint32_t)(r | 0x80) <<  8) |
                       b | 0x80 ;
}

// Set pixel color from separate 7-bit R, G, B components:
void LPD8806::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    if(n < this->numLEDs) { // Arrays are 0-indexed, thus NOT '<='
        uint8_t *p = &this->pixels[n * 3];
        *p++ = g | 0x80; // Strip color order is GRB,
        *p++ = r | 0x80; // not the more common RGB,
        *p++ = b | 0x80; // so the order here is intentional; don't "fix"
    }
}

// Set pixel color from 'packed' 32-bit GRB (not RGB) value:
void LPD8806::setPixelColor(uint16_t n, uint32_t c) {
  if(n < this->numLEDs) { // Arrays are 0-indexed, thus NOT '<='
    uint8_t *p = &this->pixels[n * 3];
    *p++ = (c >> 16) | 0x80;
    *p++ = (c >>  8) | 0x80;
    *p++ =  c        | 0x80;
  }
}

// Query color from previously-set pixel (returns packed 32-bit GRB value)
uint32_t LPD8806::getPixelColor(uint16_t n) {
    if(n < this->numLEDs) {
        uint16_t ofs = n * 3;
        return ((uint32_t)(this->pixels[ofs    ] & 0x7f) << 16) |
               ((uint32_t)(this->pixels[ofs + 1] & 0x7f) <<  8) |
                (uint32_t)(this->pixels[ofs + 2] & 0x7f);
    }

    return 0; // Pixel # is out of bounds
}


void LPD8806::spi_out(int value){
    this->spi->write(value);
}

void LPD8806::writeZeroes(){
    for( int i = 0; i < 3 * this->numLEDs; ++i){
        spi->write(0);
    }
}

void LPD8806::draw(u_int color){
    for( int i = 0; i < this->numLEDs * 3 ; ++i){
        spi->write(0x80| color);
    }
    
    this->writeZeroes();
}

