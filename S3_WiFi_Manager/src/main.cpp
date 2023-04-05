#include <Arduino.h>
#include <wifi_manager.h>
#include <fast_led_setup.h>
#include <pwm_setup.h>

#include <ArduinoJson.h>
#include <HTTPClient.h>

unsigned long previousMillis = 0;   // will store last time server data was grabbed
DynamicJsonDocument doc(1024);

const char* serverName = "http://raspberrypi2.local/post-esp-data.php";
const char* GET_server = "http://raspberrypi2.local/get_slider_data.php";
const char* slider1 = "000";
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BRL_test";
String sensorLocation = "Office";

unsigned long previousMillis_serverUpdate = 0;
const unsigned long serverUpdateInterval = 1000;

unsigned long previousMillis_toggleTappers = 0; // Store the last time the duty cycle was updated
const unsigned long tapperIntervalMax = 5000; // Interval for turning off the device (1 second)
int tapperInterval = 1000;
float LEDdutyCycle = 0.2;
float tapDutyCycle = 0.9;
bool isOn = true; 

void pullLatestData(){
  //Serial.println("Pulling latest data from server");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status() == WL_CONNECTED) 
  {
  HTTPClient http;

    // Begin request to server URL
    http.begin(GET_server);

    // Send GET request and get response code
    int httpResponseCode = http.GET();

    // Check if request was successful
    if (httpResponseCode>0) {
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);

      // Get response payload as a string
      String payload = http.getString();
      //Serial.println(payload);

      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
          Serial.print(F("Failed to parse JSON: "));
          Serial.println(error.c_str());
          return;
      }

      slider1 = doc["slider1"];
      Serial.println(slider1);
      
    }
    else {
      //Serial.print("Error code: ");
      //Serial.println(httpResponseCode);
    }
    // Close connection and free resources
    http.end();
  }
  else 
  {
    Serial.println("WiFi Disconnected");
  }
}

int convertAndMapValue(const String& stringValue, int minValue, int maxValue) {
  // Convert the string to an integer
  int intValue = stringValue.toInt();

  // Constrain the value to the range [0, 255]
  intValue = constrain(intValue, 0, 255);

  // Map the value to the desired range [minValue, maxValue]
  int mappedValue = map(intValue, 0, 255, minValue, maxValue);

  return mappedValue;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  setup_WifiManager();
  setup_FastLED();

  setup_pwm_duty_cycle(led_pin_1, 1);
  setup_pwm_duty_cycle(led_pin_2, 1);

  setup_pwm_duty_cycle(tap_pin_1, 1);
  setup_pwm_duty_cycle(tap_pin_2, 1);

  previousMillis_toggleTappers = 0; // Store the current time
  previousMillis_serverUpdate = 0;

}

void loop()
{ 
  // put your main code here, to run repeatedly
  check_status();

  unsigned long currentMillis_serverUpdate = millis();
  // check if time to get latest server data
  if (currentMillis_serverUpdate - previousMillis_serverUpdate >= serverUpdateInterval)
  {
    pullLatestData();
    tapperInterval = convertAndMapValue(slider1, 0, tapperIntervalMax);
    previousMillis_serverUpdate = currentMillis_serverUpdate;
  }

  unsigned long currentMillis_toggleTappers = millis();
  // Check if time to toggle tappers
  if (currentMillis_toggleTappers - previousMillis_toggleTappers >= tapperInterval) {
    // Save the current time
    previousMillis_toggleTappers = currentMillis_toggleTappers;

    isOn = !isOn;
    //Serial.println(isOn);

    // Update duty cycle based on device state
    if (isOn) {
      //switch left hand led and tapper on, right off
      set_pwm_duty_cycle(led_pin_1, LEDdutyCycle); // Set PWM duty cycle to the desired value
      set_pwm_duty_cycle(led_pin_2, 0); // Set PWM duty cycle to the desired value
      set_pwm_duty_cycle(tap_pin_1, tapDutyCycle); // Set PWM duty cycle to the desired value
      set_pwm_duty_cycle(tap_pin_2, 0); // Set PWM duty cycle to the desired value
    } else {
      // switch right on, left off
      set_pwm_duty_cycle(led_pin_1, 0); // Set PWM duty cycle to zero to turn off
      set_pwm_duty_cycle(led_pin_2, LEDdutyCycle); // Set PWM duty cycle to the desired value
      set_pwm_duty_cycle(tap_pin_1, 0); // Set PWM duty cycle to the desired value
      set_pwm_duty_cycle(tap_pin_2, tapDutyCycle); // Set PWM duty cycle to the desired value
    }
  }
}