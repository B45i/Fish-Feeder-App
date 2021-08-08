String data;

int feedTimeArray[10];
int feedDurationArray[10];
unsigned long int pumpOnDuration, pumpOffDuration;
int aerationOnTime, aerationOffTime;
bool settings_useSystemTime;
String settings_customTime;
int set_hour, set_minute;

int feederPin =D5 , airPin = D7, pumpPin = D0;

bool feedTimeStart = 0;