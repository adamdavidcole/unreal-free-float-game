#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int timeNow;
int timePost=0;
float gy1y=0;
float gy1z=0;


void setup() {
  // put your setup code here, to run once:
//  mainBoard.begin(9600);
  //Serial.begin(9600);
  
  Serial1.begin(9600);


   if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    
    while(1);
  }
  
  bno.setExtCrystalUse(true);


}

void loop() {

 sensors_event_t event; 
 bno.getEvent(&event);
 gy1y=event.orientation.y;
 gy1z=event.orientation.z;

 Serial1.print("b");
 Serial1.print(gy1z);
 Serial1.print(gy1y);
 Serial1.println("n");
 delay(50);
// Serial1.println(0);
// Serial1.println(int(gy1z));
 ///Serial.print(gy1y);


}
