String jsonString;
StaticJsonDocument<1024> doc;

int feedTimeArray[10];
int feedDurationArray[10];
int pumpOnDuration, pumpOffDuration;
int aerationOnTime, aerationOffTime;
bool settings_useSystemTime;
String settings_customTime;
int set_hour, set_minute;

unsigned long int feedDurationMillis;
unsigned long int pumpOnDurationMillis;
unsigned long int pumpOffDurationMillis;

int feederPin =D5 , airPin = D7, pumpPin = D0;

bool feedTimeStart = 0;
