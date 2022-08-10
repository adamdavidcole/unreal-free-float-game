// LED.cpp
#include "LED.h"

int STANDBY_BASE_INTENSITY = 25;
int STANDBY_BASE_BLINK_INTENSITY = 10;

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
  int baseColor = STANDBY_BASE_INTENSITY;
  int blinkFactor = STANDBY_BASE_BLINK_INTENSITY;
  int variation = sin(millis()/1000.0) * blinkFactor;

  int nextColor = baseColor + variation;

//  Serial.println(nextColor);
  
  setPixelsColor(color(nextColor,nextColor,nextColor));
}

void LED::setPreActive(int touchPoints) {  
  int baseColor = touchPoints > 0 ? 200 : STANDBY_BASE_INTENSITY;
  int blinkFactor = touchPoints > 0 ? 55 : STANDBY_BASE_BLINK_INTENSITY;
  int variation = sin(millis()/1000.0) * blinkFactor;

  int nextColor = baseColor + variation;

  setPixelsColor(color(nextColor,nextColor,nextColor));
}

void LED::setPreActiveBlink() {  
  int baseColor = 200;
  int blinkFactor = 55;
  int variation = sin(millis()/75.0) * blinkFactor;

  int nextColor = baseColor + variation;  
  setPixelsColor(color(nextColor,nextColor,nextColor));
}

void LED::setActive(int r, int g, int b) {
  int baseBlueColor = 180;
  int blinkFactor = 75;

  int variation = sin(millis()/500.0) * blinkFactor;

  int nextRedColor = r + variation;
  int nextGreenColor = g + variation;
  int nextBlueColor = b + variation;
  
  setPixelsColor(color(r,g,b));
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
