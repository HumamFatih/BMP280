/*
 * CBmp280SpiDriver.h
 *
 *  Created on: Jun 23, 2024
 *      Author: humam
 */

#ifndef SRC_CBMP280SPIDRIVER_H_
#define SRC_CBMP280SPIDRIVER_H_
#include "spi.h"
#include "CBmp280LowLevelDriver.h"
#include "CGpioPin.h"

/**
 *  This class models SPI driver for the BMP280 sensor.
 */
class CBmp280SpiDriver : public CBmp280LowLevelDriver {
private :
	/** Pointer to the GPIO pin used for SPI slave select. */
	CGpioPin* m_ss;

	/** Pointer to the SPI handler. */
	SPI_HandleTypeDef* m_hspi;

public:
	/**
	 * Constructor for CBmp280SpiDriver.
	 *
	 * @param hspi Pointer to the SPI handler
	 * @param ss Pointer to the GPIO pin used for slave select
	 */
	CBmp280SpiDriver(SPI_HandleTypeDef* hspi, CGpioPin* ss);

	/**
	 * Method to read registers of BMP280 sensor.
	 *
	 * @param startAddress Address of the first register to read
	 * @param data Pointer to a buffer that store the data
	 * @param bytes Number of bytes to read
	 * @return true if read operation is succeed, false otherwise
	 */
	bool readRegisters(uint8_t startAddress, uint8_t* data, uint8_t bytes);

	/**
	 * Method to write registers of BMP280 sensor.
	 *
	 * @param address Address of the register to write
	 * @param data Value to write to the register
	 * @return true if write operation is succeed, false otherwise
	 */
	bool writeRegister(uint8_t address, uint8_t data);
};

#endif /* SRC_CBMP280SPIDRIVER_H_ */
