//include libraries for BNO055
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include <Adafruit_MPU6050.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

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
boolean forceCapTouched1 = true;  // prod: false
boolean forceCapTouched2 = true; // prod: false
boolean forceCapTouched3 = true; // prod: false

boolean isBnoActive1 = true;  // prod: true
boolean isBnoActive2 = true; // prod: true
boolean isBnoActive3 = true; // prod: true

boolean singleHandMode = false;

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
//
//Adafruit_MPU6050 mpu;
//Adafruit_Sensor *mpu_accel;

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;


//for Hardware Serial Communication:
//Leonardo Board One Pin0 is connected to Leonardo Board Two Pin 1
//Board One Pin1 is connected to Board Two Pin0
char inChar;
char inData[10];
int dataLength = 0;
String gyro3Readings;

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

uint8_t cap1countR = 0;
uint8_t cap1countL = 0;
uint8_t cap1countTotal = 0;
uint8_t cap2countR = 0;
uint8_t cap2countL = 0;
uint8_t cap2countTotal = 0;
uint8_t cap3countR = 0;
uint8_t cap3countL = 0;
uint8_t cap3countTotal = 0;

float cap1active = 0;
float cap2active = 0;
float cap3active = 0;
byte cap1target = 0;
byte cap2target = 0;
byte cap3target = 0;
byte checkpoint1 = 0;
byte checkpoint2 = 0;
byte checkpoint3 = 0;
const uint8_t threshold=1;

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 15 // Popular NeoPixel ring size

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

// Time to display feedback after player disconnects
unsigned long disconnectFeedbackDuration = 10000;
unsigned long timeOfLastDisconnectPlayer1 = disconnectFeedbackDuration;
unsigned long timeOfLastDisconnectPlayer2 = disconnectFeedbackDuration;
unsigned long timeOfLastDisconnectPlayer3 = disconnectFeedbackDuration;

// Time player needs to be touching all touchpoints to be considered fully activated
unsigned long preFullPreActivationDuration = 250;
unsigned long timeofLastPreActivationStartPlayer1R = preFullPreActivationDuration;
unsigned long timeofLastPreActivationStartPlayer2R = preFullPreActivationDuration;
unsigned long timeofLastPreActivationStartPlayer3R = preFullPreActivationDuration;

// Time player needs to be touching all touchpoints to be considered fully activated
unsigned long timeofLastPreActivationStartPlayer1L = preFullPreActivationDuration;
unsigned long timeofLastPreActivationStartPlayer2L = preFullPreActivationDuration;
unsigned long timeofLastPreActivationStartPlayer3L = preFullPreActivationDuration;


// Time player needs to be touching all touchpoints to be considered fully activated
unsigned long preFullActivationDuration = 1000;
unsigned long timeofLastActivationPlayer1 = 0;
unsigned long timeofLastActivationPlayer2 = 0;
unsigned long timeofLastActivationPlayer3 = 0;

// True if player was fully activated before disconnecting from touchpointd
boolean wasFullyActivatedBeforeDisconnectPlayer1 = false;
boolean wasFullyActivatedBeforeDisconnectPlayer2 = false;
boolean wasFullyActivatedBeforeDisconnectPlayer3 = false;

// True if player was reconnected while "disconnect" feedback was active
boolean wasActivatedDuringDisconnectPlayer1 = false;
boolean wasActivatedDuringDisconnectPlayer2 = false;
boolean wasActivatedDuringDisconnectPlayer3 = false;

// STATES: ["STANDBY", "PREACTIVE", "ACTIVE", "DISCONNECTED"]

enum PlayerState {STANDBY, PREACTIVE, ACTIVE, DISCONNECTED};

PlayerState playerState1 = STANDBY;
PlayerState playerState2 = STANDBY;
PlayerState playerState3 = STANDBY;

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  
  // start Serial to write to computer, Serial1 to listen to the sender board
  Serial.begin(9600);
  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
//  Serial1.begin(9600);
//  while (!Serial1) { // needed to keep leonardo/micro from starting too fast!
//    delay(10);
//  }
  Serial.println("starting up");


  //Initialize the gyroscopes
  initializeGyros();

  // initializing MPR121 capacitive sensors
  initializeMPRs();

  // initializing LEDs
  clearAllPlayerLEDs();
  
  
  Serial.println("Initialization Done");
}

String getPlayerStateAsString(PlayerState playerState) {
  switch(playerState) {
    case STANDBY:
      return "STANDBY";
    case PREACTIVE:
      return "PREACTIVE";
    case ACTIVE:
      return "ACTIVE";
    case DISCONNECTED:
      return "DISCONNECTED";
  }
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
  Serial.print(", ");
  Serial.print(playerState1);
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
  Serial.print(", ");
  Serial.print(playerState2);
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
  Serial.print(", ");
  Serial.print(playerState3);
  Serial.print(" ]");

  Serial.println(']');
}

boolean isWithinTimeOfLastDisconnectFeedback(unsigned long timeOfLastDisconnect) {
  unsigned long timeSinceLastDisconnect = millis() - timeOfLastDisconnect;
  return timeSinceLastDisconnect < disconnectFeedbackDuration;
}

boolean isFullyActivated(unsigned long timeOfLastActivation) {
  unsigned long timeSinceLastActivation = millis() - timeOfLastActivation;
  return timeSinceLastActivation > preFullActivationDuration;
}

boolean isFullyPreActivated(unsigned long timeOfLastPreActivationStart) {
  unsigned long timeSinceLastPreActivation = millis() - timeOfLastPreActivationStart;
  return timeSinceLastPreActivation > preFullPreActivationDuration;
}

boolean shouldPrintStateUpdate() {
   unsigned long timeSinceLastStateUpdate = millis() - timeOfLastStateUpdate;
   return timeSinceLastStateUpdate > timeToUpdateState;
}

boolean shouldDisplayDisconnectFeedbackPlayer1() {
  return shouldDisplayDisconnectFeedback(
      checkpoint1,
      wasFullyActivatedBeforeDisconnectPlayer1,
      timeOfLastDisconnectPlayer1
  );
}

boolean shouldDisplayDisconnectFeedbackPlayer2() {
  return shouldDisplayDisconnectFeedback(
      checkpoint2,
      wasFullyActivatedBeforeDisconnectPlayer2,
      timeOfLastDisconnectPlayer2
  );
}

boolean shouldDisplayDisconnectFeedbackPlayer3() {
  return shouldDisplayDisconnectFeedback(
      checkpoint3,
      wasFullyActivatedBeforeDisconnectPlayer3,
      timeOfLastDisconnectPlayer3
  );
}

boolean shouldDisplayDisconnectFeedback(
  bool checkpoint, 
  bool wasFullyActivatedBeforeDisconnect, 
  unsigned long timeOfLastDisconnect
) {
    return checkpoint != 1 && 
      wasFullyActivatedBeforeDisconnect &&
      isWithinTimeOfLastDisconnectFeedback(timeOfLastDisconnect);
}

void updateGyroState() {
  readGyro3Data();
  readGyro1Data();
  readGyro2Data();
}

void updateCapTouchState() {
    unsigned long currTime = millis();
  
    //reset our cap sensor values
    resetArray(cap1values);
    resetArray(cap2values);
    resetArray(cap3values);

    int preCap1CountR = cap1countR;
    int preCap2CountR = cap2countR;
    int preCap3CountR = cap3countR;
    
    int preCap1CountL = cap1countL;
    int preCap2CountL = cap2countL;
    int preCap3CountL = cap3countL;
    
    cap1countR = 0;
    cap1countL = 0;
    cap2countR = 0;
    cap2countL = 0;
    cap3countR = 0;
    cap3countL = 0;

    int  preCap1CountTotal = cap1countTotal;
    int  preCap2CountTotal = cap2countTotal;
    int  preCap3CountTotal = cap3countTotal;
    cap1countTotal = 0;
    cap2countTotal = 0;
    cap3countTotal = 0;
  
    //get all three MPR121 readings
    readMPRs();
    cap1active=checkActivation(cap1countR, cap1countL, cap1target, cap1active, threshold, singleHandMode);
    cap2active=checkActivation(cap2countR, cap2countL, cap2target, cap2active, threshold, singleHandMode);
    cap3active=checkActivation(cap3countR, cap3countL, cap3target, cap3active, threshold, singleHandMode);

    //check if stations are "activated" as defined by our threshold
    if(cap1active<0.2){
      if (checkpoint1 == 1) {
        // player 1 disconnected
        timeOfLastDisconnectPlayer1 = millis();
        wasFullyActivatedBeforeDisconnectPlayer1 = isFullyActivated(timeofLastActivationPlayer1);
      }
      
      checkpoint1=0;
    } else {
      
      if (checkpoint1 == 0) {
        // player 1 activated
        if (!shouldDisplayDisconnectFeedbackPlayer1()) {
          // only update time of activation if not currently in disconnect phase
          timeofLastActivationPlayer1 = millis();
        }
      }
      checkpoint1=1;
    }
    
    if(cap2active<0.2){
      if (checkpoint2 == 1) {
        // player 2 disconnected
        timeOfLastDisconnectPlayer2 = millis();
        wasFullyActivatedBeforeDisconnectPlayer2 = isFullyActivated(timeofLastActivationPlayer2);
      }
      checkpoint2=0;
    } else {
      if (checkpoint2 == 0) {
         // player 2 activated
         if (!shouldDisplayDisconnectFeedbackPlayer2()) {
            // only update time of activation if not currently in disconnect phase
            timeofLastActivationPlayer2 = millis();
         }
      }
      checkpoint2=1;
    }
    
    if(cap3active<0.2){
      if (checkpoint3 == 1) {
        // player 3 disconnected
        timeOfLastDisconnectPlayer3 = millis();
        wasFullyActivatedBeforeDisconnectPlayer3 = isFullyActivated(timeofLastActivationPlayer3);
      }
      checkpoint3=0;
    } else {
      if (checkpoint3 == 0) {
         // player 3 activated
        if (!shouldDisplayDisconnectFeedbackPlayer3()) {
          // only update time of activation if not currently in disconnect phase
          timeofLastActivationPlayer3 = millis();
        }
      }
      checkpoint3=1;
    }

    if (preCap1CountR == 0 && cap1countR > 0) {
      timeofLastPreActivationStartPlayer1R = millis();    
    }
    if (preCap2CountR == 0 && cap2countR > 0) {
      timeofLastPreActivationStartPlayer2R = millis();    
    }
    if (preCap3CountR == 0 && cap3countR > 0) {
      timeofLastPreActivationStartPlayer3R = millis();    
    }
    
    if (preCap1CountL == 0 && cap1countL > 0) {
      timeofLastPreActivationStartPlayer1L = millis();    
    }
    if (preCap2CountL == 0 && cap2countL > 0) {
      timeofLastPreActivationStartPlayer2L = millis();    
    }
    if (preCap3CountL == 0 && cap3countL > 0) {
      timeofLastPreActivationStartPlayer3L = millis();    
    }
}

void updatePlayerStates() {
  // PLAYER 1
  if (shouldDisplayDisconnectFeedbackPlayer1()) {
     playerState1 = DISCONNECTED;
  } else if (checkpoint1 == 1 && isFullyActivated(timeofLastActivationPlayer1)) {
     playerState1 = ACTIVE;
  } else if (cap1countTotal > 0 && isFullyPreActivated(timeofLastPreActivationStartPlayer1R) && isFullyPreActivated(timeofLastPreActivationStartPlayer1L)) {
     playerState1 = PREACTIVE;
  } else {
     playerState1 = STANDBY;
  }

  // PLAYER 2
  if (shouldDisplayDisconnectFeedbackPlayer2()) {
     playerState2 = DISCONNECTED;
  } else if (checkpoint2 == 1 && isFullyActivated(timeofLastActivationPlayer2)) {
     playerState2 = ACTIVE;
  } else if (cap2countTotal > 0 && isFullyPreActivated(timeofLastPreActivationStartPlayer2R) && isFullyPreActivated(timeofLastPreActivationStartPlayer2L)) {
     playerState2 = PREACTIVE;
  } else {
     playerState2 = STANDBY;
  }

  // PLAYER 3
  if (shouldDisplayDisconnectFeedbackPlayer3()) {
     playerState3 = DISCONNECTED;
  } else if (checkpoint3 == 1 && isFullyActivated(timeofLastActivationPlayer3)) {
     playerState3 = ACTIVE;
  } else if (cap3countTotal > 0 && isFullyPreActivated(timeofLastPreActivationStartPlayer3R) && isFullyPreActivated(timeofLastPreActivationStartPlayer3L)) {
     playerState3 = PREACTIVE;
  } else {
     playerState3 = STANDBY;
  }
}

void updateLEDs() {
  // Display DISCONNECTED lights if user was connected for a sufficient amount of time to be considered fully activated and then released a touch point
  // else Display CONNECTED lights if user is fully activated or has fingers on some touch points
  // else Display STANDBY lights

  if (playerState1 == DISCONNECTED) {
    setPlayer1LEDsToDisconnected(timeOfLastDisconnectPlayer1);
  } else if (playerState1 == ACTIVE) {
     setPlayer1LEDsToActive(cap1countTotal);
  } else if (playerState1 == PREACTIVE) {
    if (checkpoint1 == 1 && !isFullyActivated(timeofLastActivationPlayer1)) {
      setPlayer1LEDsToPreActiveBlink();
    } else {
      setPlayer1LEDsToPreActive(cap1countR, cap1countL);
    }
  } else {
    setPlayer1LEDsToStandby();
  }

  if (playerState2 == DISCONNECTED) {
    setPlayer2LEDsToDisconnected(timeOfLastDisconnectPlayer2);
  } else if (playerState2 == ACTIVE) {
    setPlayer2LEDsToActive(cap2countR);
  } else if (playerState2 == PREACTIVE) {
    if (checkpoint2 == 1 && !isFullyActivated(timeofLastActivationPlayer2)) {
      setPlayer2LEDsToPreActiveBlink();
    }
     setPlayer2LEDsToPreActive(cap2countR, cap2countL);
  } else {
    setPlayer2LEDsToStandby();
  }

   if (playerState3 == DISCONNECTED) {
    setPlayer3LEDsToDisconnected(timeOfLastDisconnectPlayer3);
  } else if (playerState3 == ACTIVE) {
    setPlayer3LEDsToActive(cap3countR);
  } else if (playerState3 == PREACTIVE) {
    if (checkpoint3 == 1 && !isFullyActivated(timeofLastActivationPlayer3)) {
      setPlayer3LEDsToPreActiveBlink();
    }
     setPlayer3LEDsToPreActive(cap3countR, cap3countL);
  } else {
    setPlayer3LEDsToStandby();
  }
}

void loop() {
//  Serial.println("looping");

  updateCapTouchState();
  
  if (shouldPrintStateUpdate()) {
    timeOfLastStateUpdate = millis();

    updateGyroState();
      
    printGameState();
  }

  updatePlayerStates();
  updateLEDs();
//
//  Serial.print(timeofLastPreActivationStartPlayer1);
//  Serial.print(", ");
//  Serial.print(timeofLastPreActivationStartPlayer2);
//  Serial.print(", ");
//  Serial.println(timeofLastPreActivationStartPlayer3);
  delay(100);
}
