// LED.cpp
#include "LED.h"

LED::LED(int _numpixels, int pin) {
//  ledPin   = pin;
//  ledState = LOW;
  numpixels = _numpixels;
  pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);

  pixels.begin();

//  setPixelsColor(color(0,0,0));
//  pixels.clear();
//  pixels.show();
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

void LED::setActive(int touchPointsCount) {
  touchPointsCount = constrain(touchPointsCount, 0, 4);
  float touchPointsFactor = float(touchPointsCount)/4.0;
  
  int baseBlueColor = 50 + 155 * touchPointsFactor;
  int blinkFactor = 0;
  if (touchPointsCount >= 4) {
    baseBlueColor = 180;
    blinkFactor = 75;
  }
  int variation = sin(millis()/500.0) * blinkFactor;

  int nextBlueColor = baseBlueColor + variation;

//  Serial.println(nextColor);
  
  setPixelsColor(color(0,0,nextBlueColor));
}

void LED::setDisconnected(unsigned long timeOfLastDisconnect) {
  float timeSinceLastDisconnect = millis() - timeOfLastDisconnect;
  float timeSinceLastDisconnectFactor = (timeSinceLastDisconnect)/1000.0;
  float timeFactor = (timeSinceLastDisconnect)/1000.0;

//  if (timeSinceLastDisconnect > 8000) {
//    timeSinceLastDisconnectFactor *= 2.0;
//  }

  int baseRedColor = 150;
  int blinkFactor = 100;
  int variation = sin(timeFactor * timeSinceLastDisconnectFactor) * blinkFactor;

  int nextRedColor = baseRedColor + variation;

//  Serial.println(nextColor);
  
  setPixelsColor(color(nextRedColor,0,0));
}
