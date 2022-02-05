#!/bin/env sh
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno ~/PiGro/arduino_src -v
