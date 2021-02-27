#ifndef GREEN_HOUSE_H
#define GREEN_HOUSE_H

#include <DHT.h>

class Greenhouse {
    public:
        // constructor
        Greenhouse();

        // initializes pins for input/output
        void begin();

         /* runs actions based on sensor data turns on the heater if it's cold
         * waters dry bays etc...
         * must run run_sensor_check() at least once before running this
         */
        void run_tasks();

        /* Checks all sensors and stores them in private variables
         */
        void run_sensor_check();

        // getters
        bool get_light_status();
        bool get_heater_status();
        float get_temp();
        float get_humidity();
        int get_light_level();
        int get_soil_moisture_1();
        int get_soil_moisture_2();
        int get_temp_low();
        int get_temp_high();
        int get_humidity_low();
        int get_light_thresh();
        int get_soil_moisture_1_thresh();
        int get_soil_moisture_2_thresh();

        // setters
        void set_temp_low(int);
        void set_temp_high(int);
        void set_humidity_low(int);
        void set_light_thresh(int);
        void set_soil_moisture_1_thresh(int);
        void set_soil_moisture_2_thresh(int);
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
        int _temp_low;
        int _temp_high;
        int _humidity_low;
        int _light_thresh;
        int _soil_moisture_1_thresh;
        int _soil_moisture_2_thresh;
        DHT _dht;

        // Checks sensors and stores results
        float check_temp();
        float check_humidity();
        int check_light_level();
        int check_soil_moisture_1();
        int check_soil_moisture_2();

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