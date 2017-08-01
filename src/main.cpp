#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_ASFcore.h>
#include <Adafruit_SleepyDog.h>
#include <WiFi101.h>

String readSensor(int PINS){
  String dataString = "";
  for (int analogPin = 0; analogPin < PINS; analogPin++) {
    int sensor = analogRead(analogPin);
    int mapper = map(sensor, 0, 770, 1, 13000);
    if (mapper > 13500){
      mapper = 0;
    }
    dataString += String(mapper);
    if (analogPin < PINS-1) {
      dataString += ",";
    }
  }
  return dataString;
}
void writeFILE(String dataString) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("Reading file failed!");
  }
}
void writeSERIAL(String dataString) {
	Serial.println(dataString);
}
void sleepTime(float minutes, bool Dog){
  int millisec = minutes * 60000;
  if (Dog){
    Watchdog.sleep(millisec);
  }
  else{
    delay(millisec);
  }
}
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
  int status = WL_IDLE_STATUS;     // the WiFi radio's status
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
}

void setup() {
  initSerial(9600);
  initSD(10);
  //initWiFi("APPLEFY", "orchard1");
}

void loop() {
  String data = readSensor(6);
  writeFILE(data);
  writeSERIAL(data);
  sleepTime(0.1, false);
}
