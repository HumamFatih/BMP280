/*
 * CLcd1602.cpp
 *
 *  Created on: Apr 30, 2024
 *      Author: humam
 */

#include "CLcd1602.h"

CLcd1602::CLcd1602(CGpioPin *rs, CGpioPin *enable, CGpioBus *data) {
	// Assigns the provided Pointer RS pin of the LCD to the attribute variable
	m_rs = rs;

	// Assigns provided Pointer Enable pin of the LCD to the attribute variable
	m_enable = enable;

	// Assigns provided Pointer data bus of the LCD to the attribute variable
	m_data = data;
}

void CLcd1602::writeNibble(uint8_t rs, uint8_t data) {
	// Set the value of RS pin on the LCD
	*m_rs = rs;

	// Set the enable pin
	*m_enable = 1;

	// Add delay. Delay is given due to speed of LCD controller is not fast enough.
	// The LCD doesn't work when this delay is removed.
	HAL_Delay(1);

	// Writes the data in the bus
	*m_data = data;

	// Reset the enable pin
	*m_enable = 0;
}

void CLcd1602::writeByte(uint8_t rs, uint8_t data) {
	// Writes the higher order 4 bits
	writeNibble(rs, (data>>4) & 0x0f);

	// Writes the lower order 4 bits
	writeNibble(rs, data & 0x0f);
}

void CLcd1602::init() {
	// Call the method to start the timer
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	// Delay after Vcc rises to 4.5V
	HAL_Delay(20);

	// Write nibble with DB4 and DB5 are set to 1 and RS = 0. The rest is 0
	writeNibble(0,0x3);

	// Delay for 5ms
	HAL_Delay(5);

	// Write nibble with DB4 and DB5 are set to 1 and RS = 0. The rest is 0
	writeNibble(0,0x3);

	// Delay for 1ms
	HAL_Delay(1);

	// Write nibble with DB4 and DB5 are set to 1 and RS = 0. The rest is 0
	writeNibble(0,0x3);

	// Write nibble with DB5 is set to 1 and RS = 0. The rest is 0
	writeNibble(0,0x2);

	// Function set : DL=0 (4 bit mode), N=1 (2 line display), F=0
	writeByte(0,0x28);

	// Display off : DB3=1, the rest is 0
	writeByte(0,0x08);

	// Display clear : DB0=1, the rest is 0
	writeByte(0,0x01);

	// Entry mode set : I/D=1 and S=0
	writeByte(0,0x06);

	// Display on/off control : D=1, C=1, B=1
	writeByte(0,0x0f);
}

void CLcd1602::write(const char *text) {
	// loops through the given character
	while(*text){
		// Writes each character to the LCD
		writeByte(1,*text++);
	}
}

void CLcd1602::setCursor(int row, int column) {
	// initialize cursor position
	int cursorPosition = 0;

	// Check if the given row is 0 or 1
	switch (row)
	{
	case 0:
		// Set the the row and column according the DDRAM address
		cursorPosition = column| 0x80;
		break;
	case 1:
		// Set the the row and column according the DDRAM address
		cursorPosition = column | 0xC0;
		break;
	default :
		// Set the default posotion
		cursorPosition = 0x80;
	}

	// Write the instruction to the LCD
	writeByte(0, cursorPosition);
}

void CLcd1602::write(const std::string& text) {
	// Iterates over each character in the text and writes each character as a
	// byte
	for (char c : text) {
		writeByte(1, c);
	}
}

