#include "Arduino.h"
#include <DHT.h>
#include "greenhouse.h"

#define  DHT_TYPE                DHT11
#define  DHT_PIN                 8
#define  LED_STRIP_PIN           3
#define  WATER_PUMP_PIN          6
#define  VALVE_01                7
#define  VALVE_02                5
#define  VALVE_03                4
#define  LIGHT_SENSOR_PIN        A0
#define  SOIL_MOISTURE01_PIN     A1
#define  SOIL_MOISTURE02_PIN     A2
#define  HEATER_PIN              9


Greenhouse::Greenhouse():_dht(DHT_PIN, DHT_TYPE) {
  _temp_low = 26;
  _temp_high = 33;
  _humidity_low = 80;
  _light_thresh = 400;
  _dht = DHT(DHT_PIN, DHT_TYPE);
}

Greenhouse::Greenhouse(int temp_low, int temp_high,
                       int humidity_low, int light_thresh
                      ):_dht(DHT_PIN, DHT_TYPE) {
  _temp_low = temp_low;
  _temp_high = temp_high;
  _humidity_low = humidity_low;
  _light_thresh = light_thresh;
  _dht = DHT(DHT_PIN, DHT_TYPE);
}

void Greenhouse::begin() {

  _dht.begin();

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

  _is_light_on = false;
  _is_heater_on = false;
}

void Greenhouse::run_tasks() {
  operate_water();
  operate_light();
  operate_water();
}

void Greenhouse::run_sensor_check() {
  _temp = check_temp();
  _humidity = check_humidity();
  _light_level = check_light_level();
  _soil_moisture_1 = check_soil_moisture_1();
  _soil_moisture_2 = check_soil_moisture_2();
}

bool Greenhouse::get_light_status() {
  return _is_light_on;
}

bool Greenhouse::get_heater_status() {
  return _is_heater_on;
}

float Greenhouse::get_temp() {
  return _temp;
}

float Greenhouse::get_humidity() {
  return _humidity;
}

int Greenhouse::get_light_level() {
  return _light_level;
}

int Greenhouse::get_soil_moisture_1() {
  return _soil_moisture_1;
}

int Greenhouse::get_soil_moisture_2() {
  return _soil_moisture_2;
}

float Greenhouse::check_temp() {
  return _dht.readTemperature();
}

float Greenhouse::check_humidity() {
  return _dht.readHumidity();
}

int Greenhouse::check_light_level() {
  return analogRead(LIGHT_SENSOR_PIN);
}

void Greenhouse::water_pump_on_off(bool on_off) {
  if (on_off) {
    digitalWrite(WATER_PUMP_PIN, HIGH);
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW);
  }
}

int Greenhouse::check_soil_moisture_1() {
  return analogRead(SOIL_MOISTURE01_PIN);
 }

int Greenhouse::check_soil_moisture_2() {
  return analogRead(SOIL_MOISTURE02_PIN);
}

void Greenhouse::lights_on_off(bool on_off) {
  _is_light_on = on_off;
  if (on_off) {
    digitalWrite(LED_STRIP_PIN, HIGH);
  } else {
    digitalWrite(LED_STRIP_PIN, LOW);
  }
}

void Greenhouse::heater_on_off(bool on_off) {
  _is_heater_on = on_off;
  if (on_off) {
    digitalWrite(HEATER_PIN, HIGH);
  } else {
    digitalWrite(HEATER_PIN, LOW);
  }
}

void Greenhouse::operate_heat() {
  if (_temp < _temp_low) {
    heater_on_off(true);
  }
  if (_temp > _temp_high) {
    heater_on_off(false);
  }
}

void Greenhouse::operate_light() {
  lights_on_off(false);
  delay(100);

  if (check_light_level() < _light_thresh) {
    lights_on_off(true);
  }
}

void Greenhouse::operate_water() {
  // dummy
}