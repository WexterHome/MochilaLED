
void  drawImage(const uint32_t image[mh][mw]) {
  if (CONNECTION_TYPE == 0) {
    int i = 0;
    for (int x = 0; x < mh; x++) {
      for (int y = 0; y < mw; y++) {
        if (x % 2 == 1) {
          leds[i] = pgm_read_dword(&(image[x][y]));
        }
        else {
          leds[i] = pgm_read_dword(&(image[x][mw - y - 1]));
        }
        i++;
      }
    }
  }
  else {
    int i = 0;
    for (int x = 0; x < mh; x++) {
      for (int y = 0; y < mw; y++) {
        leds[i] = pgm_read_dword(&(image[x][y]));
        i++;
      }
    }
  }
  FastLED.show();
}



uint32_t getPixColor(int thisSegm) {
  int thisPixel = thisSegm * 1;
  if (thisPixel < 0 || thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

uint16_t getPixelNumber(int8_t x, int8_t y) {
  if ((y % 2 == 0)) {               // если чётная строка
    return (y * mw + x);
  } else {                                              // если нечётная строка
    return (y * mw + mw - x - 1);
  }
}

uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(getPixelNumber(x, y));
}

void drawPixelXY(int8_t x, int8_t y, CRGB color) {
  if (x < 0 || x > mw - 1 || y < 0 || y > mh - 1) return;
  int thisPixel = getPixelNumber(x, y);
  leds[thisPixel] = color;

}
