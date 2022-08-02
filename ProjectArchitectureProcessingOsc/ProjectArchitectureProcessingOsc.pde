/**
 * oscP5message by andreas schlegel
 * example shows how to create osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
//serial code added and adapted from https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all
 
 
import oscP5.*;
import netP5.*;
import processing.serial.*;


OscP5 oscP5;
NetAddress myRemoteLocation;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

boolean handshake;

void setup() 
{
  handshake = false;
   
  size(400,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 12000);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
   //127.0.0.1 is a loopback address 
  myRemoteLocation = new NetAddress("127.0.0.1", 8000);
  
  
  
  // Open whatever port is the one you're using - check in Arduino IDE
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port - for me only 2 were available even though arduino on port 4
  myPort = new Serial(this, portName, 9600); //match baud rate set in Arduino
}


void draw() 
{
  background(0);  
  
  if (myPort.available() > 0 && !handshake) {
    val = myPort.readStringUntil('\n');  
   
    if (val != null) {
      println(val);
      if (val.contains("Initialization Done")) {
        println("SETUP COMPLETE");
        handshake = true;
      }
    }
  }
  
  if ( myPort.available() > 0 && handshake) {  
    // If data is available, and setup is c
    val = myPort.readStringUntil('\n');  
   
    if (val != null)
    {    
      println(val); //print it out in the console// read it and store it in val
      
      JSONArray jsonArr = parseJSONArray(val);
      if (jsonArr == null) {
        println("JSONObject could not be parsed");
      } else {         
        JSONArray playerA = jsonArr.getJSONArray(0);
        JSONArray playerB = jsonArr.getJSONArray(1);
        JSONArray playerC = jsonArr.getJSONArray(2);
        
        boolean playerAIsEnaged = playerA.getInt(0) == 1;
        float playerAInput0 =  playerA.getFloat(1);
        float playerAInput1 =  playerA.getFloat(2);
        int playerAState = playerA.getInt(3);
        
        boolean playerBIsEnaged = playerB.getInt(0) == 1;
        float playerBInput0 =  playerB.getFloat(1);
        float playerBInput1 =  playerB.getFloat(2);
        int playerBState = playerB.getInt(3);
        
        boolean playerCIsEnaged = playerC.getInt(0) == 1;
        float playerCInput0 =  playerC.getFloat(1);
        float playerCInput1 =  playerC.getFloat(2);
        int playerCState = playerC.getInt(3);
        
        OscMessage myMessage = new OscMessage("/test/");
      
        myMessage.add(playerAIsEnaged);
        myMessage.add(playerAInput0);
        myMessage.add(playerAInput1);
        
        myMessage.add(playerBIsEnaged);
        myMessage.add(playerBInput0);
        myMessage.add(playerBInput1);
        
        myMessage.add(playerCIsEnaged);
        myMessage.add(playerCInput0);
        myMessage.add(playerCInput1);
                
        myMessage.add(playerAState); 
        myMessage.add(playerBState); 
        myMessage.add(playerCState);
                
        oscP5.send(myMessage, myRemoteLocation); 
      }
    }
  
  } 
  
  
}

void mousePressed() 
{  
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/test/");
  
  //myMessage.add(123); /* add an int to the osc message */
    myMessage.add(12.34); /* add a float to the osc message */
  //myMessage.add("some text"); /* add a string to the osc message */
  //myMessage.add(new byte[] {0x00, 0x01, 0x10, 0x20}); /* add a byte blob to the osc message */
  //myMessage.add(new int[] {1,2,3,4}); /* add an int array to the osc message */

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) 
{
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  //println(" typetag: "+theOscMessage.typetag());
  
 //getting data from OSC message
 
 /* parse theOscMessage and extract the values from the osc message arguments. */
 // int firstValue = theOscMessage.get(0).intValue();  // get the first osc argument
  //float secondValue = theOscMessage.get(1).floatValue(); // get the second osc argument
  String stringValue = theOscMessage.get(0).stringValue(); 
  //String stringValue1 = theOscMessage.get(2).stringValue(); // get the third osc argument
  println(stringValue);
  //println(stringValue1);
  
  //writing to serial for arduino to read
  myPort.write(stringValue);
}
