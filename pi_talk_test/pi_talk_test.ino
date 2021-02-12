#include <DHT.h>
#include "PiSerial.h"

// ****************
// * Setting pins *
// ****************
#define DHT_PIN 		8
#define DHT_TYPE 		DHT11
#define LED_STRIP_PIN 		3
#define WATER_PUMP_PIN		6
#define VALVE_01		7
#define VALVE_02		5
#define VALVE_03		4
#define LIGHT_SENSOR_PIN 	A0
#define HEATER_PIN		9

DHT dht(DHT_PIN, DHT_TYPE);

int light_amount = 0;
int temp_low_thres = 0;
int temp_high_thres = 0;
int thresh_humidity = 0;
int thresh_light = 800;

void setup() {
  // Begin serial communication at
  // baude rate of 9600
  Serial.begin(9600);

  // Set pins as OUTPUT 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(VALVE_01, OUTPUT);
  pinMode(VALVE_02, OUTPUT);
  pinMode(VALVE_03, OUTPUT); 

  // Set pins as INPUT
  pinMode(DHT_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // begin dht sensor temp monitor
  dht.begin();

  // A small delay at startup
  delay(1000);
}

void loop() {
  String data = "";
  float temp_humid[2];
  digitalWrite(LED_BUILTIN, HIGH);

  if (Serial.available() > 0) {
    data = Serial.readStringUntil('>');
  }
  
  checkTempHumid(temp_humid);
  int lightLevel = checkLightLevel();
  levelLights(thresh_light);
	 

  Serial.print('<');
  Serial.print("light:");
  Serial.print(lightLevel);
  Serial.print(',');
  Serial.print("temp:");
  Serial.print(temp_humid[0]);
  Serial.print(',');
  Serial.print("humidity:");
  Serial.print(temp_humid[1]);
  Serial.print('>');

  digitalWrite(LED_BUILTIN, LOW);
  delay(300000);

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
void levelLights(int thresh_low) {

  digitalWrite(LED_STRIP_PIN, LOW);
  delay(100);

  if (checkLightLevel() < thresh_low) {
      digitalWrite(LED_STRIP_PIN, HIGH);
    }
}
