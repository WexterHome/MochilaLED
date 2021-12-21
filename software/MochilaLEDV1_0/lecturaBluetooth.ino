#define PARSE_AMOUNT 4    // максимальное количество значений в массиве, который хотим получить
#define header '$'        // стартовый символ
#define divider ' '       // разделительный символ
#define ending ';'        // завершающий символ

byte effect;
byte animation;
byte imagen;
int intData[PARSE_AMOUNT];
boolean parseStarted;
boolean effectsFlag;
boolean imageFlag;
boolean animationFlag;
boolean textFlag;
boolean recievedFlag;


int modo;
byte pos;

void bluetoothRoutine() {
  parsing();
  if (!parseStarted) {
    if (effectsFlag) effects();
    if (imageFlag) {
      drawImage(chooseImage(intData[1]));
      imageFlag = false;
    }
    if (animationFlag) animations();
  }
  //if (textFlag) text(runningText, globalColor);
}

byte parse_index;
String string_convert = "";
enum modes {NORMAL, COLOR, TEXT} parseMode;

void parsing() {
  if (recievedFlag) {
    recievedFlag = false;
    switch (intData[0]) {
      case 1:                                    //Modo1: Cambio de brillo
        globalBrightness = intData[1];
        FastLED.setBrightness(globalBrightness);
        FastLED.show();
        break;
      case 2:                                    //Modo2: Limpiar Matriz
        effectsFlag = false;
        imageFlag = false;
        FastLED.clear();
        FastLED.show();
        break;
      case 3:                                    //Modo3: Elegir Efecto
        effect = intData[1];
        loadingFlag = true;
        effectsFlag = true;
        imageFlag = false;
        animationFlag = false;
        FastLED.setBrightness(globalBrightness);
        FastLED.show();
        break;
      case 4:                                    //Modo4: Mostrar Imagen
        effectsFlag = false;
        imageFlag = true;
        animationFlag = false;
        FastLED.setBrightness(globalBrightness);
        break;
      case 5:                                     //Modo5: Mostrar Animación
        animation = intData[1];
        effectsFlag = false;
        imageFlag = false;
        animationFlag = true;
        FastLED.setBrightness(globalBrightness);
        break;
    }
  }

  // ****************** PARSING*****************
  //Escrito por AlexGyver
  if (Serial.available() > 0) {
    char incomingByte;
    if (parseMode == TEXT) {
      runningText = Serial.readString();
      incomingByte = ending;
      parseMode = NORMAL;
    } else {
      incomingByte = Serial.read();
      Serial.println(incomingByte);
    }
    if (parseStarted) {
      if (incomingByte != divider && incomingByte != ending) {
        string_convert += incomingByte;
      } else {
        if (parse_index == 0) {
          byte thisMode = string_convert.toInt();
          if (thisMode == 6) parseMode = TEXT;
          else parseMode = NORMAL;

        }

        if (parse_index == 1) {
          if (parseMode == NORMAL) intData[parse_index] = string_convert.toInt();
        } else {
          intData[parse_index] = string_convert.toInt();
        }
        string_convert = "";
        parse_index++;
      }
    }
    if (incomingByte == header) {
      parseStarted = true;
      parse_index = 0;
      string_convert = "";
    }
    if (incomingByte == ending) {
      parseMode == NORMAL;
      parseStarted = false;
      recievedFlag = true;
    }
  }
}




uint32_t chooseImage(byte numImage) {
  switch (numImage) {
    case 0:
      return (img01);
      break;
    case 1:
      return (img02);
      break;
    case 2:
      return (img03);
      break;
    case 3:
      return (img04);
      break;
    case 4:
      return (img05);
      break;
    case 5:
      return (img06);
      break;
    case 6:
      return (img07);
      break;
    case 7:
      return (img08);
      break;
    case 8:
      return (img09);
      break;
    case 9:
      return (img10);
      break;
    case 10:
      return (img11);
      break;
  }
}

void animations() {
  switch (animation) {
    case 0: marioAnimation();
      break;
    case 1: personXAnimation();
      break;
  }
  FastLED.show();
}


void effects() {
  switch (effect) {
    case 0: rainbowEffect(10);
      break;
    case 1: colorsEffect();
      break;
    case 2: cloudEffect();
      break;
    case 3: partyEffect();
      break;
    case 4: oceanEffect();
      break;
    case 5: matrixEffect();
      break;
  }
  FastLED.show();
}
