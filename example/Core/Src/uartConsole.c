/*
 * uartConsole.c
 *
 *  Created on: May 19, 2024
 *      Author: humam
 */

#include "stm32f4xx_hal.h"
#include "usart.h"
#include <string.h>
#include <stdbool.h>

/**
 * This function is used to write/transmit data to UART interface.
 *
 * @param file If file = 1, Output to the console.
 * @param ptr Pointer to data to be transmitted.
 * @param len The data length to be transmitted.
 * @return the number of characters.
 */
int _write(int file, char *ptr, int len){
 	// print out test with polling
//	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
//	return len;

	// Declare static buffer with 100 characters for DMA solution
	static char textBuffer[100];

	// initialize flag maximum characters
	bool maximumChar = false;

	// Check if the length of data is exceeding 100 characters
	if(len >= 100){
		maximumChar = true;
	}else{
		maximumChar = false;
	}

	// Wait until UART state ready
	while(HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){
	}

	// copy data to 'textBuffer'
	memcpy(textBuffer, ptr, maximumChar?100:len);

	// transmit the data with DMA method and wait until transmission is complete
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)textBuffer, maximumChar?100:len);

	// return the length of data
	if(maximumChar){
		return 100;
	} else{
		return len;
	}
}

int _read(int file, char *ptr, int len) {
	// Wait for an input character.
	HAL_UART_Receive(&huart2, (uint8_t*)ptr, 1, HAL_MAX_DELAY);
	// Map return to newline which is expected as line termination by caller.
	if (*ptr == '\r') {
		*ptr = '\n';
	}
	// Echo received character, newline (special case) as CR/NL.
	if (*ptr == '\n') {
		_write(1, "\r\n", 2);
	} else {
		_write(1, ptr, 1);
	}
	// One character has been received.
	return 1;
}
