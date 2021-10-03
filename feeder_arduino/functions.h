/*init_rtc()
   Initialises the rtc and wire library.
*/
void init_rtc() {
  Wire.begin(); //Initiallise the Wire library for I2C communication with DS3231
  rtc.begin(); //Initialise the rtc
}
//_____________________________________________________________________________________________________________________________

/*setTime()
   Write time to the DS3231
*/
void setTime() {
  rtc.adjust(DateTime(2021, 8, 8, set_hour, set_minute, 0));
}
//_____________________________________________________________________________________________________________________________

/*getTime()
   For debugging only.
   Gets the current time from DS3231 and print to serial.
*/
void getTime() {
  DateTime now = rtc.now();
  sprintf(currentTime, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
  Serial.println(currentTime);
}
//_____________________________________________________________________________________________________________________________

/*getData()
   Deserialize json string and store the values to variables.
   Also print the variables in serial for debugging.
*/
void getData()
{
  while (!Serial)
    continue;

  //Check for any error
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //Extract values and store to variables
  static int feedTimeSize = doc["feedTimes"].size();

  for (int i = 0; i < feedTimeSize; i++)
  {
    feedTimeArray[i] = doc["feedTimes"][i]["feedTime"];
    feedDurationArray[i] = doc["feedTimes"][i]["feedDuration"];
  }

  pumpOnDuration = doc["pumpTime"]["onTime"];       // 20
  pumpOffDuration = doc["pumpTime"]["offTime"];     // 40
  aerationOnTime = doc["aerationTime"]["onTime"];   // 7
  aerationOffTime = doc["aerationTime"]["offTime"]; // 18

  pumpOnDurationMillis = (pumpOnDuration * 60 * 1000);
  pumpOffDurationMillis = (pumpOffDuration * 60 * 1000);

  bool settings_useSystemTime = doc["settings"]["useSystemTime"]; // false
  String settings_customTime = doc["settings"]["customTime"];     // "11:41"
  set_hour = settings_customTime.substring(0, 2).toInt();
  set_minute = settings_customTime.substring(3, 5).toInt();

  //Print the variables for debugging
  Serial.println("");
  for (int i = 0; i < feedTimeSize; i++)
  {
    Serial.print(feedTimeArray[i]);
    Serial.print(" | ");
    Serial.println(feedDurationArray[i]);
  }

  Serial.println(pumpOnDuration);
  Serial.println(pumpOffDuration);
  Serial.println(aerationOnTime);
  Serial.println(aerationOffTime);
  Serial.println(settings_useSystemTime);
  Serial.println(set_hour);
  Serial.println(set_minute);
}
//_____________________________________________________________________________________________________________________________

/*readJsonStringFromFile()
   Open the 'values.txt' file where the json string is stored.
   Read the file and write the content to the string 'jsonString'.
   Print the string for debugging and close the file.
*/
void readJsonStringFromFile()
{
  //Initialise file system
  Serial.println(F("Inizializing FS..."));
  if (SPIFFS.begin())
  {
    Serial.println(F("SPIFFS mounted correctly."));
  }
  else
  {
    Serial.println(F("!An error occurred during LittleFS mounting"));
  }

  //Opens the value.txt file from the flash.
  File valueFile = SPIFFS.open("/values.txt", "r");
  if (!valueFile)
  {
    Serial.println("file open failed");
  }

  //Stores the content of the file to the jsonString, print to serial and the file is closed.
  jsonString = valueFile.readString();
  Serial.println("------ Data from file START -------");
  Serial.println(jsonString);
  Serial.println("------ Data from file END -------");
  valueFile.close();
}
//_____________________________________________________________________________________________________________________________

/*writeJsonStringToFile()
   Open the 'values.txt' file where the json string is stored.
   Write the new json string to the file and close the file.
*/
void writeJsonStringToFile()
{
  //Opens the value.txt file from the flash.
  File valueFile = SPIFFS.open("/values.txt", "w");

  //If file is found, write the content of jsonString to the file and close the file.
  if (!valueFile)
  {
    // File not found
    Serial.println("Failed to open text file");
    return;
  }
  else
  {
    valueFile.println(jsonString);
    valueFile.close();
  }
}
//_____________________________________________________________________________________________________________________________

/*setupWebServer()
   Starts the Access Point and print the AP IP Address to the Serial
   Starts the server by using the default html page which is stored in the flash and with the last uploaded settings.
   Initialise the AsyncCallbackJsonWebHandler
     If a request has come, store the new json to the stack
     Cleares the 'jsonString'; json object is serialized to get a string and stored in 'jsonString'.
     The new 'jsonString' is parsed to update the variables with the func getData()
     Recieved real time is updated to the DS3231 with the func setTime()
     Write the new 'jsonString' to the flash using the func writeJsonStringToFile()
   Add the handler
   So the default page and requests are defined. Now begin the server, serve the file and listen for the requests
*/
void setupWebServer()
{
  //Starts the access point and print the IP address in the serial.
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(AP_Name, AP_Password) ? "Ready" : "Failed!");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  //Set the default files which are stored in the flash.
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.serveStatic("/MaterialIcons-Regular.woff2", SPIFFS, "/").setDefaultFile("MaterialIcons-Regular.woff2");

  //Turn on the server with the default page and the last loaded values.
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json", jsonString); //take the last loaded values.
  });


  AsyncCallbackJsonWebHandler* handler =
  new AsyncCallbackJsonWebHandler("/settings", [](AsyncWebServerRequest * request, JsonVariant & json) {
    doc = json.as<JsonObject>(); //store the json to doc

    jsonString = ""; //cleares the previous json string
    serializeJson(doc, jsonString); //Serialize the json object to jsonString
    Serial.println(jsonString);
    getData();  //Parse the json string and store the settings to variables.
    setTime();  //Set the system time to rtc
    writeJsonStringToFile(); //Write the new string to the flash.

    request->send(200, "application/json", jsonString);
  });

  server.addHandler(handler); //Add the request handler

  server.begin(); //Begins the server
}
//_____________________________________________________________________________________________________________________________

/*initPins()
    Initialise the GPIO Pins
*/
void initPins()
{
  //Initialise GPIOs for feeder DC Motor, submersible pump and aerator.
  pinMode(feederPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(airPin, OUTPUT);
}
//_____________________________________________________________________________________________________________________________

/*controlFeed()
    Check the current time from DS3231
    If it belongs to any of the feed timings, feeding starts.
    The millis when the feeding started is stored to feedTimeStartMillis
    Feeding is stopped when the difference between millis and feedTimeStartMillis elapses the feed duration.
*/
void controlFeed()
{
  DateTime now = rtc.now(); //Fetch current time.

  for (int i = 0; i <= 10; i++)
  {
    //if it's not feeding and the current time is equal to any of the feed time,
    if (!feedTimeStart && now.hour() == feedTimeArray[i] && now.minute() == 0 && now.second() < 2)
    {
      feedDurationMillis = feedDurationArray[i] * 1000;
      digitalWrite(feederPin, HIGH); //feeder motor turns ON
      feedTimeStartMillis = millis(); //millis when the feeding has started is stored to the variable.
      feedTimeStart = 1; //Set the flag to know that the feeding has started.
    }

    //if it is feeding and difference between millis and feedTimeStartMillis elapsed,
    if (feedTimeStart && (millis() - feedTimeStartMillis) >= feedDurationMillis)
    {
      digitalWrite(feederPin, LOW); //feeder motor turns OFF
      feedTimeStart = 0; //Reset the flag to know that the feeding has stopped.
    }
  }
}
//_____________________________________________________________________________________________________________________________

/*controlPump()
   if the pump is ON
    and if the time difference is higher than the ON time, OFF the pump and update the time variable
   if the pump is OFF
    and if the time difference is higher than the OFF time, ON the pump and update the time variable
*/
void controlPump()
{
  //if the pump is ON,
  if (pumpState == HIGH)
  {
    //if the time difference is higher than the ON time, OFF the pump and update the time variable.
    if ((millis() - previousPumpMillis) >= pumpOnDurationMillis)
    {
      pumpState = LOW;
      previousPumpMillis = millis();
    }
  }
  //if the pump is OFF,
  else
  {
    //if the time difference is higher than the OFF time, ON the pump and update the time variable.
    if ((millis() - previousPumpMillis) >= pumpOffDurationMillis)
    {
      pumpState = HIGH;
      previousPumpMillis = millis();
    }
  }

  //Write the pump state to the GPIO
  digitalWrite(pumpPin, pumpState);
}
//_____________________________________________________________________________________________________________________________

/*controlAir()
    Check the current time from DS3231
    If current hour belongs to the air ON hour, turn the aerator ON.
    Else turn the aerator OFF.
    Two separate code blocks are used for (aerationOnTime < aerationOffTime) and
    (aerationOnTime > aerationOffTime) to avoid the calculation error.
*/
void controlAir()
{
  DateTime now = rtc.now(); //Fetch current time.

  //Two separate code blocks are used for (aerationOnTime < aerationOffTime) and
  //(aerationOnTime > aerationOffTime) to avoid the calculation error.

  if (aerationOnTime < aerationOffTime)
  {
    //if current hour is in between the ON time, turn the aerator ON.
    if ((now.hour() >= aerationOnTime) && (now.hour() < aerationOffTime))
    {
      digitalWrite(airPin, HIGH);
    }
    //if current hour is in between the OFF time, turn the aerator OFF.
    else
    {
      digitalWrite(airPin, LOW);
    }
  }
  else
  {
    //if current hour is in between the OFF time, turn the aerator OFF.
    if ((now.hour() < aerationOnTime) && (now.hour() > aerationOffTime))
    {
      digitalWrite(airPin, LOW);
    }
    //if current hour is in between the ON time, turn the aerator ON.
    else
    {
      digitalWrite(airPin, HIGH);
    }
  }
}
