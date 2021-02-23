#include "Arduino.h"
#include "green_house.h"

int temp_low_thresh = 26;  // low thresh at which heater kicks on
int temp_high_thresh = 0;  // high thresh at which heater kicks off
int thresh_humidity = 0;  // point at which to humidify the air
int thresh_light = 500;   // thresh at which to turn on lights
int sleep_delay = 300000;        // miliseconds to wait for next loop

struct Sensor_Data {
  float temp_humid[2];
  float soil_moisture[2];
  bool lightsOn;
  bool heatOn;
  int lightLevel;
};

void loop() {
  // Initialize variables
  String data = "";
  struct Sensor_Data sensor_data = {
    {0.0, 0.0},
    {0.0, 0.0},
    false,
    false,
    0
  };

  // Light up built-in LED while running sensor check
  digitalWrite(LED_BUILTIN, HIGH);

  // Read in any available threshold settings.
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('>');
  }

  // Check sensors
  checkTempHumid(sensor_data.temp_humid);
  checkSoilMoisture(sensor_data.soil_moisture);
  sensor_data.lightLevel = checkLightLevel();

  // Take respective actions on sensor readings
  sensor_data.lightsOn = levelLights(thresh_light);
  sensor_data.heatOn = heat(temp_low_thresh, sensor_data.temp_humid[0]);


  // Write data to serial line
  Serial.print('<');
  Serial.print("light:");
  Serial.print(sensor_data.lightLevel);
  Serial.print(',');
  Serial.print("temp:");
  Serial.print(sensor_data.temp_humid[0]);
  Serial.print(',');
  Serial.print("humidity:");
  Serial.print(sensor_data.temp_humid[1]);
  Serial.print(',');
  Serial.print("heat:");
  Serial.print(sensor_data.heatOn);
  Serial.print(',');
  Serial.print("light_status:");
  Serial.print(sensor_data.lightsOn);
  Serial.print(',');
  Serial.print("soil_moisture_1:");
  Serial.print(sensor_data.soil_moisture[0]);
  Serial.print(',');
  Serial.print("soil_moisture_2:");
  Serial.print(sensor_data.soil_moisture[1]);
  Serial.print('>');

  // Turn off built-in LED light before sleep
  digitalWrite(LED_BUILTIN, LOW);

  // Sleep until next loop
  delay(sleep_delay);
}