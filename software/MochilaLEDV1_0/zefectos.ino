
int RainbowCount = 0;

#define COOLING 40    //55
#define SPARKING 120  //120
bool gReverseDirection = true;



////////////////////////////////////ANIMACIONES///////////////
void marioAnimation() {
  drawImage(img01);
  delay(100);
  drawImage(img02);
  delay(100);
}

void personXAnimation() {
  drawImage(img03);
  delay(100);
  drawImage(img02);
  delay(100);
}


/////////////////////////////EFECTOS///////////////////////////
byte hue;

////Cambio de color de la matriz entera
void colorsEffect() {
  hue += 4;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
}

//rAINBOW
void rainbowEffect(int SpeedDelay) {
  byte *c;
  uint16_t i;

  // 5 cycles of all colors on wheel: 256*5
  for (i = 0; i < NUM_LEDS; i++) {
    c = Wheel(((i * 256 / NUM_LEDS) + RainbowCount) & 255);
    leds[i].setRGB(*c, *(c + 1), *(c + 2));
  }
  if (RainbowCount == 255) {
    RainbowCount = 0;
  }
  else {
    RainbowCount += 1;
  }
  FastLED.show();
  delay(SpeedDelay);
}


byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void cloudEffect() {
  static uint8_t colorIndex = 0;
  colorIndex = colorIndex + 1;
  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( CloudColors_p, colorIndex, globalBrightness, LINEARBLEND);
    colorIndex += 2;
  }
}

void partyEffect() {
  static uint8_t colorIndex = 0;
  colorIndex = colorIndex + 1;
  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( PartyColors_p, colorIndex, globalBrightness, LINEARBLEND);
    colorIndex += 2;
  }
}

void oceanEffect() {
  static uint8_t colorIndex = 0;
  colorIndex = colorIndex + 1;
  for ( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( OceanColors_p, colorIndex, globalBrightness, LINEARBLEND);
    colorIndex += 2;
  }
}

void matrixEffect() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = 14;
    FastLED.clear();
  }
  for (byte x = 0; x < mw; x++) {
    uint32_t thisColor = getPixColorXY(x, mh - 1);
    if (thisColor == 0)
      drawPixelXY(x, 0, 0x00FF00 * (random(0, 25) == 0));
    else if (thisColor < 0x002000)
      drawPixelXY(x, 0, 0);
    else
      drawPixelXY(x, 0, thisColor - 0x002000);
  }
  for (byte x = 0; x < mw; x++) {
    for (byte y = mh-1; y > 0; y--) {
      drawPixelXY(x, y, getPixColorXY(x, y - 1));
    }
  }
  delay(100);
}
