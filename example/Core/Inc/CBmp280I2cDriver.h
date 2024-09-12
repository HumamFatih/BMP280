/*
 * CBmp280I2cDriver.h
 *
 *  Created on: Jun 22, 2024
 *      Author: humam
 */

#ifndef SRC_CBMP280I2CDRIVER_H_
#define SRC_CBMP280I2CDRIVER_H_
#include "i2c.h"
#include "CBmp280LowLevelDriver.h"

/**
 *  This class models I2C driver for the BMP280 sensor.
 */
class CBmp280I2cDriver : public CBmp280LowLevelDriver {
private :
	/** Pointer to the I2C handler. */
	I2C_HandleTypeDef* m_hi2c;

	/** Address of the BMP280 device. */
	uint8_t m_deviceAddress;

public:
	/**
	 * Constructor for CBmp280I2cDriver.
	 *
	 * @param h12c Pointer to the I2C handler
	 * @param deviceAddress address of BMP280 sensor
	 */
	CBmp280I2cDriver(I2C_HandleTypeDef* hi2c, uint8_t deviceAddress);

	/**
	 * Method to read registers of BMP280 sensor.
	 *
	 * @param startAddress Address of the first register to read
	 * @param data Pointer to the buffer to store the data
	 * @param bytes Number of bytes to read
	 * @return true if the read operation is succeed, false otherwise
	 */
	bool readRegisters(uint8_t startAddress, uint8_t* data, uint8_t bytes);

	/**
	 * Method to write registers of BMP280 sensor.
	 *
	 * @param address Address of the register
	 * @param data Value to write to the register
	 * @return true if the write operation is succeed, false otherwise
	 */
	bool writeRegister(uint8_t address, uint8_t data);
};

#endif /* SRC_CBMP280I2CDRIVER_H_ */
