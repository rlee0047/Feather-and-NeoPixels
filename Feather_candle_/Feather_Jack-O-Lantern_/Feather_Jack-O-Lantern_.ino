/*
  NeoPixel Feather Wing Jack-O-Lantern  

  Created by Phillip Burgess 
  October 14, 2016 
  Orginal code found here
 
  https://learn.adafruit.com/circuit-playground-jack-o-lantern

  Edited by Ryan Lee
  October 27, 2016 
  Adapted to work with Feathers and the NeoPixel Wing
  
 */



#include<Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
 #endif 

#define PIN 6 //Which pin the pixels read from 

#define NUM_PIXS 32 // Number of Pixels 

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();// Start with all pixels off
}

uint8_t prev = 0;  // Start with all pixels off

void loop() {
  uint8_t lvl = random(64, 192); // End brightness at 128±64
  split(prev, lvl, 32); // Start subdividing, ±32 at midpoint
  prev = lvl; // Assign end brightness to next start
}

void split(uint8_t y1, uint8_t y2, uint8_t offset) {
  if(offset) { // Split further into sub-segments w/midpoint at ±offset
    uint8_t mid = (y1 + y2 + 1) / 2 + random(-offset, offset);
    split(y1 , mid, offset / 2); // First segment (offset is halved)
    split(mid, y2 , offset / 2); // Second segment (ditto)
  } else { // No further subdivision - y1 determines LED brightness
    uint32_t c = (((int)(pow((float)y1 / 255.0, 2.7) * 255.0 + 0.5) // Gamma 
    * 0x1004004) >> 8) & 0xFF3F03; // Expand to 32-bit RGB color
    for(uint8_t i=0; i<NUM_PIXS; i++) 
    strip.setPixelColor(i, c);
    strip.show();
    delay(4);
  }
}

