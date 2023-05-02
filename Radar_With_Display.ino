#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "FreqPeriod.h"

double lfrq;
long int pp;


#define OLED_MOSI     9
#define OLED_CLK      10
#define OLED_DC       11
#define OLED_CS       12
#define OLED_RST      13


// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

int counter = 0;

void setup()   {
  Serial.begin(9600);
  FreqPeriod::begin();
  Serial.println("FreqPeriod Library Test");

  display.begin(0, true); 
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Failure is always an option");
  display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
 
  
}


void loop() {
  display.clearDisplay();
  pp = FreqPeriod::getPeriod();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  if (pp) {
    //Serial.print ("period: ");
    //Serial.print(pp);
    //Serial.print(" 1/16us / frequency: ");

  lfrq = 16000400.0 /pp;
  //if ((lfrq/31.36) > 6.0) {
  display.print("Speed: "); display.print(lfrq/31.36); display.println(" Mph ");
  //Serial.print(lfrq);
  //Serial.print(" Hz ");
  Serial.print(lfrq/31.36);
  Serial.println( " Mph ");
  //} else {
  //display.print("Speed: "); display.println("0 Mph ");
  //}
  display.display();
  if ((lfrq/31.36) > 6.0) {
  delay(900);
  }
}

