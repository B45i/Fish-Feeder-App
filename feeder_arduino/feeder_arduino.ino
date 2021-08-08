#include <LittleFS.h>
#include <configure.h>
#include <fileSystem.h>
#include <ArduinoJson.h>
#include <jsonParse.h>
#include <rtcFile.h>
#include <controller.h>

void setup()
{
  Serial.begin(9600);
  init_rtc();
  initPins();
  readDataFromFile();
  getData();
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