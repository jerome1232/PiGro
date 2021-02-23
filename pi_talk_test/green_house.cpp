#include "Arduino.h"
#include <DHT.h>

#define  DHT_PIN                 8
#define  DHT_TYPE                DHT11
#define  LED_STRIP_PIN           3
#define  WATER_PUMP_PIN          6
#define  VALVE_01                7
#define  VALVE_02                5
#define  VALVE_03                4
#define  LIGHT_SENSOR_PIN        A0
#define  SOIL_MOISTURE01_PIN     A1
#define  SOIL_MOISTURE02_PIN     A2
#define  HEATER_PIN              9

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  // Begin serial communication at
  // baud rate of 9600
  Serial.begin(9600);
  // Set pins as OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(VALVE_01, OUTPUT);
  pinMode(VALVE_02, OUTPUT);
  pinMode(VALVE_03, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);

  // Set pins as INPUT
  pinMode(DHT_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(SOIL_MOISTURE01_PIN, INPUT);
  pinMode(SOIL_MOISTURE02_PIN, INPUT);

  delay(1000);
}

// checkTempHumid
// checks a DHT11 sensor and puts
// it's return values into passed array
void checkTempHumid(float data[]) {
  data[0] = dht.readTemperature();
  data[1] = dht.readHumidity();
}

// checkLightLevel
//
// reads analog reading from photoresistor
// and returns raw reading.
int checkLightLevel() {
  return analogRead(LIGHT_SENSOR_PIN);
}


// levelLights
//
// Turns off LED's to check the current brightness
// and turn on/off LED's based on the light level
bool levelLights(int thresh_low) {

  digitalWrite(LED_STRIP_PIN, LOW);
  delay(100);

  if (checkLightLevel() < thresh_low) {
      digitalWrite(LED_STRIP_PIN, HIGH);
      return true;
    }
  return false;
}

// heat
//
// Turns on the heaters
bool heat(int thresh_low, int cur_temp) {
  if (cur_temp < thresh_low) {
    digitalWrite(HEATER_PIN, HIGH);
    return true;
  }
  digitalWrite(HEATER_PIN, LOW);
  return false;
}

// checkSoilMoisture
//
// Checks soil moisture of both bays
void checkSoilMoisture(float data[]) {
  data[0] = analogRead(SOIL_MOISTURE01_PIN);
  data[1] = analogRead(SOIL_MOISTURE02_PIN);
}