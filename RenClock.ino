// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <FastLED.h>

RTC_DS1307 rtc;

#define NUM_LEDS 45
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

const byte switchPin = 3; // momentary switch on 3, other side connected to ground, 2&3 are interrupt pins
volatile byte state = LOW;
bool override = false; //override is off

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  pinMode(switchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(switchPin),interruptbutton,CHANGE);
  
  //digitalWrite(switchPin, HIGH);  // turn on pullup resistor
  //delay(2000);
  //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.setBrightness(50);
  FastLED.show();

  while (!Serial); // for Leonardo/Micro/Zero

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // July 16, 2020 at 7am you would call:
    // rtc.adjust(DateTime(2020, 7, 16, 7, 0, 0));
  }
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop () {

  if (override == true) {
    Serial.print("We are in override mode! All systems RED");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.setBrightness(50);
    FastLED.show();
  }
  DateTime now = rtc.now();

//  Serial.print(now.year(), DEC);
//  Serial.print('/');
//  Serial.print(now.month(), DEC);
//  Serial.print('/');
//  Serial.print(now.day(), DEC);
//  Serial.print(" (");
//  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//  Serial.print(") ");
//  Serial.print(now.hour(), DEC);
//  Serial.print(':');
//  Serial.print(now.minute(), DEC);
//  Serial.print(':');
//  Serial.print(now.second(), DEC);
//  Serial.println();
//  if (now.year() == 2020) {
//    Serial.print("\nIt's 2020");
//    fill_solid(leds, NUM_LEDS, CRGB::Red);
//    FastLED.show();
//  }

//Nightime 
  if (override == false && ((now.hour() >= 20) || (now.hour() <=7) && now.minute() < 30)) {
    // It's past 8:00pm OR it's before 7:30am
    Serial.print("It's past 7:30pm but before 7:30am");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.setBrightness(50);
    FastLED.show();
  }

//Morning
  if (override == false && ((now.hour() == 7 && now.minute() == 30) || (now.hour() >= 8 && now.hour() < 13))) {
    // hour is 7:30am OR (it's past 8am AND it's before 1pm)
    Serial.print("It's past 7:30am and before 1pm");
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  //Naptime
  if (override == false && (now.hour() >= 13 && now.hour() <= 14) ) {
    // hour is 1:00pm or it's 2:00-2:30
    Serial.print("It's past 1pm or between 2:00pm and 3:00pm");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  //Afternoon
  if (override == false && (now.hour() > 14 && now.hour() < 20)) {
    // (it's between 3pm AND it's before 8pm)
    Serial.print("It's past 3:00pm and before 8pm");
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  
  delay(1000); // only update every 1 seconds
  //delay(60000); // only update every 1 minute
//just tests that colors are being sent appropriately

//  fill_solid(leds, NUM_LEDS, CRGB::Purple);
//  FastLED.setBrightness(50); // full brightness throws off the clock for several colors and is too much anyway
//  FastLED.show();
//  delay(3000);
//  fill_solid(leds, NUM_LEDS, CRGB::Black);
//  FastLED.show();
//  delay(3000);

  Serial.println("\nStill working");

}

void interruptbutton() {
  if (digitalRead(switchPin) == LOW) {
  // switch is pressed - pullup keeps pin high normally
  delay(100);                     // delay to debounce switch
  fill_solid(leds, NUM_LEDS, CRGB::Purple);
  FastLED.setBrightness(50);
  FastLED.show();
  override = !override;             // toggle running variable
  delay(1000);
  }
}
