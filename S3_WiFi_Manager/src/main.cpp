#include <Arduino.h>
#include <wifi_manager.h>
#include <fast_led_setup.h>
#include <pwm_setup.h>

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  setup_WifiManager();
  setup_FastLED();

  set_pwm_duty_cycle(led_pin_1, 0.005);
  // Set the duty cycle for pin 2 to 25%
  set_pwm_duty_cycle(led_pin_2, 0.005);

}

void loop()
{ 
  // put your main code here, to run repeatedly
  check_status();
  
  ChangePalettePeriodically();
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  
  FillLEDsFromPaletteColors( startIndex);
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}