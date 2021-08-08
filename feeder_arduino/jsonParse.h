void getData()
{
  while (!Serial)
    continue;

  StaticJsonDocument<384> doc;

  // Only for testing the file system. data = "  {\r\n  \"feedTime\": [\r\n    {\r\n      \"feedTime\": 7,\r\n      \"feedDuration\": 10\r\n    }\r\n  ],\r\n  \"pumpTime\": {\r\n    \"onTime\": 20,\r\n    \"offTime\": 40\r\n  },\r\n  \"aerationTime\": {\r\n    \"onTime\": 7,\r\n    \"offTime\": 23\r\n  },\r\n  \"settings\": {\r\n    \"useSystemTime\": false,\r\n    \"customTime\": \"16:32\"\r\n  }\r\n}";
  
  DeserializationError error = deserializeJson(doc, data);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  static int feedTimeSize = doc["feedTime"].size();

  for (int i = 0; i < feedTimeSize; i++)
  {
    feedTimeArray[i] = doc["feedTime"][i]["feedTime"];
    feedDurationArray[i] = doc["feedTime"][i]["feedDuration"];
  }

  pumpOnDuration = doc["pumpTime"]["onTime"];       // 20
  pumpOffDuration = doc["pumpTime"]["offTime"];     // 40
  aerationOnTime = doc["aerationTime"]["onTime"];   // 7
  aerationOffTime = doc["aerationTime"]["offTime"]; // 18

  bool settings_useSystemTime = doc["settings"]["useSystemTime"]; // false
  String settings_customTime = doc["settings"]["customTime"];     // "11:41"
  set_hour = settings_customTime.substring(0, 2).toInt();
  set_minute = settings_customTime.substring(3, 5).toInt();

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

  // Only for testing the file system. writeDataToFile();
}