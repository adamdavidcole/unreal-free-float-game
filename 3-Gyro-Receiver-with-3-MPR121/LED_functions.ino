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

void clearAllPlayerLEDs() {
  setLEDsPlayer1(0, 0, 0);
  setLEDsPlayer2(0, 0, 0);
  setLEDsPlayer3(0, 0, 0);
}
