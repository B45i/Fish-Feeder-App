//LIBRARIES USED
#include <RTClib.h> // https://github.com/adafruit/RTClib/actions
#include <Wire.h>
#include <ESP8266WiFi.h> 
#include <ESPAsyncWebServer.h> //  https://github.com/me-no-dev/ESPAsyncWebServer and https://github.com/me-no-dev/ESPAsyncTCP
#include <WiFiClient.h>
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include "AsyncJson.h"

#include "configure.h"  //VARIABLES USED IN THE CODE
#include "functions.h" //FUNCTIONS USED IN THE CODE

void setup()
{
  Serial.begin(115200); //for debugging purpose
  SPIFFS.begin(); //begins the spiffs file system
  init_rtc(); //begins rtc
  initPins(); //initialise gpio pins
  readJsonStringFromFile(); //read json string from spiffs
  getData(); //get data and stores to respective variables by parsing json
  setupWebServer(); // creates wifi access point and starts an async web server which serves an html page and handle requests.
}

void loop()
{
  controlFeed();  //Feeder DC Motor controlling
  controlPump();  //Water pump controlling
  controlAir(); //Aerator controlling
  getTime(); //Print time to the serial
}
