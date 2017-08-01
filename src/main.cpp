#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_ASFcore.h>
#include <Adafruit_SleepyDog.h>

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

void setup() {
  initSerial(9600);
  initSD(10);
}

void loop() {
  String dataString = "";
  for (int analogPin = 0; analogPin < 6; analogPin++) {
    int sensor = analogRead(analogPin);
    int mapper = map(sensor, 1, 770, 1, 13000);
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
}
