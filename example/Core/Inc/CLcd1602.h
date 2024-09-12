/*
 * CLcd1602.h
 *
 *  Created on: Apr 30, 2024
 *      Author: humam
 */

#ifndef SRC_CLCD1602_H_
#define SRC_CLCD1602_H_
#include "tim.h"
#include "CGpioPin.h"
#include "CGPioBus.h"
#include <string>

/**
 * Models a class for controlling a 16x2 LCD (Liquid Crystal Display).
 */
class CLcd1602 {
private :
	/** Pointer to the GPIO pin connected to the RS pin of the LCD. */
	CGpioPin* m_rs;
	/** Pointer to the GPIO pin connected to the Enable pin of the LCD. */
	CGpioPin* m_enable;
	/** Pointer to the GPIO bus connected to the Data pin of the LCD. */
	CGpioBus* m_data;

	/**
	 * Write 4 bits nibble to the LCD.
	 *
	 * @param rs The value to be written in RS pin (0: instruction, 1: data)
	 * @param data The 4 bits data to be written in Data pin
	 */
	void writeNibble(uint8_t rs, uint8_t data);

	/**
	 * Write 8 bits to the LCD. First, write the higher order 4 bits and then
	 * write lower order 4 bits.
	 *
	 * @param rs The value to be written in RS pin (0: instruction, 1: data)
	 * @param data The 8 bits data to be written in Data pin
	 */
	void writeByte(uint8_t rs, uint8_t data);

public:
	/**
	 * Constructor for the class CLcd1602.
	 *
	 * @param rs Pointer to the GPIO pin connected to the RS pin of the LCD.
	 * @param enable Pointer to the GPIO pin connected to the Enable pin of the
	 * LCD.
	 * @param data Pointer to the GPIO bus connected to the Data pin of the LCD.
	 */
	CLcd1602(CGpioPin* rs, CGpioPin* enable, CGpioBus* data);

	/**
	 * Initialize the LCD configuration by configuring necessary settings.
	 */
	void init();

	/**
	 * Writes text to the LCD.
	 *
	 * @param text Text/sentence to be written in LCD.
	 */
	void write(const char* text);

	/**
	 * Sets the position of cursor on the LCD
	 *
	 * @param row Row number (0 or 2)
	 * @param column Column
	 */
	void setCursor(int row, int column);

	/**
	 * Writes text to the LCD with string.
	 *
	 * @param text Text/sentence to be written in LCD:
	 */
	void write(const std::string& text);
};

#endif /* SRC_CLCD1602_H_ */
