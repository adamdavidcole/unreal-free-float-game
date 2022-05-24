void initializeMPRs(){
  if (!cap1.begin(0x5A)) {
    Serial.println("MPR121 cap1 not found, check wiring?");
    while (1);
  }
  if (!cap2.begin(0x5C)) {
    Serial.println("MPR121 cap2 not found, check wiring?");
    while (1);
  }
  if (!cap3.begin(0x5D)) {
    Serial.println("MPR121 cap3 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121s found!");
}

void resetArray(byte arrayinput[12]){
  for (uint8_t i=0; i<12; i++){
    arrayinput[i]=0;
    }
 }

void readMPRs(){
  // getting our MPR121 sensor readings, these return a
  //number between 0 and 2^12
  currtouched1 = cap1.touched();
  delay(3);
  currtouched2 = cap2.touched();
  delay(3);
  currtouched3 = cap3.touched();
  delay(3);

   //store our readings as a binary array to make it easy to
 //see which touchpoint is on/off
 for(uint8_t i=0; i<12; i++){
    cap1values[i]=(int(currtouched1/(pow(2,(i)))))%2;
    cap2values[i]=(int(currtouched2/(pow(2,(i)))))%2;
    cap3values[i]=(int(currtouched3/(pow(2,(i)))))%2;
    if(i<11){
      if(cap1values[i]==1){
        cap1count++;
        }
        if(cap2values[i]==1){
        cap2count++;
        }
        if(cap3values[i]==1){
        cap3count++;
        }
      }
  }
 }

 
float checkActivation(uint8_t counter,byte target,float tracker, int threshold){
  if(counter>threshold){
    target=1;
    }
  else{target=0;}
  tracker=tracker+(target-tracker)*0.2;
  return tracker;
  }
