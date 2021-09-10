#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h> //  https://github.com/me-no-dev/ESPAsyncWebServer and https://github.com/me-no-dev/ESPAsyncTCP
#include <WiFiClient.h>
#include "ArduinoJson.h"
#include "AsyncJson.h"

#include "configure.h"
#include "fileSystem.h"
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
  readJsonStringFromFile();
  getData();




  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("feeder", "feed1234") ? "Ready" : "Failed!");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());




  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.serveStatic("/MaterialIcons-Regular.woff", SPIFFS, "/").setDefaultFile("MaterialIcons-Regular.woff") ;
  server.serveStatic("/MaterialIcons-Regular.woff2", SPIFFS, "/").setDefaultFile("MaterialIcons-Regular.woff2");
  server.serveStatic("/MaterialIcons-Regular.ttf", SPIFFS, "/").setDefaultFile("MaterialIcons-Regular.ttf");

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest * request) {
    // TODO: Send current file values
    request->send(200, "application/json", jsonString);
  });


  AsyncCallbackJsonWebHandler* handler =
  new AsyncCallbackJsonWebHandler("/settings", [](AsyncWebServerRequest * request, JsonVariant & json) {
    doc = json.as<JsonObject>();

    jsonString = "";
    serializeJson(doc, jsonString);
    Serial.println(jsonString);
    getData();
    setTime();
    writeJsonStringToFile();

    request->send(200, "application/json", jsonString);
  });


  server.addHandler(handler);

  server.begin();
}

void loop()
{
  controlFeed();
  controlPump();
  controlAir();
  getTime();
  delay(1000);
}

/*
  If the submit button is pressed,
  store the json to the String variable 'data'
  call the 'writeDataToFile()' to store the data to the flash.
  call 'setTime()'
*/
