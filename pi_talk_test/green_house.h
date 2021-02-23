
#ifndef GREEN_HOUSE_H
#define GREEN_HOUSE_H

// checkTempHumid
// checks a DHT11 sensor and puts
// it's return values into passed array
void checkTempHumid(float data[]);

// checkLightLevel
//
// reads analog reading from photoresistor
// and returns raw reading.
int checkLightLevel();

// levelLights
//
// Turns off LED's to check the current brightness
// and turn on/off LED's based on the light level
bool levelLights(int thresh_low);

// heat
//
// Turns on the heaters
bool heat(int thresh_low, int cur_temp);

// checkSoilMoisture
//
// Checks soil moisture of both bays
void checkSoilMoisture(float data[]);

#endif