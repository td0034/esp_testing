#include <Arduino.h>
/**
 * OnDemandConfigPortal.ino
 * example of running the configPortal AP manually, independantly from the captiveportal
 * trigger pin will start a configPortal AP for 120 seconds then turn it off.
 * 
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>
#include <HTTPClient.h>

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 9
// define led and tapper pins
const int left_led = 21;
const int right_led = 13;
const int left_tap = 12;
const int right_tap = 14;

// seconds to run config portal for
int timeout = 120; 

unsigned long previousMillis = 0;   // will store last time server data was grabbed
DynamicJsonDocument doc(1024);

const char* serverName = "https://emdrtool.ddns.net/post-esp-data.php";
const char* GET_server = "https://emdrtool.ddns.net/get_slider_data.php";

unsigned long previousMillis_serverUpdate = 0;
const unsigned long serverUpdateInterval = 100;

const char* slider1 = "000";
unsigned long previousMillis_toggleTappers = 0; // Store the last time the duty cycle was updated
const unsigned long tapperIntervalMax = 3000; // Interval for turning off the device (1 second)
int tapperInterval = 100;
float LEDdutyCycle = 0.2;
float tapDutyCycle = 1.0;
bool isOn = true; 

void toggleTask(void *parameter);
void networkTask(void *parameter);

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
          //Serial.print(payload);
          //Serial.println(error.c_str());
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

void setup() {
  //WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println("\n Starting");
  
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);
  pinMode(left_tap, OUTPUT);
  pinMode(right_tap, OUTPUT);

  previousMillis_toggleTappers = 0; // Store the current time
  previousMillis_serverUpdate = 0;

  if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(); // Attempt to reconnect
  }

  xTaskCreatePinnedToCore(
    toggleTask,   /* Task function */
    "ToggleTask", /* Name of the task (for debugging) */
    10000,        /* Stack size */
    NULL,         /* Parameter to pass */
    1,            /* Task priority */
    NULL,         /* Task handle */
    0             /* Core where the task should run */
  );
  xTaskCreatePinnedToCore(
    networkTask,   /* Task function */
    "NetworkTask", /* Name of the task (for debugging) */
    10000,         /* Stack size */
    NULL,          /* Parameter to pass */
    1,             /* Task priority */
    NULL,          /* Task handle */
    1              /* Core where the task should run */
  );
}

void loop() {
  // is configuration portal requested?
 /* if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;    

    //reset settings - for testing
    //wm.resetSettings();
  
    // set configportal timeout
    wm.setConfigPortalTimeout(timeout);

    if (!wm.startConfigPortal("EMDRTool")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

  }

  unsigned long currentMillis_serverUpdate = millis();
  // check if time to get latest server data
  if (currentMillis_serverUpdate - previousMillis_serverUpdate >= serverUpdateInterval)
  {
    pullLatestData();
    tapperInterval = convertAndMapValue(slider1, 25, tapperIntervalMax);
    //Serial.println(tapperInterval);
    previousMillis_serverUpdate = currentMillis_serverUpdate;
  }

  unsigned long currentMillis_toggleTappers = millis();
  if (currentMillis_toggleTappers - previousMillis_toggleTappers >= tapperInterval)
  {
    isOn = !isOn;
    digitalWrite(left_led, isOn ? HIGH : LOW);
    digitalWrite(left_tap, isOn ? HIGH : LOW);
    digitalWrite(right_led, isOn ? LOW : HIGH);
    digitalWrite(right_tap, isOn ? LOW : HIGH);
    previousMillis_toggleTappers = currentMillis_toggleTappers;
  }

*/
  
 /* digitalWrite(left_led, HIGH);
    digitalWrite(left_tap, HIGH);
    digitalWrite(right_led, LOW);
    digitalWrite(right_tap, LOW);
    delay(tapperInterval);
    digitalWrite(left_led, LOW);
    digitalWrite(left_tap, LOW);
    digitalWrite(right_led, HIGH);
    digitalWrite(right_tap, HIGH);
    delay(tapperInterval);
  */
}

void toggleTask(void *parameter) {
  for (;;) {
    isOn = !isOn;
    digitalWrite(right_led, isOn ? HIGH : LOW);
    digitalWrite(left_tap, isOn ? HIGH : LOW);
    digitalWrite(left_led, isOn ? LOW : HIGH);
    digitalWrite(right_tap, isOn ? LOW : HIGH);

    vTaskDelay(tapperInterval / portTICK_PERIOD_MS); // Yield to other tasks
  }
}

void networkTask(void *parameter) {
  for (;;) {
    if ( digitalRead(TRIGGER_PIN) == LOW) {
      WiFiManager wm;    

      //reset settings - for testing
      //wm.resetSettings();
    
      // set configportal timeout
      wm.setConfigPortalTimeout(timeout);

      if (!wm.startConfigPortal("EMDRTool")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5000);
      }

      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");

    }
    
    pullLatestData();
    tapperInterval = convertAndMapValue(slider1, 50, tapperIntervalMax);

    vTaskDelay(serverUpdateInterval / portTICK_PERIOD_MS); // Delay for a second
  }
}