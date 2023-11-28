#include <Arduino.h>
#include <../include/242oled.h>

void setup() 
{
  setupOLED();  
}

void loop(void)
{
  //ripples();
  //fillScreen();
  //delay(1000);
  int x = random(0,100);
  speedRipples(x);
}