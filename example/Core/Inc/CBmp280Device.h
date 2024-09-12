/*
 * CBmp280Device.h
 *
 *  Created on: Jun 26, 2024
 *      Author: humam
 */

#ifndef SRC_CBMP280DEVICE_H_
#define SRC_CBMP280DEVICE_H_
#include "CBmp280LowLevelDriver.h"
#include <iostream>

// Define the first address of Registers
#define BMP280_REG_CTR 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_PRESSURE 0xF7
#define BMP280_REG_TEMPERATURE 0xFA
#define BMP280_REG_CALIBRATION 0x88

using namespace std;

/**
 * Models a class for measuring pressure and temperature with BMP280 Sensor.
 */
class CBmp280Device {
private :
	/** Pointer to low level driver. */
	CBmp280LowLevelDriver* m_driver;

	/** Fine temperature value used for pressure compensation. */
	int32_t m_tFine;

	/** Struct to store the calibration parameters from the sensor. */
	struct calibrationParam{
		uint16_t T1;
		int16_t T2;
		int16_t T3;
		uint16_t P1;
		int16_t P2;
		int16_t P3;
		int16_t P4;
		int16_t P5;
		int16_t P6;
		int16_t P7;
		int16_t P8;
		int16_t P9;
	} m_calibParam;

public:
	/** Enumeration for the operating mode of the sensor. */
	enum OperatingMode{HighResolution, LowPower, StandardResolution,
		UltraHighResolution, UltraLowPower, Uninitialized};

	/** Enumeration for the standby times between measurement. */
	enum StandbyTime{SBT_0_5, SBT_1000, SBT_125, SBT_2000, SBT_250, SBT_4000,
		SBT_500, SBT_62_5};

	/**
	 * Constructor for Bmp280Device.
	 *
	 * @param driver Pointer to a low level driver instance
	 */
	CBmp280Device(CBmp280LowLevelDriver* driver);

	/**
	 * Get the current pressure reading from the sensor.
	 *
	 * @return Pressure in Pa
	 */
	float getPreassure();

	/**
	 * Get the current temperature reading from the sensor
	 *
	 * @return Temperature in Celcius
	 */
	float getTemperature();

	/**
	 * Get both pressure and temperature readings from the sensor. Temperature
	 * will be measure first.
	 *
	 * @param pressure Reference to store the pressure value
	 * @param temperature Reference to store the temperature value
	 */
	void getAll(float& pressure, float& temperature);

	/**
	 * Set the operating mode and standby time for the sensor.
	 *
	 * @param mode Desired operating mode
	 * @param sbt Desired standby time
	 */
	void setMode(OperatingMode mode, StandbyTime sbt);

	/**
	 * Get the raw pressure data from the sensor.
	 *
	 * @return raw pressure data
	 */
	int32_t getRawPressure();

	/**
	 * Get the raw temperature data from the sensor.
	 *
	 * @return raw temperature data
	 */
	int32_t getRawTemperature();

	/**
	 * read the calibration parameters from the sensor.
	 */
	void getCalibration();

	/**
	 *  Generates the control register value based on the operating mode.
	 *
	 *  @param mode The operating mode of the sensor
	 *  @return The generated control register value
	 */
	uint8_t generateControlRegister(OperatingMode mode);

	/**
	 * Generates the config register value based on mode and standby time.
	 *
	 * @param mode The operating mode of the sensor
	 * @param time The standby time
	 * @return uint8_t The generated config register value
	 */
	uint8_t generateConfigRegister(StandbyTime time);
};

#endif /* SRC_CBMP280DEVICE_H_ */
