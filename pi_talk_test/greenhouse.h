#ifndef GREEN_HOUSE_H
#define GREEN_HOUSE_H

#include <DHT.h>

class Greenhouse {
    DHT _dht;
    public:
        Greenhouse();
        Greenhouse(
            int temp_low, int temp_high,
            int humidity_low, int light_thresh);
        void begin();
        void run_tasks();
        void run_sensor_check();
        bool get_light_status();
        bool get_heater_status();
        float get_temp();
        float get_humidity();
        int get_light_level();
        int get_soil_moisture_1();
        int get_soil_moisture_2();

    private:
        float _temp;
        float _humidity;
        int _light_level;
        int _soil_moisture_1;
        int _soil_moisture_2;
        bool _is_light_on;
        bool _is_heater_on;

        int _temp_low;
        int _temp_high;
        int _humidity_low;
        int _light_thresh;

        float check_temp();
        float check_humidity();
        int check_light_level();
        void water_pump_on_off(bool on_off);
        int check_soil_moisture_1();
        int check_soil_moisture_2();
        void lights_on_off(bool on_off);
        void heater_on_off(bool on_off);
        void operate_heat();
        void operate_light();
        void operate_water();
};

#endif