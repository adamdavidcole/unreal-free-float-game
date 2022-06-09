uint16_t DEFAULT_CAP_ON_VAL = 111111111111;
uint16_t DEFAULT_CAP_OFF_VAL = 0;


void initializeMPRs(){
  Serial.println("Begin initializeMPRs");
  
  if (isCapActive1 && !cap1.begin(0x5A)) {
    Serial.println("MPR121 cap1 not found, check wiring?");
    while (1);
  }
  if (isCapActive2 && !cap2.begin(0x5C)) {
    Serial.println("MPR121 cap2 not found, check wiring?");
    while (1);
  }
  if (isCapActive3 && !cap3.begin(0x5D)) {
    Serial.println("MPR121 cap3 not found, check wiring?");
    while (1);
  }

  int touchThreshold = 8;
  int releaseThreshold = 6;
  if (isCapActive1) cap1.setThresholds(touchThreshold,releaseThreshold);
  if (isCapActive2) cap2.setThresholds(touchThreshold,releaseThreshold);
  if (isCapActive3) cap3.setThresholds(touchThreshold,releaseThreshold);
  

  
  Serial.println("MPR121s found!");
}

void resetArray(byte arrayinput[12]){
  for (uint8_t i=0; i<12; i++){
    arrayinput[i]=0;
    }
 }


uint16_t getCapTouched1() {
  if (!isCapActive1) return forceCapTouched1 ? DEFAULT_CAP_ON_VAL : DEFAULT_CAP_OFF_VAL;
  
  return cap1.touched();
}

uint16_t getCapTouched2() {
  if (!isCapActive2) return forceCapTouched2 ? DEFAULT_CAP_ON_VAL : DEFAULT_CAP_OFF_VAL;
  
  return cap2.touched();
  
}

uint16_t getCapTouched3() {
  if (!isCapActive3) return forceCapTouched3 ? DEFAULT_CAP_ON_VAL : DEFAULT_CAP_OFF_VAL;
  
  return cap3.touched();
}

void readMPRs(){
  // getting our MPR121 sensor readings, these return a
  //number between 0 and 2^12
  currtouched1 = getCapTouched1();
  delay(3);
  currtouched2 = getCapTouched2();
  delay(3);
  currtouched3 = getCapTouched3();
  delay(3);

   //store our readings as a binary array to make it easy to
 //see which touchpoint is on/off
 int pinTotal = 12;
 for(uint8_t i=0; i<pinTotal/2; i++){
    cap1values[i]=(int(currtouched1/(pow(2,(i)))))%2;
    cap2values[i]=(int(currtouched2/(pow(2,(i)))))%2;
    cap3values[i]=(int(currtouched3/(pow(2,(i)))))%2;
    if(cap1values[i]==1){
      cap1countR++;
    }
    if(cap2values[i]==1){
      cap2countR++;
    }
    if(cap3values[i]==1){
      cap3countR++;
    }
  }

  for(uint8_t i=pinTotal/2; i<12; i++) {
    cap1values[i]=(int(currtouched1/(pow(2,(i)))))%2;
    cap2values[i]=(int(currtouched2/(pow(2,(i)))))%2;
    cap3values[i]=(int(currtouched3/(pow(2,(i)))))%2;
    if(cap1values[i]==1){
     cap1countL++;
    }
    if(cap2values[i]==1){
      cap2countL++;
    }
    if(cap3values[i]==1){
      cap3countL++;
    }
  }

  cap1countTotal = cap1countR + cap1countL;
  cap2countTotal = cap2countR + cap2countL;
  cap3countTotal = cap3countR + cap3countL;
 }

 
float checkActivation(uint8_t counterR, uint8_t counterL, byte target, float tracker, int threshold, boolean singleHandMode) {
  boolean isActive = singleHandMode ? 
    (counterR > 0 || counterL > 0) : 
    (counterR > 0 && counterL > 0);
  
  if (isActive){
    target=1;
  } else {
    target=0;
  }
  
  tracker = tracker + (target - tracker) * 0.2;
  
  return tracker;
}
