// LED.cpp
#include "LED.h"

LED::LED(int _numpixels, int pin) {
//  ledPin   = pin;
//  ledState = LOW;
  numpixels = _numpixels;
  pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);

  pixels.begin();

//  setPixelsColor(color(0,0,0));
}

uint32_t LED::color(int r, int g, int b) {
  return pixels.Color(r, g, b);
}

void LED::setPixelsColor(uint32_t color) {  
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i = 0; i < numpixels; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, color);
  }

  pixels.show();   // Send the updated pixel colors to the hardware.
}

void LED::setStandbyLights() {
  int baseColor = 100;
  int blinkFactor = 50;
  int variation = sin(millis()/1000.0) * blinkFactor;

  int nextColor = baseColor + variation;

//  Serial.println(nextColor);
  
  setPixelsColor(color(nextColor,nextColor,nextColor));
}
