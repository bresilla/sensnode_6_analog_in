#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_ASFcore.h>
#include <Adafruit_SleepyDog.h>
#include <WiFi101.h>
#include <ThingerWifi101.h>

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
void sleepTime(float minutes, bool Dog){
  int millisec = minutes * 60000;
  if (Dog){
    Watchdog.sleep(millisec);
  }
  else{
    delay(millisec);
  }
}
void blinkLED(int blink) {
  for (int loop=0; loop<blink; loop++){
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);
	}
}

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
void writeTHING(String dataString){
  ThingerWifi101 thing("trimbresilla", "FEATHER_01", "FEATHER_01");
  thing.handle();
  pson data;
  data["FruitGauge"] = dataString;
  thing.write_bucket("FruitGauge", data);
}

void setup() {
  blinkLED(10);
  initSerial(9600);
  initSD(10);
  initWiFi("APPLEFY", "orchard1");
}

void loop() {
  String data = readSensor(6);
  writeFILE(data);
  writeSERIAL(data);
  //writeTHING(data);
  sleepTime(0.1, false);
}
