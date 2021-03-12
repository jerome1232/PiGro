/**
 * @file: greenhouse.cpp
 * @author: Jeremy D. Jones
 *
 * The purpose of this file is to implement the Greenhouse class
 * This allows for control of an automated greenhouse that can
 * heat, water, and light itself based on various thresholds.
 */

#include "Arduino.h"
#include "greenhouse.h"


/*
 * Default constructor
 * Sets all thresholds to default values
 */
Greenhouse::Greenhouse():_dht(DHT_PIN, DHT_TYPE) {
  _temp_low = 26;
  _temp_high = 33;
  _humidity_low = 30;
  _light_thresh = 300;
  _soil_moisture_1_thresh = 700;
  _soil_moisture_2_thresh = 700;
  _is_light_on = false;
  _is_heater_on = false;
}

/*
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

/*
 * Runs all grenhouse tasks, such as heating
 * light operation, watering and heating.
 */
void Greenhouse::run_tasks() {
  operate_water();
  operate_light();
  operate_water();
  operate_heat();
}

/*
 * Checks soil moisture for the bay passed as an int.
 */
int Greenhouse::check_soil_moisture(int bay) {
  int reading = -1;
  switch (bay) {
    case 1 : digitalWrite(SOIL_MOISTURE01_ON_PIN, HIGH);
             delay(1);
             reading = analogRead(SOIL_MOISTURE01_PIN);
             digitalWrite(SOIL_MOISTURE01_ON_PIN, LOW);
             break;
    case 2 : digitalWrite(SOIL_MOISTURE02_ON_PIN, HIGH);
             delay(1);
             reading = analogRead(SOIL_MOISTURE02_PIN);
             digitalWrite(SOIL_MOISTURE02_ON_PIN, LOW);
             break;
  }
  return reading;
}

/*
 * Checks all sensors and stores the result
 */
void Greenhouse::run_sensor_check() {
  _temp = check_temp();
  _humidity = check_humidity();
  _light_level = check_light_level();
  _soil_moisture_1 = check_soil_moisture(1);
  _soil_moisture_2 = check_soil_moisture(2);
}

/*
 * Turns led strip on/off
 */
void Greenhouse::lights_on(bool on) {
  _is_light_on = on;
  if (on) {
    digitalWrite(LED_STRIP_PIN, HIGH);
  } else {
    digitalWrite(LED_STRIP_PIN, LOW);
  }
}

/*
 * Turns water pump on/off
 */
void Greenhouse::water_pump_on(bool on) {
  if (on) {
    digitalWrite(WATER_PUMP_PIN, HIGH);
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW);
  }
}

/*
 * Turns heater on/off
 */
void Greenhouse::heater_on(bool on) {
  _is_heater_on = on;
  if (on) {
    digitalWrite(HEATER_PIN, HIGH);
  } else {
    digitalWrite(HEATER_PIN, LOW);
  }
}

/*
 * Turns valve_1 on/off
 */
void Greenhouse::valve_1_on(bool on) {
  if (on) {
    digitalWrite(VALVE_01, HIGH);
  } else {
    digitalWrite(VALVE_01, LOW);
  }
}

/*
 * Turns valve_2 on/off
 */
void Greenhouse::valve_2_on(bool on) {
  if (on) {
    digitalWrite(VALVE_02, HIGH);
  } else {
    digitalWrite(VALVE_02, LOW);
  }
}

/*
 * Turns valve_3 on/off
 */
void Greenhouse::valve_3_on(bool on) {
  if (on) {
    digitalWrite(VALVE_03, HIGH);
  } else {
    digitalWrite(VALVE_03, LOW);
  }
}


/*
 * Turns the heater on/off based on temp threshold data
 */
void Greenhouse::operate_heat() {
  if (_temp < _temp_low) {
    heater_on(true);
  }
  if (_temp > _temp_high) {
    heater_on(false);
  }
}

/*
 * Turns the lights on/off based on light threshold data
 */
void Greenhouse::operate_light() {
  lights_on(false);
  delay(100);

  if (check_light_level() < _light_thresh) {
    lights_on(true);
  }
}

/*
 * Waters planters based on soil moisture data.
 * Note: This will temporarily turn heating off to avoid overstressing
 *       power supply.
 * Also sprays a mister to humidify the air if the air has low relative humidity
 */
void Greenhouse::operate_water() {
  bool will_water = false;
  bool heat = _is_heater_on;

  if (_humidity < _humidity_low ||
    _soil_moisture_1 > _soil_moisture_1_thresh ||
      _soil_moisture_2 > _soil_moisture_2_thresh) {
    will_water = true;
    // Turn off the heater to avoid maxing out the power supply
    if (heat) { heater_on(false); }
    // prime the water pressure
    water_pump_on(true);
    delay(1000);
  }
  // do humidity mister separately
  if (_humidity < _humidity_low) {
    valve_2_on(true);
    delay(30000);
    valve_2_on(false);
  }
  // turn on watering valves
  if (_soil_moisture_1 > _soil_moisture_1_thresh) {
    valve_1_on(true);
  }
  if (_soil_moisture_2 > _soil_moisture_2_thresh) {
    valve_3_on(true);
  }

  // run water for 2.5 minutes
  if (will_water) {
    delay(60000);
  }

  // turn everything off
  water_pump_on(false);
  valve_1_on(false);
  valve_2_on(false);
  valve_3_on(false);

  // turn the heater back on if it was on previously
  if (heat) { heater_on(true); }
}