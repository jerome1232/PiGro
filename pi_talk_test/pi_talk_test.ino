#include <DHT.h>

// ****************
// * Setting pins *
// ****************
#define DHT_PIN 		8
#define DHT_TYPE 		DHT11
#define LED_STRIP_PIN 		3
#define WATER_PUMP_PIN		4
#define VALVE_01		5
#define VALVE_02		6
#define VALVE_03		7
#define LIGHT_SENSOR_PIN 	A0

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  dht.begin();
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
  
  // test code to test out MOSFET
  digitalWrite(LED_STRIP_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_STRIP_PIN, LOW);

  
  float temp_humid[2];
  checkTempHumid(temp_humid);
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

  if (lightLevel < 300) {
    digitalWrite(LED_STRIP_PIN, HIGH);
  } else {
    digitalWrite(LED_STRIP_PIN, LOW);
  }

  delay(5000);

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
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  return lightLevel;
}
