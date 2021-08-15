#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h> //  https://github.com/me-no-dev/ESPAsyncWebServer and https://github.com/me-no-dev/ESPAsyncTCP
#include <WiFiClient.h>

#include "configure.h"
#include "fileSystem.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "jsonParse.h"
#include "rtcFile.h"
#include "controller.h"

AsyncWebServer server(80);

String demoJson = "  {\n"
"  \"feedTimes\": [\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 2\n"
"    },\n"
"    {\n"
"      \"feedTime\": 2,\n"
"      \"feedDuration\": 22\n"
"    }\n"
"  ],\n"
"  \"pumpTime\": {\n"
"    \"onTime\": 2,\n"
"    \"offTime\": 2\n"
"  },\n"
"  \"aerationTime\": {\n"
"    \"onTime\": 2,\n"
"    \"offTime\": 2\n"
"  },\n"
"  \"settings\": {\n"
"    \"useSystemTime\": true,\n"
"    \"customTime\": \"\"\n"
"  }\n"
"}";

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();
  init_rtc();
  initPins();
  readDataFromFile();
  getData();

  // todo: set as access point
  //  WiFi.begin("Yedu", "yeduyedu");
  WiFi.begin("ZTE_2.4G_ExQCMa", "NullReferenceException#123");
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

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest * request) {
    // TODO: Send current file values
    request->send(200, "application/json", demoJson);
  });


  AsyncCallbackJsonWebHandler* handler =
  new AsyncCallbackJsonWebHandler("/settings", [](AsyncWebServerRequest * request, JsonVariant & json) {
    StaticJsonDocument<768> doc = json.as<JsonObject>();

    for (JsonObject feedItem : doc["feedTimes"].as<JsonArray>()) {

      int feedTime = feedItem["feedTime"];
      int feedDuration = feedItem["feedDuration"];

      Serial.print("feedTime: ");
      Serial.println(feedTime);
      Serial.print("feedDuration: ");
      Serial.println(feedDuration);

    }

    int pumpOnTime = doc["pumpTime"]["onTime"];
    int pumpOffTime = doc["pumpTime"]["offTime"];

    int aerationOnTime = doc["aerationTime"]["onTime"];
    int aerationOffTime = doc["aerationTime"]["offTime"];

    bool useSystemTime = doc["settings"]["useSystemTime"];
    const char* customTime = doc["settings"]["customTime"];

    Serial.print("pumpOnTime: ");
    Serial.println(pumpOnTime);

    Serial.print("aerationOnTime: ");
    Serial.println(aerationOnTime);

    Serial.print("useSystemTime: ");
    Serial.println(useSystemTime);

    Serial.print("customTime: ");
    Serial.println(customTime);

    request->send(200, "application/json", demoJson);
  });


  server.addHandler(handler);

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
