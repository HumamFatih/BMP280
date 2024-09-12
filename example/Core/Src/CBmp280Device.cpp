/*
 * CBmp280Device.cpp
 *
 *  Created on: Jun 26, 2024
 *      Author: humam
 */

#include "CBmp280Device.h"

CBmp280Device::CBmp280Device(CBmp280LowLevelDriver *driver) {
	// store the low lever driver instance
	m_driver = driver;

	// get the calibration parameters from the sensor
	getCalibration();

	// initialize the fine temperature value
	m_tFine = 0;
}

float CBmp280Device::getPreassure(){
	// get the raw pressure value
	int32_t rawPressure = getRawPressure();

	// define the variables for the pressure measurement
	int64_t var1, var2, pressure;

	// first calculation
	var1 = ((int64_t)m_tFine) - 128000;
	var2 = var1 * var1 * (int64_t)m_calibParam.P6;
	var2 = var2 + ((var1 * (int64_t)m_calibParam.P5) << 17);
	var2 = var2 + (((int64_t)m_calibParam.P4) << 35);
	var1 = ((var1 * var1 * (int64_t)m_calibParam.P3) >> 8) +
			((var1 * (int64_t)m_calibParam.P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)m_calibParam.P1) >> 33;

	// when the var1 is 0 the return pressure with 0
	if(var1 == 0){
		return 0;
	}

	// second calculation
	pressure = 1048576 - rawPressure;
	pressure = (((pressure << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)m_calibParam.P9) * (pressure >> 13) *
			(pressure >> 13)) >> 25;
	var2 = (((int64_t)m_calibParam.P8) * pressure) >> 19;

	// calculate the pressure
	pressure = ((pressure + var1 + var2) >> 8) +
			(((int64_t)m_calibParam.P7) << 4);

	// return with pressure value devided with 256.0
	return (float)(pressure / 256.0);
}

float CBmp280Device::getTemperature(){
	// get the raw temperature
	int32_t rawTemperature = getRawTemperature();

	// define the variables for calculation the temperature
	int32_t var1, var2, temperature;

	// calculate the var1
	var1 = ((((rawTemperature >> 3) - ((int32_t)m_calibParam.T1 << 1))) *
			((int32_t)m_calibParam.T2)) >> 11;

	// calculate the var2
	var2 = (((((rawTemperature >> 4) - ((int32_t)m_calibParam.T1)) *
			((rawTemperature >> 4) - ((int32_t)m_calibParam.T1))) >> 12) *
			((int32_t)m_calibParam.T3)) >> 14;

	// store the fine temperature value
	m_tFine = var1 + var2;

	// calculate the temperature
	temperature = (m_tFine * 5 + 128) >> 8;

	// return the temperature by dividing the value with 100.0
	return (float)(temperature/100.0);
}

void CBmp280Device::getAll(float &pressure, float &temperature) {
	// get the temperature and pressure
	temperature = getTemperature();
	pressure = getPreassure();
}

void CBmp280Device::setMode(OperatingMode mode, StandbyTime sbt) {
	// prepare the control register data
	uint8_t controlRegister = generateControlRegister(mode);

	// prepare the config register without filter
	uint8_t configRegister = generateConfigRegister(sbt);

	// Write to control register (0xF4)
	m_driver->writeRegister(BMP280_REG_CTR, controlRegister);

	// Write to configuration register (0xF5)
	m_driver->writeRegister(BMP280_REG_CONFIG, configRegister);
}

int32_t CBmp280Device::getRawPressure() {
    // Array buffer to store the raw pressure data
	uint8_t dataArray[3];

	// read 3 Bytes of pressure data
	m_driver->readRegisters(BMP280_REG_PRESSURE, dataArray, 3);

	// Combine bytes
	int32_t pressureData = (int32_t)((((uint32_t)dataArray[0]) << 12) |
			(((uint32_t)dataArray[1]) << 4) | (((uint32_t)dataArray[2]) >> 4));

	// return the raw pressure data
	return pressureData;
}

int32_t CBmp280Device::getRawTemperature() {
	// Array to hold raw temperature data
	uint8_t dataArray[3];

	// read 3 Bytes of temperature data
	m_driver->readRegisters(BMP280_REG_TEMPERATURE, dataArray, 3);

	// combines the bytes
	int32_t temperatureData = (int32_t)((((uint32_t)dataArray[0]) << 12) |
			(((uint32_t)dataArray[1]) << 4) | (((uint32_t)dataArray[2]) >> 4));

	// return the raw temeperature value
	return temperatureData;
}

void CBmp280Device::getCalibration() {
	// Array to hold calibration data
	uint8_t dataArray[24];

	// Read 24 bytes of calibration data (first address 0x88)
	m_driver->readRegisters(BMP280_REG_CALIBRATION, dataArray, 24);

	// combine the data from 0x88 and 0x89 and store them to T1
	m_calibParam.T1 = (uint16_t)((dataArray[1] << 8) | (dataArray[0]));

	// combine the data from 0x8A and 0x8B and store them to T2
	m_calibParam.T2 = (int16_t)((dataArray[3] << 8) | (dataArray[2]));

	// combine the data from 0x8C and 0x8D and store them to T3
	m_calibParam.T3 = (int16_t)((dataArray[5] << 8) | (dataArray[4]));

	// combine the data from 0x8E and 0x8F and store them to P1
	m_calibParam.P1 = (uint16_t)((dataArray[7] << 8) | (dataArray[6]));

	// combine the data from 0x90 and 0x91 and store them to P2
	m_calibParam.P2 = (int16_t)((dataArray[9] << 8) | (dataArray[8]));

	// combine the data from 0x92 and 0x93 and store them to P3
	m_calibParam.P3 = (int16_t)((dataArray[11] << 8) | (dataArray[10]));

	// combine the data from 0x94 and 0x95 and store them to P4
	m_calibParam.P4 = (int16_t)((dataArray[13] << 8) | (dataArray[12]));

	// combine the data from 0x96 and 0x97 and store them to P5
	m_calibParam.P5 = (int16_t)((dataArray[15] << 8) | (dataArray[14]));

	// combine the data from 0x98 and 0x99 and store them to P6
	m_calibParam.P6 = (int16_t)((dataArray[17] << 8) | (dataArray[16]));

	// combine the data from 0x9A and 0x9B and store them to P7
	m_calibParam.P7 = (int16_t)((dataArray[19] << 8) | (dataArray[18]));

	// combine the data from 0x9C and 0x9D and store them to P8
	m_calibParam.P8 = (int16_t)((dataArray[21] << 8) | (dataArray[20]));

	// combine the data from 0x9E and 0x9F and store them to P9
	m_calibParam.P9 = (int16_t)((dataArray[23] << 8) | (dataArray[22]));
}

uint8_t CBmp280Device::generateControlRegister(OperatingMode mode) {
	// Buffer for control register data
	uint8_t controlMeasurementRegister;

	// set the control register data according the operating mode
	switch (mode) {
	case Uninitialized:
		// Temp. Oversampling = skip, Press. Oversampling = skip, Power = sleep
		controlMeasurementRegister = 0;
		break;
	case UltraLowPower:
		// Temp. Oversampling = x1, Press. Oversampling = x1, Power = force
		controlMeasurementRegister = 0b00100101;
		break;
	case LowPower:
		// Temp. Oversampling = x1, Press. Oversampling = x2, Power = normal
		controlMeasurementRegister = 0b00101011;
		break;
	case StandardResolution:
		// Temp. Oversampling = x1, Press. Oversampling = x4, Power = normal
		controlMeasurementRegister = 0b00101111;
		break;
	case HighResolution:
		// Temp. Oversampling = x1, Press. Oversampling = x8, Power = normal
		controlMeasurementRegister = 0b00110011;
		break;
	case UltraHighResolution:
		// Temp. Oversampling = x2, Press. Oversampling = x16, Power = normal
		controlMeasurementRegister = 0b01010111;
		break;
	default:
		// Temp. Oversampling = skip, Press. Oversampling = skip, Power = sleep
		controlMeasurementRegister = 0;
		break;
	}

	// return the control register
	return controlMeasurementRegister;
}

uint8_t CBmp280Device::generateConfigRegister(StandbyTime time) {
	//Buffer for config register data
	uint8_t configRegister;

	// set the desired standby time
	switch (time) {
	case SBT_0_5:
		// set standby time for 0.5 ms
		configRegister = 0 << 5;
		break;
	case SBT_62_5:
		// set standby time for 62.5 ms
		configRegister = 1 << 5;
		break;
	case SBT_125:
		// set standby time for 125 ms
		configRegister = 2 << 5;
		break;
	case SBT_250:
		// set standby time for 250 ms
		configRegister = 3 << 5;
		break;
	case SBT_500:
		// set standby time for 500 ms
		configRegister = 4 << 5;
		break;
	case SBT_1000:
		// set standby time for 1000 ms
		configRegister = 5 << 5;
		break;
	case SBT_2000:
		// set standby time for 2000 ms
		configRegister = 6 << 5;
		break;
	case SBT_4000:
		// set standby time for 4000 ms
		configRegister = 7 << 5;
		break;
	}

	// return the config register
	return configRegister;
}
