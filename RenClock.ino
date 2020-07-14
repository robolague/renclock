// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <FastLED.h>

RTC_DS1307 rtc;

#define NUM_LEDS 45
#define DATA_PIN 5
CRGB leds[NUM_LEDS];


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  delay(2000);
  //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  fill_solid(leds, NUM_LEDS, CRGB::Black);
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
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {
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
  if ((now.hour() == 19 && now.minute() == 30) || (now.hour() >= 20) || (now.hour() <=7)) {
    // hour is 7:30pm OR it's past 8:00pm OR it's before 7am
    Serial.print("It's past 7:30pm but before 7:00am");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.setBrightness(50);
    FastLED.show();
  }

//Morning
  if ((now.hour() == 7 && now.minute() == 30) || (now.hour() >= 8 && now.hour() < 13)) {
    // hour is 7:30am OR (it's past 8am AND it's before 1pm)
    Serial.print("It's past 7:30am and before 1pm");
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  //Naptime
  if ((now.hour() >= 13 && now.hour() < 14) || (now.hour() ==14 && now.minute() <= 30) ) {
    // hour is 1:00pm or it's 2:00-2:30
    Serial.print("It's past 1pm or between 2:00pm and 2:30pm");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  //Afternoon
  if ((now.hour() == 14 && now.minute() == 30) || (now.hour() > 14 && now.hour() < 19)) {
    // hour is 2:30pm OR (it's past 2pm AND it's before 7pm)
    Serial.print("It's past 2:30pm and before 1pm");
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.setBrightness(50);
    FastLED.show();
  }

  delay(60000); // only update every 1 minute

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
