void readDataFromFile()
{
  Serial.println(F("Inizializing FS..."));
  if (LittleFS.begin())
  {
    Serial.println(F("LittleFS mounted correctly."));
  }
  else
  {
    Serial.println(F("!An error occurred during LittleFS mounting"));
  }
  File valueFile = LittleFS.open("/values.txt", "r");
  if (!valueFile)
  {
    Serial.println("file open failed");
  }
  data = valueFile.readString();
  Serial.println("------ Data from file START -------");
  Serial.println(data);
  Serial.println("------ Data from file END -------");
  valueFile.close();
}

void writeDataToFile()
{
  File valueFile = LittleFS.open("/values.txt", "w");
  if (!valueFile)
  {
    // File not found
    Serial.println("Failed to open text file");
    return;
  }
  else
  {
    valueFile.println(data);
    valueFile.close();
  }
}