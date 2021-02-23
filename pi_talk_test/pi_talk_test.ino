#include "Arduino.h"
#include "green_house.h"

// ****************
// * Setting pins *
// ****************
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


int temp_low_thresh = 26;  // low thresh at which heater kicks on
int temp_high_thresh = 0;  // high thresh at which heater kicks off
int thresh_humidity = 0;  // point at which to humidify the air
int thresh_light = 500;   // thresh at which to turn on lights
int sleep_delay = 300000;        // miliseconds to wait for next loop

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

  // begin dht sensor temp monitor

  // A small delay at startup
  delay(1000);
}

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