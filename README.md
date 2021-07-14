# PiGro

This project utilizes an Arduino Uno paired with a Raspberry Pi 3b+ (though any Raspberry Pi would work), to automate a small greenhouse and host a local website which allows you to view visualizations of the greenhouse statistics. Things like temperature, humidity levels and etc...

### Capabilities

Currently the project tracks temperature, humidity, light leves, and soil moisture levels. It will water a plant bay when soil moisture drops below a set threshold. It turns on a strip of LED grow lights when the light level drops, and operates a small heater to prevent freezing when the temperature drops below a set threshold.

### Major TODO's

- [x] Get Pi communicating thresholds back to arduino.
- [ ] Move from static graphs to interactive graphs.
- [x] Get the graphs to update as new data comes in.

### Sample Web UI

![](images/web-sample.png)
