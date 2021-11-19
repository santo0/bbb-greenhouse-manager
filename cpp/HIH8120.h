
#ifndef HIH8120_H_
#define HIH8120_H_
#include"I2CDevice.h"

/// The HIH8120 has 0x10 registers for data fields
#define BUFFER_SIZE 0x10

namespace exploringBB {

/**
 * @class HIH8120
 * @brief Specific class for the HIH8120 Temperature and Humidity Sensor that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the HIH8120 class.
 */
class HIH8120:protected I2CDevice{

private:
	unsigned int I2CBus, I2CAddress;
	unsigned char *registers;
	float temperature;
	float humidity;

public:
	HIH8120(unsigned int I2CBus, unsigned int I2CAddress=0x27);

	virtual short combineRegisters(unsigned char msb, unsigned char lsb);

	virtual int readSensorState();

	virtual float getTemperature();
	virtual float getHumidity();

	virtual ~HIH8120();
};

} /* namespace exploringBB */

#endif /* HIH8120_H_ */
