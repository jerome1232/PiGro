#include "Arduino.h"
#include "green_house.h"

int temp_low_thresh = 26;  // low thresh at which heater kicks on
int temp_high_thresh = 0;  // high thresh at which heater kicks off
int thresh_humidity = 0;  // point at which to humidify the air
int thresh_light = 500;   // thresh at which to turn on lights
int sleep_delay = 300000;        // miliseconds to wait for next loop

void loop() {
  String data = "";
  float temp_humid[2];
  float soil_moisture[2];
  bool lightsOn = false;
  bool heatOn = false;
  int lightLevel = 0;

  // Light up built-in LED while running sensor check
  digitalWrite(LED_BUILTIN, HIGH);

  // Read in any available threshold settings.
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('>');
  }

  // Check sensors
  checkTempHumid(temp_humid);
  checkSoilMoisture(soil_moisture);
  lightLevel = checkLightLevel();

  // Take respective actions on sensor readings
  lightsOn = levelLights(thresh_light);
  heatOn = heat(temp_low_thresh, temp_humid[0]);

  // Write data to serial line
  Serial.print('<');
  Serial.print("light:");
  Serial.print(lightLevel);
  Serial.print(',');
  Serial.print("temp:");
  Serial.print(temp_humid[0]);
  Serial.print(',');
  Serial.print("humidity:");
  Serial.print(temp_humid[1]);
  Serial.print(',');
  Serial.print("heat:");
  Serial.print(heatOn);
  Serial.print(',');
  Serial.print("light_status:");
  Serial.print(lightsOn);
  Serial.print('>');

  // Turn off built-in LED light before sleep
  digitalWrite(LED_BUILTIN, LOW);

  // Sleep until next loop
  delay(sleep_delay);
}