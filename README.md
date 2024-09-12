# BMP280 Sensor Driver (SPI and I2C)

## Overview
This project provides a driver for the BMP280 barometric pressure sensor, offering support for both SPI and I2C communication interfaces. The drivers enable easy interaction with the BMP280 sensor using STM32 HAL libraries. The drivers allow reading and writing sensor registers, enabling temperature and pressure measurements.

## Features
- **I2C Interface**: A driver for communicating with the BMP280 sensor over the I2C protocol.
- **SPI Interface**: A driver for communicating with the BMP280 sensor over the SPI protocol.
- **Register Read/Write**: Support for reading and writing registers on the BMP280 sensor.
- **Extensible**: Easily integrates with STM32 HAL libraries and GPIO configurations.

## Class Descriptions

### CBmp280I2cDriver
This class provides an I2C-based communication interface to interact with the BMP280 sensor.

#### Public Methods:
- **`CBmp280I2cDriver(I2C_HandleTypeDef* hi2c, uint8_t deviceAddress)`**: Constructor for the I2C driver.
- **`bool readRegisters(uint8_t startAddress, uint8_t* data, uint8_t bytes)`**: Reads multiple bytes from the sensor starting at the specified register.
- **`bool writeRegister(uint8_t address, uint8_t data)`**: Writes a byte to a specific register of the BMP280 sensor.

### CBmp280SpiDriver
This class provides an SPI-based communication interface for the BMP280 sensor.

#### Public Methods:
- **`CBmp280SpiDriver(SPI_HandleTypeDef* hspi, CGpioPin* ss)`**: Constructor for the SPI driver.
- **`bool readRegisters(uint8_t startAddress, uint8_t* data, uint8_t bytes)`**: Reads multiple bytes from the BMP280 sensor starting from a given register.
- **`bool writeRegister(uint8_t address, uint8_t data)`**: Writes a value to the sensor's register.

## Installation

### Prerequisites
- **STM32 HAL Libraries**: Your project must include the STM32 HAL libraries (`i2c.h`, `spi.h`, `gpio.h`).
- **BMP280 Sensor**: Ensure you have a BMP280 sensor connected via either the SPI or I2C interface.
- **CGpioPin Class**: A GPIO helper class to control GPIO pins used for slave select (in the case of SPI).

### Steps
1. Clone the repository and include the `CBmp280I2cDriver` and `CBmp280SpiDriver` header and source files in your project.
2. Make sure to properly initialize the I2C or SPI peripherals using STM32CubeMX or manually in your initialization code.
3. Connect your BMP280 sensor to the appropriate pins for either SPI or I2C.
4. Configure the GPIO pins used for slave select (SPI only) or I2C communication.

