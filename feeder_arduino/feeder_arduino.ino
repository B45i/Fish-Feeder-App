#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h> //  https://github.com/me-no-dev/ESPAsyncWebServer and https://github.com/me-no-dev/ESPAsyncTCP
#include <WiFiClient.h>

#include "configure.h"
#include "fileSystem.h"
#include "ArduinoJson.h"
#include "jsonParse.h"
#include "rtcFile.h"
#include "controller.h"

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();
  init_rtc();
  initPins();
  readDataFromFile();
  getData();

  // todo: set as access point
  WiFi.begin("Yedu", "yeduyedu");
  WiFi.mode(WIFI_STA);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.begin();
}

void loop()
{
  // controlFeed();
  // controlPump();
  // controlAir();
}

/*
  If the submit button is pressed,
  store the json to the String variable 'data'
  call the 'writeDataToFile()' to store the data to the flash.
  call 'setTime()'
*/
