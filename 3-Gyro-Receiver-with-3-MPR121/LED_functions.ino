// Set arbitrary color
void setLEDsPlayer1(int r, int g, int b) {
  LEDsPlayers1Right.setPixelsColor(LEDsPlayers1Right.color(r, g, b));
  LEDsPlayers1Left.setPixelsColor(LEDsPlayers1Left.color(r, g, b));
}

void setLEDsPlayer2(int r, int g, int b) {
  LEDsPlayers2Right.setPixelsColor(LEDsPlayers2Right.color(r, g, b));
  LEDsPlayers2Left.setPixelsColor(LEDsPlayers2Left.color(r, g, b));
}

void setLEDsPlayer3(int r, int g, int b) {
  LEDsPlayers3Right.setPixelsColor(LEDsPlayers3Right.color(r, g, b));
  LEDsPlayers3Left.setPixelsColor(LEDsPlayers3Left.color(r, g, b));
}



// Set to PREACTIVE
void setPlayer1LEDsToPreActive(int touchPointsRight, int touchPointsLeft) {
  LEDsPlayers1Right.setPreActive(touchPointsRight);
  LEDsPlayers1Left.setPreActive(touchPointsLeft);
}
void setPlayer1LEDsToPreActiveBlink() {
  LEDsPlayers1Right.setPreActiveBlink();
  LEDsPlayers1Left.setPreActiveBlink();
}

void setPlayer2LEDsToPreActive(int touchPointsRight, int touchPointsLeft) {
  LEDsPlayers2Right.setPreActive(touchPointsRight);
  LEDsPlayers2Left.setPreActive(touchPointsLeft);
}
void setPlayer2LEDsToPreActiveBlink() {
  LEDsPlayers2Right.setPreActiveBlink();
  LEDsPlayers2Left.setPreActiveBlink();
}

void setPlayer3LEDsToPreActive(int touchPointsRight, int touchPointsLeft) {
  LEDsPlayers3Right.setPreActive(touchPointsRight);
  LEDsPlayers3Left.setPreActive(touchPointsLeft);
}

// Set to ACTIVE pulse
void setPlayer1LEDsToActive(int touchPointsCount) {
  LEDsPlayers1Right.setActive(touchPointsCount);
  LEDsPlayers1Left.setActive(touchPointsCount);
}

// Set to active pulse
void setPlayer2LEDsToActive(int touchPointsCount) {
  LEDsPlayers2Right.setActive(touchPointsCount);
  LEDsPlayers2Left.setActive(touchPointsCount);
}

// Set to preactive pulse
void setPlayer3LEDsToActive(int touchPointsCount) {
  LEDsPlayers3Right.setActive(touchPointsCount);
  LEDsPlayers3Left.setActive(touchPointsCount);
}
void setPlayer3LEDsToPreActiveBlink() {
  LEDsPlayers3Right.setPreActiveBlink();
  LEDsPlayers3Left.setPreActiveBlink();
}

// Set to Disconnected pulse
void setPlayer1LEDsToDisconnected(unsigned long timeOfLastDisconnect) {
  LEDsPlayers1Right.setDisconnected(timeOfLastDisconnect);
  LEDsPlayers1Left.setDisconnected(timeOfLastDisconnect);
}

// Set to Disconnected pulse
void setPlayer2LEDsToDisconnected(unsigned long timeOfLastDisconnect) {
  LEDsPlayers2Right.setDisconnected(timeOfLastDisconnect);
  LEDsPlayers2Left.setDisconnected(timeOfLastDisconnect);
}

// Set to Disconnected pulse
void setPlayer3LEDsToDisconnected(unsigned long timeOfLastDisconnect) {
  LEDsPlayers3Right.setDisconnected(timeOfLastDisconnect);
  LEDsPlayers3Left.setDisconnected(timeOfLastDisconnect);
}


// Set to STANDBY pulse
void setPlayer1LEDsToStandby() {
  LEDsPlayers1Right.setStandbyLights();
  LEDsPlayers1Left.setStandbyLights();
}

void setPlayer2LEDsToStandby() {
  LEDsPlayers2Right.setStandbyLights();
  LEDsPlayers2Left.setStandbyLights();
}

void setPlayer3LEDsToStandby() {
  LEDsPlayers3Right.setStandbyLights();
  LEDsPlayers3Left.setStandbyLights();
}

void clearAllPlayerLEDs() {
  setLEDsPlayer1(0, 0, 0);
  setLEDsPlayer2(0, 0, 0);
  setLEDsPlayer3(0, 0, 0);
}
