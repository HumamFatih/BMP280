/*
 * CGpioBus.cpp
 *
 *  Created on: Apr 28, 2024
 *      Author: humam
 */

#include "CGpioBus.h"

CGpioBus::CGpioBus(){
	// Assign m_pins with nullptr to create new Bus without any Pins
	m_pins = nullptr;
}

CGpioBus::CGpioBus(const CGpioBus& orig){
	// Pass over the first pin address from the original bus
	Pin* origPin = orig.m_pins;

	// Initialize previous new pin pointer with nullptr
	Pin* prevNewPin = nullptr;

	// Deep copy every Pin in the original bus
	while(origPin != nullptr){
		// Create new Pin and assign attribute of original Pin in the new Pin
		Pin* newPin = new Pin;
		newPin->port = origPin->port;
		newPin->mask = origPin->mask;
		newPin->next = nullptr;

		// Check if the previous new pointer is nullptr, otherwise assign the
		// newPin in the previous new pin
		if(prevNewPin == nullptr){
			m_pins = newPin;
		}else{
			prevNewPin->next = newPin;
		}

		// Assign the previous new Pin with new Pin
		prevNewPin = newPin;

		// Assign the address of the next Pin
		origPin = origPin->next;
	}
}

CGpioBus& CGpioBus::operator =(const CGpioBus& src){
	// Pass over the first pin address to the pointer currentPin
	Pin* currentPin = m_pins;

	// Deallocated all pins in the linked list before making deep copy
	while(currentPin != nullptr){
		// Pass the next pin address before deleting the current pin
		Pin* nextPin = currentPin->next;

		// delete the pin and pass the address of the next pin
		delete currentPin;
		currentPin = nextPin;
	}

	// set to nullptr to avoid hanging pointer
	m_pins = nullptr;

	// Checking for self assignment
	if (this != &src) {
		// Pass over the first pin address from the source bus and Initialize
		// previous new pin pointer with nullptr
		Pin* srcPin = src.m_pins;
		Pin* prevNewPin = nullptr;

		// Deep copy every Pin in the original bus
		while(srcPin != nullptr){
			// Create new Pin and assign attribute of original Pin in new Pin
			Pin* newPin = new Pin;
			newPin->port = srcPin->port;
			newPin->mask = srcPin->mask;
			newPin->next = nullptr;

			// Check if the previous new pointer is nullptr, otherwise assign
			// the newPin in the previous new pin
			if(prevNewPin == nullptr){
				m_pins = newPin;
			}else{
				prevNewPin->next = newPin;
			}

			// Assign the previous new Pin with new Pin and assign the address
			// of the next Pin
			prevNewPin = newPin;
			srcPin = srcPin->next;
		}
	}

	// return to the current object
	return *this;
}

CGpioBus::~CGpioBus(){
	// Pass over the first pin address to the pointer currentPin
	Pin* currentPin = m_pins;

	// Delete all pins in the linked list
	while(currentPin != nullptr){
		// Pass the next pin address before deleting the current pin
		Pin* nextPin = currentPin->next;

		// delete the pin and pass the address of the next pin
		delete currentPin;
		currentPin = nextPin;
	}

	// set to nullptr to avoid hanging pointer
	m_pins = nullptr;
}

CGpioBus& CGpioBus::addPin(GPIO_TypeDef* port, uint16_t pinMask){
	// Allocate memory to create a new Pin
	Pin* newPin = new Pin;

	// Assign the parameter in the member of the struct Pin. The member next is
	// assigned for the first time with nullptr
	newPin->port = port;
	newPin->mask = pinMask;
	newPin->next = nullptr;

	// Check if there is no pin the bus yet, otherwise find the last pin and
	// assign the new pin to its next
	if(m_pins == nullptr){
		m_pins = newPin;
	}else{
		Pin* currentPin = m_pins;

		// find the last pin and assign the new pin to its next
		while(currentPin->next != nullptr){
			currentPin = currentPin->next;
		}
		currentPin->next = newPin;
	}

	// return to the current object CGpioBus
	return *this;
}

CGpioBus& CGpioBus::write(uint16_t value){
	// Assign the first pin address in pointer current pin
	Pin* currentPin = m_pins;

	// Initialize the bitShift with 0
	int bitShift = 0;

	// Go through all pin the bus and set/reset its value
	while(currentPin != nullptr){
		// Do logic operation to get the value for each pin
		bool on = (value >> bitShift) & 0x1;

		// set or reset the pin according its value
		HAL_GPIO_WritePin(currentPin->port, currentPin->mask, on ? GPIO_PIN_SET
				: GPIO_PIN_RESET);

		// Jump to the next pin
		currentPin = currentPin->next;

		// Increment the bitShift
		bitShift++;
	}

	// return to the current object
	return *this;
}

CGpioBus& CGpioBus::operator =(uint16_t value){
	// call write method
	write(value);

	// return to the current object
	return *this;
}

uint16_t CGpioBus::read()const{
	// Initialize helper variable value and bitPosition with 0
	int value = 0;
	int bitPosition = 0;

	// Create new pointer to first pin address
	Pin* currentPin = m_pins;

	// Go through each pin and read the value on the pin
	while(currentPin != nullptr){
		// Check if pin value in the input data register is set "1" with help of
		// the pin mask. Then set the helper variable "value" according the
		// position of pin in the bus
		if(currentPin->mask & currentPin->port->IDR){
			value = value | (1 << bitPosition);
		}

		// jump to next pin address and increment the bit position accordingly
		currentPin = currentPin->next;
		bitPosition = bitPosition + 1;
	}

	// return the new value
	return value;
}

CGpioBus::operator uint16_t() const{
	// return by calling read() method
	return read();
}
