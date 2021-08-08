#include <RTClib.h>
#include <Wire.h>

char currentTime[32];
RTC_DS3231 rtc;

void init_rtc(){
  Wire.begin();
  rtc.begin();
}

void setTime(){
  rtc.adjust(DateTime(2021, 8, 8, set_hour, set_minute, 0));
}

void getTime(){
  DateTime now = rtc.now();
  sprintf(currentTime, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
  Serial.println(currentTime);
}