float GYRO_RANGE = 90.0;

// UTILITIES
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapGyro(float val) {
  return mapfloat(val, -GYRO_RANGE, GYRO_RANGE, -1.0, 1.0);
}

void initializeGyros(){
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no first BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   if(!bno2.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no second BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);
  bno2.setExtCrystalUse(true);

 //initialize incoming data array for Gryo3 attached on Arduino board2
  for (dataLength = 0; dataLength < 10; dataLength++) {
    inData[dataLength]=',';
    }
   Serial.println("bnos found");
  }


void readGyro1Data() {
 sensors_event_t event; 
 bno.getEvent(&event);
 gy1y=mapGyro(event.orientation.y);
 gy1z=mapGyro(event.orientation.z);
  }

void readGyro2Data(){
   sensors_event_t event2; 
 bno2.getEvent(&event2);
 gy2y=mapGyro(event2.orientation.y);
 gy2z=mapGyro(event2.orientation.z);
  }

void readGyro3Data() {
  if (Serial1.available()) {
      //char inByte = Serial1.read();
//    Serial.write(inByte);
     if (Serial1.read() == 'b') {
       for (dataLength = 0; dataLength < 10; dataLength++) {
        delay(2);
        inData[dataLength] = Serial1.read();
         if (inData[dataLength] == 'n') {
           inData[dataLength] = ',';
           break;
         }
        }
      }
    }

  gyro3Readings="";
  for (int i = 0; i < 10; i++) {
    gyro3Readings = gyro3Readings + inData[i];
  }
  
  //Serial.println(gyro3Readings);
    getGyro3Values();

}


void getGyro3Values(){
   int firstCommaIndex = gyro3Readings.indexOf(',');
   int secondCommaIndex = gyro3Readings.indexOf(',', firstCommaIndex+1);

   String firstValue = gyro3Readings.substring(0,firstCommaIndex);
   String secondValue = gyro3Readings.substring(firstCommaIndex+1,secondCommaIndex);

   gy3z= mapGyro(firstValue.toInt());
   gy3y= mapGyro(secondValue.toInt());
 }
