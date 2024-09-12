/*
 * CBmp280I2cDriver.cpp
 *
 *  Created on: Jun 22, 2024
 *      Author: humam
 */

#include "CBmp280I2cDriver.h"

CBmp280I2cDriver::CBmp280I2cDriver(I2C_HandleTypeDef *hi2c,
		uint8_t deviceAddress) {
	// initialize the I2C handler
	m_hi2c = hi2c;

	// initialize the I2C device address
	m_deviceAddress = deviceAddress;
}

bool CBmp280I2cDriver::readRegisters(uint8_t startAddress, uint8_t *data,
		uint8_t bytes) {
	// transmit the start address to the BMP280 device, return false if the
	// transmission is failed
	if(HAL_I2C_Master_Seq_Transmit_DMA(m_hi2c, m_deviceAddress << 1,
			&startAddress, 1, I2C_FIRST_FRAME) != HAL_OK){
		return false;
	}

	// Wait until the I2C bus ready
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	// Receive the data from the BMP280 device and return false if the receive
	// is failed
	if(HAL_I2C_Master_Seq_Receive_DMA(m_hi2c, m_deviceAddress << 1, data, bytes,
			I2C_LAST_FRAME) != HAL_OK){
		return false;
	}

	// Wait until the I2C bus is ready
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	// return true if the read operation is succeed
	return true;
}

bool CBmp280I2cDriver::writeRegister(uint8_t address, uint8_t data) {
	// I2C frame to store address and data
	uint8_t frameI2c [2] = {address, data};

	// Transmit the address and data to the BMP280 sensor and return false if
	// the transmission is failed
	if(HAL_I2C_Master_Transmit(m_hi2c, m_deviceAddress << 1,
			frameI2c, 2, HAL_MAX_DELAY) != HAL_OK){
		return false;
	}

	// return true if write operation is succeed
	return true;
}
