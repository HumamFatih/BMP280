/*
 * CSensorDataDisplay.cpp
 *
 *  Created on: Jun 27, 2024
 *      Author: humam
 */

#include "CSensorDataDisplay.h"

void CSensorDataDisplay::run() {
	// Address of BMP280
	const uint16_t bmp280Addr = 0x76;

	// Slave select pin
	CGpioPin csb(GPIOB, 1 << 12);

	// Create I2c and SPI drivers for BMP280
	CBmp280I2cDriver I2c(&hi2c1, bmp280Addr);
	CBmp280SpiDriver Spi(&hspi2, &csb);

	// Create BMP280 device object for I2C (device1) and SPI (device2)
	CBmp280Device device1(&I2c);
	CBmp280Device device2(&Spi);

	// Creates pin and bus object which control the LCD
	CGpioPin lcdRs(GPIOD, 1<<2);
	CGpioPin lcdE(GPIOC, 1<<9);
	CGpioBus lcdBus = CGpioBus().addPin(GPIOA, 1<<11).addPin(GPIOA, 1<<12)
			.addPin(GPIOB, 1<<1).addPin(GPIOB, 1<<2);

	// Creates LCD object
	CLcd1602 lcd(&lcdRs, &lcdE, &lcdBus);

	// Lcd initialization
	lcd.init();

	// Set the mode for I2C Ultra low power with standby time 0.5 ms
//	device1.setMode(CBmp280Device::UltraLowPower, CBmp280Device::SBT_0_5);

	// Set the mode for SPI Ultra low power with standby time 0.5 ms
//	device2.setMode(CBmp280Device::UltraLowPower, CBmp280Device::SBT_0_5);

	// Set the mode for I2C Ultra High Resolution with standby time 1s
//	device1.setMode(CBmp280Device::UltraHighResolution,CBmp280Device::SBT_1000);

	// Set the mode for SPI Ultra High Resolution with standby time 1s
	device2.setMode(CBmp280Device::UltraHighResolution,CBmp280Device::SBT_1000);

	// Defines the variable to store the pressure and temperature
	float pressure, temperature;

	// Test for ultra low power
//	device1.getAll(pressure, temperature);
//	device2.getAll(pressure, temperature);

	// Print temperature to the LCD (for Ultra Low Power Mode)
//	lcd.setCursor(0, 0);
//	lcd.write("T : " + to_string(temperature).substr(0, 5) + char(223) + "C");
//	lcd.setCursor(1, 0);
//	lcd.write("P : " + to_string(pressure).substr(0, 8) + " Pa");

	while(true){
		// read the pressure and temperature I2C
//		device1.getAll(pressure, temperature);

		// read the pressure and temperature SPI
		device2.getAll(pressure, temperature);

		// Print the temperature in the first row of LCD
		lcd.setCursor(0, 0);
		lcd.write("T : " + to_string(temperature).substr(0, 5) + char(223) + "C");

		// Print the pressure in the second row of LCD
		lcd.setCursor(1, 0);
		lcd.write("P : " + to_string(pressure).substr(0, 8) + " Pa");
	}
}
