#include <Arduino.h>

#include <FastLED.h>

#define LED_PIN     38
#define NUM_LEDS    92
#define BRIGHTNESS  50
#define LED_TYPE    UCS1903
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;

void setup() { FastLED.addLeds<UCS1903,LED_PIN>(leds, NUM_LEDS); }

void loop(){ 
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS; i++) {   
    // fade everything out
    
 
    // let's set an led value
    leds[i] = CHSV(hue++,255,255);
 
    // now, let's first 20 leds to the top 20 leds, 
    FastLED.show();

    leds.fadeToBlackBy(10);
    FastLED.delay(10);
  }
}
 