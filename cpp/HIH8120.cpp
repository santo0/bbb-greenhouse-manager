//============================================================================
// Name        : HIH8120.cpp
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description : Gets temperature and humidity from HIH8120 sensor through I2C
//============================================================================

#include "HIH8120.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <bitset>

using namespace std;

namespace exploringBB {


/**
 * The constructor for the HIH8120 accelerometer object. It passes the bus number and the
 * device address (with is 0x53 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the HIH8120 device is on - typically 0 or 1
 * @param I2CAddress The address of the ADLX345 device (default 0x53, but can be altered)
 */
HIH8120::HIH8120(unsigned int I2CBus, unsigned int I2CAddress):
	I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->temperature=-1.0;
	this->humidity=-1.0;
	this->registers = NULL;
	//this->updateRegisters();
}

/**
 * Reads the temperature and humidity sensor, and converts it in readable format.
 *
 * @return 0 if the registers are successfully read.
 */


int HIH8120::readSensorState(){
	//Wake up sensor
	this->write(1);

	//Wait for sensor reply
	sleep(1);

	//Get 4 bytes from i2c
	unsigned char* data (this->readDevice(4));

	//Get 14 bits starting from the third bit (first two bits are for status)
	short humidity = ((data[0] & 0x3f) << 8) | data[1];
	//Convert bits to readable float format
	this->humidity = humidity/ 16382.0 * 100.0;

	//Get 14 bits starting from the third byte
	short temperature = ((data[2] & 0xff) << 6) | (data[3] >> 2);
	//Convert bits to readable float format
	this->temperature = temperature / 16382.0 * 165.0 - 40;

	return 0;
}


/**
 * Returns the temperature in celsius saved from the last sensor reading.
 *
 * @return the temperature saved from the last sensor reading.
 */
float HIH8120::getTemperature() {
	return this->temperature;
}

/**
 * Returns the humidity percentage saved from the last sensor reading.
 *
 * @return the humidity saved from the last sensor reading.
 */
float HIH8120::getHumidity() {
	return this->humidity;
}


HIH8120::~HIH8120() {}

} /* namespace exploringBB */


int main() {
	exploringBB::HIH8120 sensor(2, 0x27);
	sensor.readSensorState();
	cout << "humidity="<<sensor.getHumidity()<<endl;
	cout << "temperature="<<sensor.getTemperature()<<endl;

	return 0;
}
