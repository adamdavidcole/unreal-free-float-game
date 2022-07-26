float GYRO_RANGE_MIN_Z = -30.0;
float GYRO_RANGE_MAX_Z = 30.0;
float GYRO_RANGE_MIN_Y = -15;
float GYRO_RANGE_MAX_Y = 30;

float MPU_GYRO_RANGE_MIN = -10000;
float MPU_GYRO_RANGE_MAX = 10000;

float SMOOTH_VALUE_FACTOR = 0.5;
float IGNORE_VALUES_RANGE = 0.20;

String inDataString;

const byte numChars = 100;
char myString[numChars];

const unsigned int MAX_MESSAGE_LENGTH = 256;


// UTILITIES
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapGyroY(float val) {
  if (val >= 0) {
    return mapfloat(val, 0, GYRO_RANGE_MAX_Y, 0, 1.0);
  }

  return mapfloat(val, GYRO_RANGE_MIN_Y, 0, -1.0, 0.0);
}

float mapGyroZ(float val) {
  return mapfloat(val, GYRO_RANGE_MIN_Z, GYRO_RANGE_MAX_Z, -1.0, 1.0);
}
float mapMpuGyro(float val) {
  val = constrain(val, MPU_GYRO_RANGE_MIN, MPU_GYRO_RANGE_MAX);
  return mapfloat(val, MPU_GYRO_RANGE_MIN, MPU_GYRO_RANGE_MAX, -1.0, 1.0);
}


float getSmoothedValue(float value, float target) {
  if (abs(target) < IGNORE_VALUES_RANGE) target = 0;
  float smoothed = value + (target - value) * SMOOTH_VALUE_FACTOR;
  
  return constrain(smoothed, -1.0, 1.0);
}
//gy1yTarget=event.orientation.y;
// gy1zTarget=event.orientation.z;
// gy1y=gy1y+(gy1Target-gy1y)*0.2;
// gy2y=gy2y+(gy2Target-gy2y)*0.2;

void initializeGyros(){
  Serial.println("initializeGyros start");

  if(isBnoActive1 && !bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no first BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  Serial.println("Bno1 Complete");
  
   if(isBnoActive2 && !bno2.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no second BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  Serial.println("MPU6050 (Gyro3) Start!");
//
  if (isBnoActive3) {
//    Serial.println("Failed to find MPU6050 chip");
//    while (1) {
//      delay(10);
//    }

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  }

  

//  Serial.println("MPU6050 (Gyro3) Found!");
//  mpu_accel = mpu.getAccelerometerSensor();
//  mpu_accel->printSensorDetails();
 

  if (isBnoActive1) bno.setExtCrystalUse(true);
  if (isBnoActive2) bno2.setExtCrystalUse(true);

 //initialize incoming data array for Gryo3 attached on Arduino board2
//  for (dataLength = 0; dataLength < 10; dataLength++) {
//    inData[dataLength]=',';
//  }

  
 
   Serial.println("initializeGyros complete: bnos found");

//   delay(3000);
}


void readGyro1Data() {
 if (!isBnoActive1) return;
 
 sensors_event_t event; 
 bno.getEvent(&event);

 float mappedEventY = mapGyroY(event.orientation.y);
 float mappedEventZ = mapGyroZ(event.orientation.z);

 gy1y = getSmoothedValue(gy1y, mappedEventY) * -1.0;
 gy1z = getSmoothedValue(gy1z, mappedEventZ);
}

void readGyro2Data(){
 if (!isBnoActive2) return;
  
 sensors_event_t event2; 
 bno2.getEvent(&event2);
 
 float mappedEventY = mapGyroY(event2.orientation.y);
 float mappedEventZ = mapGyroZ(event2.orientation.z);

 gy2y = getSmoothedValue(gy2y, mappedEventY) * -1.0;
 gy2z = getSmoothedValue(gy2z, mappedEventZ);
}

void readGyro3Data() {
  if (!isBnoActive3) return;

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

//    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
//    Serial.print("a/g:\t");
//    Serial.print(ax); Serial.print("\t");
//    Serial.print(ay); Serial.print("\t");
//    Serial.print(az); Serial.print("\t");
//  sensors_event_t accel;
//  mpu.getEvent(&accel);
//  mpu_accel = mpu.getAccelerometerSensor();
//  mpu_accel->printSensorDetails();
//  
//  mpu_accel->printSensorDetails();


//  sensors_event_t a, g, temp;
//  mpu.getEvent(&a, &g, &temp);
//  delay(500);

  /* Print out the values */
//  Serial.print("Acceleration X: ");
//  Serial.print(a.acceleration.x);
//
//  
  float mappedEventY = mapMpuGyro(ay);
  float mappedEventZ = mapMpuGyro(ax);
////
  gy3y = getSmoothedValue(gy3y, mappedEventY) * -1.0;
  gy3z = getSmoothedValue(gy3z, mappedEventZ);
//    gy3y = mappedEventY;
//    gy3z = mappedEventZ;






///// old 
//  Serial.print("\t\tAccel X: ");
//  Serial.print(accel.acceleration.x);
//  Serial.print(" \tY: ");
//  Serial.print(accel.acceleration.y);
//  Serial.print(" \tZ: ");
//  Serial.print(accel.acceleration.z);
//  boolean endOfLineReached = false;
//  char myChar;
//  
//  int currCharIndex = 0;
//  
//  String outputString = "";
//  while (Serial1.available() > 0) {
//     //Create a place to hold the incoming message
//     static char message[MAX_MESSAGE_LENGTH];
//     static unsigned int message_pos = 0;
//  
//     //Read the next available byte in the serial receive buffer
//     char inByte = Serial1.read();
//  
//     //Message coming in (check not terminating character) and guard for over message size
//     if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
//     {
//       //Add the incoming byte to our message
//       message[message_pos] = inByte;
//       message_pos++;
//     }
//     //Full message received...
//     else
//     {
//       //Add null character to string
//       message[message_pos] = '\0';
//
//       outputString = String(message);
//       //Print the message (or do other things)
//  
//       //Reset for the next message
//       message_pos = 0;
//     }
//   }
//
//   if (outputString.length() > 0) {
//     
//      String cleanDataString = outputString.substring(1, outputString.length() - 3);
//      if (cleanDataString.length() > 0) {
//          getGyro3Values(cleanDataString);
//      }
//    }
}


//void getGyro3Values(String cleanDataString){
//   int firstCommaIndex = cleanDataString.indexOf(',');
//   String firstValue = cleanDataString.substring(0,firstCommaIndex);
//   String secondValue = cleanDataString.substring(firstCommaIndex+1);
//
//   // NOTE: these are being sent backwards, but thats ok for now
//   float mappedEventZ = mapGyroZ(firstValue.toFloat());
//   float mappedEventY= mapGyroY(secondValue.toFloat());
//
//   gy3y = getSmoothedValue(gy3y, mappedEventY) * -1.0;
//   gy3z = getSmoothedValue(gy3z, mappedEventZ);
// }
