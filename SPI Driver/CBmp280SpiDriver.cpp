/*
 * CBmp280SpiDriver.cpp
 *
 *  Created on: Jun 23, 2024
 *      Author: humam
 */

#include "CBmp280SpiDriver.h"

CBmp280SpiDriver::CBmp280SpiDriver(SPI_HandleTypeDef *hspi, CGpioPin *ss) {
	// initialize the slave select pin
	m_ss = ss;

	// initialize the SPI handler
	m_hspi = hspi;
}

bool CBmp280SpiDriver::readRegisters(uint8_t startAddress, uint8_t *data,
		uint8_t bytes) {
	// set the slave select pin to low
	*m_ss = 0;

	// Transmit the start address to the BMP280 device. If the transmission is
	// failed set the slave select pin to high and return false
	if(HAL_SPI_Transmit(m_hspi, &startAddress, 1, HAL_MAX_DELAY) != HAL_OK){
		*m_ss = 1;
		return false;
	}

	// Receive data from the BMP280 device and store status of receive process
	bool status = HAL_SPI_Receive(m_hspi, data, bytes, HAL_MAX_DELAY) == HAL_OK;

	// set the slave select pin to high
	*m_ss = 1;

	// return true if the read operation is succeed
	return status;
}

bool CBmp280SpiDriver::writeRegister(uint8_t address, uint8_t data) {
	// prepare the control bytes (full register address without bit 7 and write
	// command)
	uint8_t controlBytes = address & 0x7F;

	// create fram with address and data
	uint8_t spiFrame [2] = {controlBytes, data};

	// set the slave select to low
	*m_ss = 0;

	// Transmit SPI frame to the BMP280 device and store the status of process
	bool status = HAL_SPI_Transmit(&hspi2, spiFrame, 2, HAL_MAX_DELAY) == HAL_OK;

	// set the slace select to high
	*m_ss = 1;

	// return true if the write operation is succeed
	return status;
}
