/*
 * CSensorDataDisplay.h
 *
 *  Created on: Jun 27, 2024
 *      Author: humam
 */

#ifndef SRC_CSENSORDATADISPLAY_H_
#define SRC_CSENSORDATADISPLAY_H_
#include "CBmp280Device.h"
#include "CGpioPin.h"
#include "CBmp280I2cDriver.h"
#include <iostream>
#include "i2c.h"
#include <string.h>
#include "spi.h"
#include "CBmp280SpiDriver.h"
#include "CGpioBus.h"
#include "CLcd1602.h"

using namespace std;

/**
 * Models an application class for the test of BMP280 driver.
 */
class CSensorDataDisplay {
public:
	/**
	 * In this method, temperature and pressure calculation tests are carried
	 * out using the BMP280 sensor. The sensor will be run with several
	 * different modes and standby times.
	 */
	void run();
};

/**
 * Defines an external C function 'runSensorDataDisplay()'
 */
extern "C" void runSensorDataDisplay() {
	CSensorDataDisplay().run();
};

#endif /* SRC_CSENSORDATADISPLAY_H_ */
