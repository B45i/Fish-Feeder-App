void readJsonStringFromFile()
{
  Serial.println(F("Inizializing FS..."));
  if (SPIFFS.begin())
  {
    Serial.println(F("SPIFFS mounted correctly."));
  }
  else
  {
    Serial.println(F("!An error occurred during LittleFS mounting"));
  }
  File valueFile = SPIFFS.open("/values.txt", "r");
  if (!valueFile)
  {
    Serial.println("file open failed");
  }
  jsonString = valueFile.readString();
  Serial.println("------ Data from file START -------");
  Serial.println(jsonString);
  Serial.println("------ Data from file END -------");
  valueFile.close();
}

void writeJsonStringToFile()
{
  File valueFile = SPIFFS.open("/values.txt", "w");
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
