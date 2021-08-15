unsigned long int feedDurationMillis;
unsigned long int feedTimeStartMillis;
unsigned long int pumpOnDurationMillis;
unsigned long int pumpOffDurationMillis;
unsigned long previousPumpMillis = 0;
boolean pumpState = true;

void initPins()
{
  pinMode(feederPin, OUTPUT);
  pinMode(airPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pumpOnDurationMillis = (pumpOnDuration * 60 * 1000);
  pumpOffDurationMillis = (pumpOffDuration * 60 * 1000);
}

void controlFeed()
{
  DateTime now = rtc.now();

  for (int i = 0; i <= 10; i++)
  {
    if (now.hour() == feedTimeArray[i] && now.minute() == 0 && now.second() < 2 && !feedTimeStart)
    {
      feedDurationMillis = feedDurationArray[i] * 1000;
      digitalWrite(feederPin, HIGH);
      feedTimeStartMillis = millis();
      feedTimeStart = 1;
    }
    if ((millis() - feedTimeStartMillis) >= feedDurationMillis && feedTimeStart)
    {
      digitalWrite(feederPin, LOW);
      feedTimeStart = 0;
    }
  }
}

void controlPump()
{
  if (pumpState == HIGH)
  {
    if ((millis() - previousPumpMillis) >= pumpOnDurationMillis)
    {
      pumpState = LOW;
      previousPumpMillis = millis();
    }
  }
  else
  {
    if ((millis() - previousPumpMillis) >= pumpOffDurationMillis)
    {
      pumpState = HIGH;
      previousPumpMillis = millis();
    }
  }

  digitalWrite(pumpPin, pumpState);
}

void controlAir()
{
  DateTime now = rtc.now();

  if (now.hour() == aerationOnTime && now.minute() == 0 && now.second() < 2 && !feedTimeStart)
  {
    digitalWrite(airPin, HIGH);
  }
  if (now.hour() == aerationOffTime && now.minute() == 0 && now.second() < 2 && !feedTimeStart)
  {
    digitalWrite(airPin, LOW);
  }
}