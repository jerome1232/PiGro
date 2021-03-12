#ifndef GREEN_HOUSE_H
#define GREEN_HOUSE_H
/**
 * @file: greenhouse.h
 * @author: Jeremy D. Jones
 *
 * The purpose of this file is to declare the Greenhouse class
 * This allows for control of an automated greenhouse that can
 * heat, water, and light itself based on various thresholds.
 */

#include <DHT.h>

#define  DHT_TYPE                DHT11
#define  DHT_PIN                 8
#define  LED_STRIP_PIN           3
#define  WATER_PUMP_PIN          6
#define  VALVE_01                7
#define  VALVE_02                5
#define  VALVE_03                4
#define  LIGHT_SENSOR_PIN        A0
#define  SOIL_MOISTURE01_PIN     A1
#define  SOIL_MOISTURE02_PIN     A2
#define  HEATER_PIN              9
#define  SOIL_MOISTURE01_ON_PIN  11
#define  SOIL_MOISTURE02_ON_PIN  10

class Greenhouse {
    public:

        // constructor
        Greenhouse();

        // initializes pins for input/output
        void begin();

         /**********************************************************************
         * runs actions based on sensor data turns on the heater if it's cold
         * waters dry bays etc...
         * must run run_sensor_check() at least once before running this
         **********************************************************************/
        void run_tasks();
        /***********************************************************************
        /* Checks all sensors and stores them in private variables
         **********************************************************************/
        void run_sensor_check();

        // getters
        bool get_light_status() { return _is_light_on; }
        bool get_heater_status() { return _is_heater_on; }
        float get_temp() { return _temp; }
        float get_humidity() { return _humidity; }
        int get_light_level() { return _light_level; }
        int get_soil_moisture_1() { return _soil_moisture_1; }
        int get_soil_moisture_2() { return _soil_moisture_2; }
        int get_temp_low() { return _temp_low; }
        int get_temp_high() { return _temp_high; }
        int get_humidity_low() { return _humidity_low; }
        int get_light_thresh() { return _light_thresh; }
        int get_soil_moisture_1_thresh() { return _soil_moisture_1_thresh; }
        int get_soil_moisture_2_thresh() { return _soil_moisture_2_thresh; }
        int get_water_time() { return _water_time; }

        // setters
        void set_temp_low(float temp_low) { _temp_low = temp_low; }
        void set_temp_high(float temp_high) { _temp_high = temp_high; }
        void set_humidity_low(int humidity_low) { _humidity_low = humidity_low; }
        void set_light_thresh(int light_thresh) { _light_thresh = light_thresh; }
        void set_soil_moisture_1_thresh(int moisture) { _soil_moisture_1_thresh = moisture; }
        void set_soil_moisture_2_thresh(int moisture) { _soil_moisture_2_thresh = moisture; }
        void set_water_time(int water_time) { _water_time = water_time; }
    private:

        // Sensor related data
        float _temp;
        float _humidity;
        int _light_level;
        int _soil_moisture_1;
        int _soil_moisture_2;
        bool _is_light_on;
        bool _is_heater_on;

        // threshold related data
        float _temp_low;
        float _temp_high;
        int _humidity_low;
        int _light_thresh;
        int _soil_moisture_1_thresh;
        int _soil_moisture_2_thresh;
        int _water_time;
        DHT _dht;

        // Checks sensors and stores results
        float check_temp() { return _dht.readTemperature();}
        float check_humidity() { return _dht.readHumidity(); }
        int check_light_level() { return analogRead(LIGHT_SENSOR_PIN); }
        int check_soil_moisture(int);

        // Turns device on/off based on bool sent
        void water_pump_on(bool);
        void lights_on(bool);
        void heater_on(bool);
        void valve_1_on(bool);
        void valve_2_on(bool);
        void valve_3_on(bool);

        // handles the logic for turning on devices
        void operate_heat();
        void operate_light();
        void operate_water();
};

#endif