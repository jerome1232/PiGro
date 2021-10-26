/*
 * PiSerial.h
 * 	Small library for communicationg with a rasbperry pi
 *
 * 	Author: Jeremy DeBry Jones
 */

#ifndef PiSerial_h
#define PiSerial_h

#include "Arduino.h"



class PiSerial {
	public:
		PiSerial();
		void read(char [256]);
		void write(char [256]);
};

#endif
