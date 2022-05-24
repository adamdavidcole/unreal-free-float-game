float GYRO_RANGE = 90.0;
float SMOOTH_VALUE_FACTOR = 0.2;

String inDataString;


// UTILITIES
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapGyro(float val) {
  return mapfloat(val, -GYRO_RANGE, GYRO_RANGE, -1.0, 1.0);
}

float getSmoothedValue(float value, float target) {
  return value + (target - value) * SMOOTH_VALUE_FACTOR;
}
//gy1yTarget=event.orientation.y;
// gy1zTarget=event.orientation.z;
// gy1y=gy1y+(gy1Target-gy1y)*0.2;
// gy2y=gy2y+(gy2Target-gy2y)*0.2;

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

 float mappedEventY = mapGyro(event.orientation.y);
 float mappedEventZ = mapGyro(event.orientation.z);

 gy1y = getSmoothedValue(gy1y, mappedEventY);
 gy1z = getSmoothedValue(gy1z, mappedEventZ);
}

void readGyro2Data(){
 sensors_event_t event2; 
 bno2.getEvent(&event2);
 
 float mappedEventY = mapGyro(event2.orientation.y);
 float mappedEventZ = mapGyro(event2.orientation.z);

 gy2y = getSmoothedValue(gy2y, mappedEventY);
 gy2z = getSmoothedValue(gy2z, mappedEventZ);
}

void readGyro3Data() {
  String readInValues = "a";
  String outputString = "";

   while (Serial1.available()) {
        char recieved = Serial1.read();
        inDataString += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
//            Serial.print("Arduino Received: ");
//            Serial.print(inDataString);
            outputString = inDataString;
            inDataString = ""; // Clear recieved buffer
        }
    }

    String cleanDataString = outputString.substring(1, outputString.length() - 3);
    gyro3Readings = cleanDataString;

    getGyro3Values();
}


void getGyro3Values(){
   int firstCommaIndex = gyro3Readings.indexOf(',');
   String firstValue = gyro3Readings.substring(0,firstCommaIndex);
   String secondValue = gyro3Readings.substring(firstCommaIndex+1);
    
   float mappedEventY = mapGyro(firstValue.toFloat());
   float mappedEventZ = mapGyro(secondValue.toFloat());

   gy3y = getSmoothedValue(gy3y, mappedEventY);
   gy3z = getSmoothedValue(gy3z, mappedEventZ);
 }
