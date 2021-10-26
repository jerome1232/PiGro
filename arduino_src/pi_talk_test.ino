#include "Arduino.h"
#include "greenhouse.h"

// int sleep_delay = 900000;           // Wait 15 minutes for next check

Greenhouse greenhouse = Greenhouse();
unsigned long sleep_delay = 300000UL;   // sleep for 5 minutes

void setup() {
  // Begin serial communication at
  // baud rate of 9600
  Serial.begin(9600);
  greenhouse.begin();
  delay(1000);
}

void loop() {

  // Light up built-in LED while running sensor check
  digitalWrite(LED_BUILTIN, HIGH);

  // Read in any available threshold settings.
  while (Serial.available() > 0) {
    String key = Serial.readStringUntil(':');
    String value = Serial.readStringUntil(',');
    if (key == "low_temp") { greenhouse.set_temp_low(value.toFloat()); }
    else if (key == "high_temp") { greenhouse.set_temp_high(value.toFloat()); }
    else if (key == "low_humidity") { greenhouse.set_humidity_low(value.toInt()); }
    else if (key == "sleep_time") { sleep_delay = value.toInt(); }
    else if (key == "water_time") { greenhouse.set_water_time(value.toInt()); }
    else if (key == "water_thresh") {
      int raw_value = (value.toInt() / 100) * 1023;
      greenhouse.set_soil_moisture_1_thresh(raw_value);
      greenhouse.set_soil_moisture_2_thresh(raw_value);
    }
    else if (key == "light_thresh") { greenhouse.set_light_thresh(value.toInt()); }
  }

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
  Serial.print(',');
  // Also send current threshold data
  Serial.print("low_temp:");
  Serial.print(greenhouse.get_temp_low());
  Serial.print(',');
  Serial.print("high_temp:");
  Serial.print(greenhouse.get_temp_high());
  Serial.print(',');
  Serial.print("low_humidity:");
  Serial.print(greenhouse.get_humidity_low());
  Serial.print(',');
  Serial.print("sleep_time:");
  Serial.print(sleep_delay);
  Serial.print(',');
  Serial.print("water_time:");
  Serial.print(greenhouse.get_water_time());
  Serial.print(',');
  Serial.print("water_thresh:");
  Serial.print(greenhouse.get_soil_moisture_1_thresh());
  Serial.print(',');
  Serial.print("light_thresh:");
  Serial.print(greenhouse.get_light_thresh());
  Serial.print('>');

  // Turn off built-in LED light before sleep
  digitalWrite(LED_BUILTIN, LOW);
  delay(sleep_delay);
  // Sleep until next loop
}
