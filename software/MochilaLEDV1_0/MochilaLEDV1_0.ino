#include <avr/pgmspace.h>
#include "FastLED.h"
#include <FontMatrise.h>

#include <FastLED_NeoMatrix.h>


//////////////////////CONFIGURACIÓN//////////////////////////
#define DATA_PIN 3         //Pin de los LEDs
#define CONNECTION_TYPE 0  //Si la conexión es en zig zag '0', si es en paralelo '1'

#define mh 17              //número de filas
#define mw 15              //número de columnas

#define NUM_LEDS  (mw * mh)

#define BRIGHTNESS 60         //Brillo Inicial
#define COLOR_ORDER GRB       //Depende del fabricante el orden de los colores puede variar{ RGB, RBG, GRB, GBR, BGR, BRG }
#define CURRENT_LIMIT 3000    //Corriente máxima de la fuente de alimentación

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
String runningText = "";
int globalBrightness = BRIGHTNESS;
byte modeCode; 
boolean loadingFlag = true;
uint32_t globalColor = 0x00ff00;

CRGB leds[NUM_LEDS];

/*
const uint32_t img01[17][15] PROGMEM = {  //pokeball
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x003366, 0x003366, },
  {0x003366, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x003366, 0x003366, },
  {0x003366, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFFFFFF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x003366, },
  {0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFFFFFF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, },
  {0x000000, 0xFFFFFF, 0xFFFFFF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x333333, 0xFFFFFF, 0x333333, 0x000000, 0xFF0000, 0xFF0000, 0x000000, },
  {0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF0000, 0xFF0000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, },
  {0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x333333, 0xFFFFFF, 0x333333, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, },
  {0x003366, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, },
  {0x003366, 0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x003366, },
  {0x003366, 0x003366, 0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
};*/
const uint32_t img01[17][15] PROGMEM = {
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0x003366, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0x003366, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, },
  {0xFF0033, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0xFF0033, },
  {0xFF0033, 0x003366, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, },
  {0xFF0033, 0x003366, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, },
  {0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, },
  {0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0x003366, 0xFF0033, },
  {0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0x003366, 0xFF0033, },
  {0xFF0033, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0x003366, 0xFF0033, 0x003366, 0xFF0033, 0x003366, 0x003366, 0xFF0033, },
  {0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0x003366, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, 0x003366, 0xFF0033, 0xFF0033, 0xFF0033, 0xFF0033, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
  {0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, 0x003366, },
};
const uint32_t img02[17][15] PROGMEM = {  //Arbol de Navidad
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0x000000, 0xFFFF00, 0x000000, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFCC00, 0x009933, 0xFFCC00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0x009933, 0x009933, 0x009933, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFCC00, 0xFFCC00, 0x3333FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x009933, 0xFF0000, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0xFFCC00, 0xFFCC00, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0xFFCC00, 0xFFCC00, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x009933, 0x3333FF, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x3333FF, 0x009933, 0x009933, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x009933, 0x009933, 0x009933, 0xFF0000, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x009933, 0x009933, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xFFCC00, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x009933, 0x009933, 0x009933, 0xFF0000, 0x009933, 0x009933, 0xFFCC00, 0x000000, 0x000000, },
  {0x000000, 0x009933, 0x009933, 0xFF0000, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x009933, 0x009933, 0x3333FF, 0x000000, 0x000000, },
  {0x000000, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0xFFCC00, 0x000000, },
  {0x009933, 0xFF0000, 0x009933, 0x009933, 0x3333FF, 0x009933, 0xFF0000, 0x009933, 0x009933, 0x009933, 0x009933, 0xFF0000, 0x009933, 0xFFCC00, 0xFFCC00, },
  {0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, 0x3333FF, 0x009933, 0x009933, 0x009933, 0x009933, 0x009933, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x663300, 0x663300, 0x663300, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
};
const uint32_t img03[17][15] PROGMEM = {  //logan
  {0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x000000, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x6666CC, 0x6666CC, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x6666CC, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0xFFFFFF, 0x000000, 0x000000, 0xFFFF33, 0x000000, 0x000000, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFF33, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0x000000, 0x000000, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0xCC9966, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xCC9966, 0xCC9966, 0xCC9966, 0x000000, 0x000000, 0x6666CC, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x6666CC, 0xCC9966, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xCC9966, 0xFFFF33, 0x6666CC, 0x6666CC, 0x6666CC, },
  {0x6666CC, 0x6666CC, 0x6666CC, 0x000000, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xCC9966, 0xFFFF33, 0x000000, 0x000000, 0x6666CC, },
  {0x6666CC, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0xFFFF33, },
  {0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0x000000, 0xFFFF33, },
  {0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0xFFFF33, 0x000000, 0x000000, 0xFFFF33, 0xFFFF33, },
};
const uint32_t img04[17][15] PROGMEM = {    //batman
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, },
  {0xFF0000, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0xFF0000, },
  {0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, },
  {0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, },
  {0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, },
  {0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, },
  {0xFF0000, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0xFF0000, },
  {0xFF0000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
};
const uint32_t img05[17][15] PROGMEM = {    //Ryu Mortal Kombat
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFF0000, 0xFF0000, 0x330000, 0x330000, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0xFF0000, 0xFF0000, 0x000033, 0xFF0000, 0x996633, 0x996633, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0xFF0000, 0x000033, 0xFF0000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x996633, 0x996633, 0x000033, 0x000033, 0xFF0000, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0x996633, 0x996633, 0x996633, 0x996633, 0x996633, 0xFF0000, 0x996633, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0x996633, 0x996633, 0x996633, 0x996633, 0x996633, 0xFF0000, 0x996633, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, 0x000033, 0x000033, 0xFF0000, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000033, 0x000033, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000033, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, },
  {0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, },
  {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, },
  {0x996633, 0xFFFFFF, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000033, },
  {0x996633, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x996633, 0x996633, 0x996633, 0x000033, 0x000033, },
  {0x996633, 0x996633, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x996633, 0x996633, 0x996633, 0x996633, 0x000033, },
  {0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, 0x000033, },
};
const uint32_t img06[17][15] PROGMEM = {   //corazon
  {0x003399, 0x009999, 0x009999, 0x009999, 0x009999, 0x009999, 0x003399, 0x003399, 0x003399, 0x009999, 0x009999, 0x009999, 0x009999, 0x009999, 0x003399, },
  {0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x003399, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, },
  {0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, },
  {0xFF0000, 0xFF0000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF0000, 0xFF0000, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFFFFFF, 0xFFFFFF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFFFFFF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, },
  {0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, },
  {0x003399, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x003399, },
  {0x003399, 0x003399, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x003399, 0x003399, },
  {0x000033, 0x003399, 0x003399, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x003399, 0x003399, 0x000033, },
  {0x000033, 0x000033, 0x003399, 0x003399, 0x009999, 0x009999, 0xFF0000, 0xFF0000, 0xFF0000, 0x009999, 0x009999, 0x003399, 0x003399, 0x000033, 0x000033, },
  {0x000000, 0x000033, 0x000033, 0x003399, 0x003399, 0x009999, 0x009999, 0xFF0000, 0x009999, 0x009999, 0x003399, 0x003399, 0x000033, 0x000033, 0x000000, },
  {0x000000, 0x000000, 0x000033, 0x000033, 0x003399, 0x003399, 0x009999, 0x009999, 0x009999, 0x003399, 0x003399, 0x000033, 0x000033, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000033, 0x000033, 0x003399, 0x003399, 0x009999, 0x003399, 0x003399, 0x000033, 0x000033, 0x000000, 0x000000, 0x000000, },
};

const uint32_t img07[17][15] PROGMEM = {    //Mario
  {0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x663300, 0x663300, 0x663300, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0xCC9933, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x663300, 0xCC9933, 0x663300, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x663300, 0xCC9933, 0x663300, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, },
  {0x000000, 0x000000, 0x663300, 0x663300, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0x0033FF, 0xFF0000, 0xFF0000, 0xFF0000, 0x0033FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x0033FF, 0xFF0000, 0xFF0000, 0x0033FF, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, },
  {0x000000, 0xCC9933, 0xCC9933, 0xFF0000, 0x0033FF, 0xCCFF00, 0x0033FF, 0x0033FF, 0xCCFF00, 0x0033FF, 0xFF0000, 0xCC9933, 0xCC9933, 0x000000, 0x000000, },
  {0x000000, 0xCC9933, 0xCC9933, 0xCC9933, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0xCC9933, 0xCC9933, 0xCC9933, 0x000000, 0x000000, },
  {0x000000, 0xCC9933, 0xCC9933, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0xCC9933, 0xCC9933, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x0033FF, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x0033FF, 0x0033FF, 0x0033FF, 0x000000, 0x000000, 0x0033FF, 0x0033FF, 0x0033FF, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x663300, 0x663300, 0x663300, 0x663300, 0x000000, 0x000000, 0x663300, 0x663300, 0x663300, 0x663300, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x663300, 0x663300, 0x663300, 0x663300, 0x663300, 0x000000, 0x000000, 0x663300, 0x663300, 0x663300, 0x663300, 0x663300, 0x000000, 0x000000, },
};
const uint32_t img08[17][15] PROGMEM = {   //Iron Man
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xCC9900, 0xFFCC33, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xCC0000, 0xCC0000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xFFCC33, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xCC0000, 0xCC0000, 0x000000, },
  {0x000000, 0x000000, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xCC0000, 0xCC0000, 0xCC0000, 0x000000, },
  {0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xCC9900, 0xCC9900, 0xCC9900, 0xCC9900, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, },
  {0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xCC9900, 0xCC9900, 0xCC9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFCC33, 0xCC0000, 0xCC0000, 0xCC0000, },
  {0x000000, 0x000000, 0xCC9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFCC33, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xCC0000, 0xCC0000, },
  {0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0x333333, 0xCC0000, 0xCC0000, },
  {0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xCC9900, 0xCC9900, 0x333333, 0x333333, 0xCC0000, 0xCC0000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xFFCC33, 0xFFCC33, 0xFFCC33, 0xCC9900, 0x333333, 0x333333, 0xCC0000, 0xCC0000, 0x330000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xCC9900, 0xCC9900, 0xCC9900, 0xCC9900, 0xCC9900, 0xCC9900, 0x333333, 0xCC0000, 0xCC0000, 0xCC0000, 0x330000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x333333, 0xCC9900, 0x333333, 0x333333, 0x333333, 0xFFCC33, 0x333333, 0xCC0000, 0xCC0000, 0x330000, 0x330000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xCC9900, 0x333333, 0xCC9900, 0xFFCC33, 0xFFCC33, 0x333333, 0xFFCC33, 0xCC0000, 0xCC0000, 0x330000, 0x330000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xCC0000, 0xCC9900, 0xCC0000, 0xCC0000, 0xCC0000, 0xFFCC33, 0xCC0000, 0xCC0000, 0x330000, 0x330000, 0x330000, 0xCC0000, },
  {0x000000, 0x000000, 0xCC0000, 0x330000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0x330000, 0x330000, 0xCC0000, 0xCC0000, 0xCC0000, },
  {0xCC0000, 0x330000, 0x330000, 0xCC0000, 0x330000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, 0x330000, 0x330000, 0xCC0000, 0xCC0000, 0xCC0000, 0xCC0000, },
};
const uint32_t img09[17][15] PROGMEM = {   //Emoti Feliz
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0xFFFF00, 0xFFFFFF, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, },
  {0x000000, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, },
  {0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, },
  {0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, },
  {0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, },
  {0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, },
  {0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
};
const uint32_t img10[17][15] PROGMEM = {     //Emoti Enfadado
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF9999, 0xFF9999, 0xFF0000, 0xFF9999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF9999, 0xFF9999, 0xFF9999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF9999, 0xFF9999, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, },
  {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, },
  {0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, },
  {0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x000000, 0x000000, 0x000000, 0x000000, },
  {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, },
};
const uint32_t img11[17][15] PROGMEM = {      //perro
  {0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, },
  {0x003399, 0xFF9900, 0xFF9900, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0xFF9900, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFF9900, 0xFF9900, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0xFF9900, 0xFF9900, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFFFFFF, 0xFF9900, 0xFF9900, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0xFF9900, 0x003399, 0x003399, 0x003399, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0xFF9900, 0x003399, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0x003399, },
  {0x003399, 0xFF9900, 0xFF9900, 0xFF9900, 0x000000, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0x000000, 0xFF9900, 0xFF9900, 0xFF9900, 0x003399, },
  {0x003399, 0xFFFFFF, 0xFF9900, 0xFF9900, 0x000000, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0x000000, 0xFF9900, 0xFF9900, 0xFFFFFF, 0x003399, },
  {0x003399, 0xFFFFFF, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFFFFFF, 0x003399, },
  {0x003399, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0x003399, },
  {0x003399, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x003399, },
  {0x003399, 0x003399, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0x000000, 0x000000, 0x000000, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x003399, 0x003399, },
  {0x003399, 0x003399, 0x003399, 0xFFFFFF, 0xFFFFFF, 0xFF9900, 0x000000, 0x000000, 0x000000, 0xFF9900, 0xFFFFFF, 0xFFFFFF, 0x003399, 0x003399, 0x003399, },
  {0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, 0x003399, },
};
void setup() {
  Serial.begin(9600);
  //Configuramos la tira LED
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
}

void loop() {
 bluetoothRoutine();
}
