//include libraries for BNO055
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//include libraries for MPR121`
#include <Wire.h>
#include "Adafruit_MPR121.h"
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

#include "LED.h"

/* ******* ******** ******** 
 *  CONTROL VALUES FOR DEBUGGING
 ******** ******** ******** */
 
boolean isCapActive1 = true; // prod: true
boolean isCapActive2 = false; // prod: true
boolean isCapActive3 = false; // prod: true
boolean forceCapTouched1 = false;  // prod: false
boolean forceCapTouched2 = false; // prod: false
boolean forceCapTouched3 = false; // prod: false

boolean isBnoActive1 = false;  // prod: true
boolean isBnoActive2 = false; // prod: true
boolean isBnoActive3 = false; // prod: true

/* ******* ******** ******** 
 *  END CONTROL VALUES FOR DEBUGGING
 ******** ******** ******** */

unsigned long timeOfLastStateUpdate = millis();
unsigned long timeToUpdateState = 50; 

//We will have 3 MPR121 capacitive sensors attached, on
//different addresses, as explained in their initializer function
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();

//Two BNO gyroscopes communicating through I2C
//one sensor of them has ADR connected to 3 volts to change the default address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, 0x29);

//for Hardware Serial Communication:
//Leonardo Board One Pin0 is connected to Leonardo Board Two Pin 1
//Board One Pin1 is connected to Board Two Pin0
char inChar;
char inData[10];
int dataLength = 0;
String gyro3Readings = "0000,0000,";

//for gyroscope info
float gy1y=0;
float gy1z=0;
float gy2y=0;
float gy2z=0;
float gy3y=0;
float gy3z=0;

//for capacitive sensors
uint16_t currtouched1 = 0;
uint16_t currtouched2 = 0;
uint16_t currtouched3 = 0;
byte cap1values[12]= {0};
byte cap2values[12]= {0};
byte cap3values[12]= {0};
uint8_t cap1count = 0;
uint8_t cap2count = 0;
uint8_t cap3count = 0;
float cap1active = 0;
float cap2active = 0;
float cap3active = 0;
byte cap1target = 0;
byte cap2target = 0;
byte cap3target = 0;
byte checkpoint1 = 0;
byte checkpoint2 = 0;
byte checkpoint3 = 0;
const uint8_t threshold=2;

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 14 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
int LEDS_PLAYER_1_RIGHT = 6;
int LEDS_PLAYER_1_LEFT = 7;
int LEDS_PLAYER_2_RIGHT = 8;
int LEDS_PLAYER_2_LEFT = 9;
int LEDS_PLAYER_3_RIGHT = 10;
int LEDS_PLAYER_3_LEFT = 11 ;

LED LEDsPlayers1Right(NUMPIXELS, LEDS_PLAYER_1_RIGHT);
LED LEDsPlayers1Left( NUMPIXELS, LEDS_PLAYER_1_LEFT);
LED LEDsPlayers2Right(NUMPIXELS, LEDS_PLAYER_2_RIGHT);
LED LEDsPlayers2Left( NUMPIXELS, LEDS_PLAYER_2_LEFT);
LED LEDsPlayers3Right(NUMPIXELS, LEDS_PLAYER_3_RIGHT);
LED LEDsPlayers3Left( NUMPIXELS, LEDS_PLAYER_3_LEFT);

unsigned long disconnectFeedbackDuration = 3000;
unsigned long timeOfLastDisconnectPlayer1 = 0;
unsigned long timeOfLastDisconnectPlayer2 = 0;
unsigned long timeOfLastDisconnectPlayer3 = 0;

void setup() {
  // start Serial to write to computer, Serial1 to listen to the sender board
  Serial.begin(9600);
  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  Serial1.begin(9600);
  while (!Serial1) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  Serial.println("starting up");

  //Initialize the gyroscopes
  initializeGyros();

  // initializing MPR121 capacitive sensors
  initializeMPRs();

  // initializing LEDs
  clearAllPlayerLEDs();
  
  
  Serial.println("Initialization Done");
}

void printGameState() {
  Serial.print('[');

  // PlayerA
  bool playerAIsEngaged = checkpoint1 == 1;
  Serial.print("[ ");
  Serial.print(playerAIsEngaged);
  Serial.print(", ");
  Serial.print(gy1z);
  Serial.print(", ");
  Serial.print(gy1y);
  Serial.print(" ]");
  
  Serial.print(",");

  // PlayerB
  bool playerBIsEngaged = checkpoint2 == 1;
  Serial.print("[ ");
  Serial.print(playerBIsEngaged);
  Serial.print(", ");
  Serial.print(gy2z);
  Serial.print(", ");
  Serial.print(gy2y);
  Serial.print(" ]");
  Serial.print(",");

  // PlayerC
  bool playerCIsEngaged = checkpoint3 == 1;
  Serial.print("[ ");
  Serial.print(playerCIsEngaged);
  Serial.print(", ");
  Serial.print(gy3z);
  Serial.print(", ");
  Serial.print(gy3y);
  Serial.print(" ]");

  Serial.println(']');
}

boolean isWithinTimeOfLastDisconnectFeedback(unsigned long timeOfLastDisconnect) {
  unsigned long timeSinceLastDisconnect = millis() - timeOfLastDisconnect;
   return timeSinceLastDisconnect < disconnectFeedbackDuration;
}

boolean shouldPrintStateUpdate() {
   unsigned long timeSinceLastStateUpdate = millis() - timeOfLastStateUpdate;
   return timeSinceLastStateUpdate > timeToUpdateState;
}

void updateGyroState() {
  readGyro3Data();
  readGyro1Data();
  readGyro2Data();
}

void updateCapTouchState() {
    //reset our cap sensor values
    resetArray(cap1values);
    resetArray(cap2values);
    resetArray(cap3values);
    cap1count = 0;
    cap2count = 0;
    cap3count = 0;
  
    //get all three MPR121 readings
    readMPRs();
    cap1active=checkActivation(cap1count, cap1target, cap1active, threshold);
    cap2active=checkActivation(cap2count, cap2target, cap2active, threshold);
    cap3active=checkActivation(cap3count, cap3target, cap3active, threshold);
  
    //check if stations are "activated" as defined by our threshold
    if(cap1active<0.2){
      // player 1 disconnected
      if (checkpoint1 == 1) {
        timeOfLastDisconnectPlayer1 = millis();
      }
      
      checkpoint1=0;
    } else {
      checkpoint1=1;
    }
    
    if(cap2active<0.2){
      // player 2 disconnected
      if (checkpoint2 == 1) {
        timeOfLastDisconnectPlayer2 = millis();
      }
      checkpoint2=0;
    } else {
      checkpoint2=1;
    }
    
    if(cap3active<0.2){
      if (checkpoint3 == 1) {
        timeOfLastDisconnectPlayer3 = millis();
      }
      checkpoint3=0;
    } else {
      checkpoint3=1;
    }
}

void loop() {
//  Serial.println("looping");

  updateCapTouchState();

  if (shouldPrintStateUpdate()) {
    timeOfLastStateUpdate = millis();
    
    // get all three gyro data
    updateGyroState();
      
    printGameState();
  }

  if (checkpoint1 != 1 && isWithinTimeOfLastDisconnectFeedback(timeOfLastDisconnectPlayer1)) {
    setLEDsPlayer1(255, 0, 0);
  } else if (checkpoint1 == 1) {
    setLEDsPlayer1(0, 0, 255);
  } else {
    setPlayer1LEDsToStandby();
  }

  if (checkpoint2 != 1 && isWithinTimeOfLastDisconnectFeedback(timeOfLastDisconnectPlayer2)) {
    setLEDsPlayer2(255, 0, 0);
  } else if (checkpoint2 == 1) {
    setLEDsPlayer2(0, 0, 255);
  } else {
    setLEDsPlayer2(100, 100, 0);
  }

  delay(10);
}
