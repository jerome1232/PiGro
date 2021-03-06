#include "Arduino.h"
#include "greenhouse.h"

// int sleep_delay = 900000;           // Wait 15 minutes for next check

Greenhouse greenhouse = Greenhouse();

void setup() {
  // Begin serial communication at
  // baud rate of 9600
  Serial.begin(9600);
  greenhouse.begin();
  delay(1000);
}

void loop() {
  unsigned long sleep_delay = 300000UL;   // sleep for 5 minutes
  // Initialize variables
  // String data = "";

  // Light up built-in LED while running sensor check
  digitalWrite(LED_BUILTIN, HIGH);

  // Read in any available threshold settings.
  // if (Serial.available() > 0) {
  //   data = Serial.readStringUntil('>');
  // }

  greenhouse.run_sensor_check();
  greenhouse.run_tasks();

  // Write data to serial line
  // I feel like I should be able to do this with a simple
  // loop, but I'm not sure how yet.
  Serial.print('<');
  Serial.print("light:");
  Serial.print(greenhouse.get_light_level());
  Serial.print(',');
  Serial.print("temp:");
  Serial.print(greenhouse.get_temp());
  Serial.print(',');
  Serial.print("humidity:");
  Serial.print(greenhouse.get_humidity());
  Serial.print(',');
  Serial.print("heat:");
  Serial.print(greenhouse.get_heater_status());
  Serial.print(',');
  Serial.print("light_status:");
  Serial.print(greenhouse.get_light_status());
  Serial.print(',');
  Serial.print("soil_moisture_1:");
  Serial.print(greenhouse.get_soil_moisture_1());
  Serial.print(',');
  Serial.print("soil_moisture_2:");
  Serial.print(greenhouse.get_soil_moisture_2());
  Serial.print('>');

  // Turn off built-in LED light before sleep
  digitalWrite(LED_BUILTIN, LOW);
  delay(sleep_delay);
  // Sleep until next loop
}
