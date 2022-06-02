// LED.h
#ifndef LED_functions
#define LED_functions

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


class LED {
  private:
    Adafruit_NeoPixel pixels;
    int numpixels;

  public:
    LED(int numpixels, int pin);
    uint32_t color(int r, int g, int b);
    void setPixelsColor(uint32_t color);
    void setStandbyLights();
    void setPreActive(int touchPoints);
    void setPreActiveBlink();
    void setActive(int touchPointsCount);
    void setDisconnected(unsigned long timeOfLastDisconnect);
};

#endif
