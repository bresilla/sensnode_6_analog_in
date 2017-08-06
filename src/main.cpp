#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <WiFi101.h>
#include <ThingerWifi101.h>

String date, sens;
RTC_PCF8523 rtc;

void initSerial(int PT) {
  Serial.begin(PT);
  while (!Serial) {
    Serial.println("Serial failed, or not connected");
    delay(1000);
    return;
  }
}
void initSD(int CS) {
  while (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    delay(1000);
    return;
  }
}
void initWiFi(char ssid[], char pass[]){
  WiFi.setPins(8,7,4,2);
  while (!WiFi.begin(ssid, pass)) {
    Serial.println("WiFi failed, or not connected");
    delay(1000);
  }
}
void initRTC(){
  while (!rtc.begin()) {
    Serial.println("RTC failed, or not present");
    delay(1000);
    return;
  }
  if (!rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}
void gotoSleep(float minutes, bool deepSleep){
  int millisec = minutes * 60000;
  Serial.println("\n\n\ngoing into sleep for " + String(minutes) + " minutes!");
  if (deepSleep){
    WiFi.lowPowerMode();
    delay(millisec);
    WiFi.noLowPowerMode();
  }
  else{
    delay(millisec);
  }
}
void blinkLED(int blink, int timer) {
  for (int loop=0; loop<blink; loop++){
		digitalWrite(LED_BUILTIN, HIGH);
		delay(timer);
		digitalWrite(LED_BUILTIN, LOW);
		delay(timer);
	}
}
int senseIT(int PIN){
  int sensor = analogRead(PIN);
  int mapper = map(sensor, 0, 770, 1, 13000);
  if (mapper > 13500){
    mapper = 0;
  }
  return mapper;
}

void writeFILE(String dataString) {
  File dataFile = SD.open("datafile.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("Writing datafile failed!");
  }
}
void writeSERIAL(String dataString) {
	Serial.println(dataString);
}
void writeTHING(String dataString){
  ThingerWifi101 thing("trimbresilla", "FEATHER_01", "FEATHER_01");
  thing.handle();
  pson data;
  data["FruitGauge"] = dataString;
  thing.write_bucket("FruitGauge", data);
}

String stringTime(DateTime time_now){
  String dataString = "";
  dataString += String(time_now.year(), DEC);
  dataString += "/";
  dataString += String(time_now.month(), DEC);
  dataString += "/";
  dataString += String(time_now.day(), DEC);
  dataString += ",";
  dataString += String(time_now.hour(), DEC);
  dataString += ":";
  dataString += String(time_now.minute(), DEC);
  return dataString;
}
String stringSens(int sens_number){
  String dataString = "";
  for (int loop=0; loop<sens_number; loop++){
    dataString += senseIT(loop);
    if (loop<sens_number-1) {
      dataString += ",";
    }
  }
  return dataString;
}

void setup() {
  initRTC();
  initSerial(9600);
  initSD(10);
  //initWiFi("APPLEFY", "orchard1");
}
void loop() {
  date = stringTime(rtc.now());
  sens = stringSens(6);
  writeFILE(date + ",\t\t" + sens);
  writeSERIAL(date + ",\t\t" + sens);
  //writeTHING(sens);
  blinkLED(10,100);
  gotoSleep(15, false);
}
