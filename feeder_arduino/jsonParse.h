void getData()
{
  while (!Serial)
    continue;
 
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

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
