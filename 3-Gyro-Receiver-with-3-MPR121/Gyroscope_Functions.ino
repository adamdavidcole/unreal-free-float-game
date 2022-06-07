float GYRO_RANGE_MIN_Z = -30.0;
float GYRO_RANGE_MAX_Z = 30.0;
float GYRO_RANGE_MIN_Y = -15;
float GYRO_RANGE_MAX_Y = 30;

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

  if (isBnoActive1) bno.setExtCrystalUse(true);
  if (isBnoActive2) bno2.setExtCrystalUse(true);

 //initialize incoming data array for Gryo3 attached on Arduino board2
  for (dataLength = 0; dataLength < 10; dataLength++) {
    inData[dataLength]=',';
  }
 
   Serial.println("initializeGyros complete: bnos found");
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
  boolean endOfLineReached = false;
  char myChar;
  
  int currCharIndex = 0;
  
  String outputString = "";
//  Serial.println("readGyro3Data");

   while (Serial1.available() > 0) {
     //Create a place to hold the incoming message
     static char message[MAX_MESSAGE_LENGTH];
     static unsigned int message_pos = 0;
  
     //Read the next available byte in the serial receive buffer
     char inByte = Serial1.read();
  
     //Message coming in (check not terminating character) and guard for over message size
     if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
     {
       //Add the incoming byte to our message
       message[message_pos] = inByte;
       message_pos++;
     }
     //Full message received...
     else
     {
       //Add null character to string
       message[message_pos] = '\0';

       outputString = String(message);
       //Print the message (or do other things)
//       Serial.println(outputString);
  
       //Reset for the next message
       message_pos = 0;
     }
   }

   if (outputString.length() > 0) {
//      Serial.println("final string: ");
     
      String cleanDataString = outputString.substring(1, outputString.length() - 3);
      if (cleanDataString.length() > 0) {
//            Serial.print("Clean data string: ");
//            Serial.println(cleanDataString);
//            gyro3Readings = String(cleanDataString);
          getGyro3Values(cleanDataString);
      }
//
//      Serial.println(gyro3Readings);

      
//      getGyro3Values();
    }

//  while (Serial1.available() && !endOfLineReached) {     // If anything comes in Serial1 (pins 0 & 1)
//    myChar = Serial1.read();
////    Serial.write(myChar);   // read it and send it out Serial (USB)
//
//    Serial.println(myChar);
////    currCharIndex = currCharIndex + 1;
////    Serial.print("Curr Length: ");
////    Serial.println(currCharIndex);
//
//
//    if (myChar == '\n' ) {
//       Serial.println("NL");
//       endOfLineReached = true;
////       Serial.print("Length: ");
////       Serial.println(currCharIndex);
//    }



    
//    myString[currCharIndex] = myChar;
//    currCharIndex += 1;
////    Serial.print(currCharIndex);
//
//    if (myChar == '\n' ) {
//      Serial.println("NL"); 
//      endOfLineReached = true;
//      myString[currCharIndex] = '\0';
//
//      Serial.print("Length: ");
//      Serial.println(currCharIndex);
//      for (int i = 0; i < currCharIndex; i++) {
//        Serial.print("hello");
//        Serial.print(myString[currCharIndex]);
//      }
//      Serial.println("");
// 
//      
//    }
//  }

  
//  
//  String readInValues = "a";
//  String outputString = "";
//  boolean endOfLineReached = false;

//   if (Serial1.available() && !endOfLineReached) {
//        char recieved = Serial1.read();
//        inDataString += recieved; 
//       
//        Serial.write(recieved);

        // Process message when new line character is recieved
//        if (recieved == '\n')
//        {
//          endOfLineReached = true;
////            Serial.print("Arduino Received: ");
////            Serial.print(inDataString);
//            outputString = inDataString;
//            inDataString = ""; // Clear recieved buffer
//        }
//    }

//    String cleanDataString = outputString.substring(1, outputString.length() - 3);
//    if (cleanDataString.length()) {
//          gyro3Readings = cleanDataString;
//    }
//
//    if (outputString.length() > 0) {
//      Serial.print("final string: ");
//     
//      String cleanDataString = outputString.substring(1, outputString.length() - 3);
//      if (cleanDataString.length()) {
//            gyro3Readings = cleanDataString;
//      }
//
//       Serial.println(outputString);
//
//      
////      getGyro3Values();
//    }
}


void getGyro3Values(String cleanDataString){
   int firstCommaIndex = cleanDataString.indexOf(',');
   String firstValue = cleanDataString.substring(0,firstCommaIndex);
   String secondValue = cleanDataString.substring(firstCommaIndex+1);

   // NOTE: these are being sent backwards, but thats ok for now
   float mappedEventZ = mapGyroZ(firstValue.toFloat());
   float mappedEventY= mapGyroY(secondValue.toFloat());

   gy3y = getSmoothedValue(gy3y, mappedEventY) * -1.0;
   gy3z = getSmoothedValue(gy3z, mappedEventZ);
 }
