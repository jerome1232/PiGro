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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(VALVE_01, OUTPUT);
  pinMode(VALVE_02, OUTPUT);
  pinMode(VALVE_03, OUTPUT); 
  pinMode(DHT_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  dht.begin();


  int temp_low_thres = 0;
  int temp_high_thres = 0;
  int thres_humidity = 0;
  int thres_light = 800;
}

void loop() {
  String data = "";

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('>');
    Serial.print('<');
    Serial.print("recv: ");
    Serial.print(data);
    Serial.print('>');
  }
  
  digitalWrite(LED_STRIP_PIN, LOW);

  delay(5000);
  digitalWrite(LED_BUILTIN, HIGH);

  float temp_humid[2];
  checkTempHumid(temp_humid);
  /*int lightLevel = levelLights(1000, 900);*/
  int lightLevel = checkLightLevel();
	 

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

int levelLights(int thresh_high, int thresh_low) {
  analogWrite(LED_STRIP_PIN, light_amount);
  delay(100);
  int lightLevel = checkLightLevel();
  
  if (lightLevel < thresh_low) {
    while (lightLevel <= thresh_high) {
      if (light_amount == 255) break;
      light_amount++;
      analogWrite(LED_STRIP_PIN, light_amount);
      delay(100);
      lightLevel = checkLightLevel();
    }
  } else {
    while (lightLevel >= thresh_low) {
      if (light_amount == 0) break;
      light_amount--;
      analogWrite(LED_STRIP_PIN, light_amount);
      delay(100);
      lightLevel = checkLightLevel();
    }
  }

  return lightLevel;
}
