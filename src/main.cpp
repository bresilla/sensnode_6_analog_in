#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_ASFcore.h>
#include <Adafruit_SleepyDog.h>
#include <WiFi101.h>



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
  String dataString = "";
  for (int analogPin = 0; analogPin < 6; analogPin++) {
    int sensor = analogRead(analogPin);
    int mapper = map(sensor, 0, 770, 1, 13000);
    if (mapper > 13500){
      mapper = 0;
    }
    dataString += String(mapper);
    if (analogPin < 5) {
      dataString += ",";
    }
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(5000);
  //Watchdog.sleep(20000);
}
