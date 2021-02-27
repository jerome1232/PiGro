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

/**
 * Default constructor
 * Sets all thresholds to default values
 */
Greenhouse::Greenhouse():_dht(DHT_PIN, DHT_TYPE) {
  _temp_low = 26;
  _temp_high = 33;
  _humidity_low = 80;
  _light_thresh = 300;
  _soil_moisture_1_thresh = 400;
  _soil_moisture_2_thresh = 400;
  _is_light_on = false;
  _is_heater_on = false;
}

/**
 * Set's all pins to output/input
 * and runs inital startup for dht11 sensor
 */
void Greenhouse::begin() {

  // Startup for DHT11 sensor
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
}

/**
 * Runs all grenhouse tasks, such as heating
 * light operation, watering and heating.
 */
void Greenhouse::run_tasks() {
  operate_water();
  operate_light();
  operate_water();
  operate_heat();
}

/**
 * Checks all sensors and stores the result
 */
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

int Greenhouse::get_temp_low() {
  return _temp_low;
}

int Greenhouse::get_temp_high() {
  return _temp_high;
}

int Greenhouse::get_humidity_low() {
  return _humidity_low;
}

int Greenhouse::get_light_thresh() {
  return _light_thresh;
}

int Greenhouse::get_soil_moisture_1_thresh() {
  return _soil_moisture_1_thresh;
}

int Greenhouse::get_soil_moisture_2_thresh() {
  return _soil_moisture_2_thresh;
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

int Greenhouse::check_soil_moisture_1() {
  return analogRead(SOIL_MOISTURE01_PIN);
 }

int Greenhouse::check_soil_moisture_2() {
  return analogRead(SOIL_MOISTURE02_PIN);
}

void Greenhouse::lights_on(bool on) {
  _is_light_on = on;
  if (on) {
    digitalWrite(LED_STRIP_PIN, HIGH);
  } else {
    digitalWrite(LED_STRIP_PIN, LOW);
  }
}

void Greenhouse::water_pump_on(bool on) {
  if (on) {
    digitalWrite(WATER_PUMP_PIN, HIGH);
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW);
  }
}

void Greenhouse::heater_on(bool on) {
  _is_heater_on = on;
  if (on) {
    digitalWrite(HEATER_PIN, HIGH);
  } else {
    digitalWrite(HEATER_PIN, LOW);
  }
}

void Greenhouse::valve_1_on(bool on) {
  if (on) {
    digitalWrite(VALVE_01, HIGH);
  } else {
    digitalWrite(VALVE_01, LOW);
  }
}

void Greenhouse::valve_2_on(bool on) {
  if (on) {
    digitalWrite(VALVE_02, HIGH);
  } else {
    digitalWrite(VALVE_02, LOW);
  }
}

void Greenhouse::valve_3_on(bool on) {
  if (on) {
    digitalWrite(VALVE_03, HIGH);
  } else {
    digitalWrite(VALVE_03, LOW);
  }
}

void Greenhouse::operate_heat() {
  if (_temp < _temp_low) {
    heater_on(true);
  }
  if (_temp > _temp_high) {
    heater_on(false);
  }
}

void Greenhouse::operate_light() {
  lights_on(false);
  delay(100);

  if (check_light_level() < _light_thresh) {
    lights_on(true);
  }
}

void Greenhouse::operate_water() {
  bool will_water = false;
  if (_humidity < _humidity_low ||
      _soil_moisture_1 < _soil_moisture_1_thresh ||
      _soil_moisture_2 < _soil_moisture_2_thresh) {
    will_water = true;
    // prime the water pressure
    water_pump_on(true);
    delay(5000);
  }
  if (_soil_moisture_1 < _soil_moisture_1_thresh) {
    valve_1_on(true);
  }
  if (_soil_moisture_2 < _soil_moisture_2_thresh) {
    valve_3_on(true);
  }
  if (_humidity < _humidity_low) {
    valve_2_on(true);
  }

  // run water for a minute
  if (will_water) {
    delay(60000);
    water_pump_on(false);
    valve_1_on(false);
    valve_2_on(false);
    valve_3_on(false);
  }
}