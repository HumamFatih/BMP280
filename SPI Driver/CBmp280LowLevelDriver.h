/*
 * CBmp280LowLevelDriver.h
 *
 *  Created on: May 23, 2021
 *      Author: mnl
 */

#ifndef SRC_CBMP280LOWLEVELDRIVER_H_
#define SRC_CBMP280LOWLEVELDRIVER_H_

#include <sys/types.h>

/**
 * Definiert die Schnittstelle zu den Low-Level-Treibern, mit denen der
 * BMP280 angesteuert werden kann.
 */
class CBmp280LowLevelDriver {
public:
	/**
	 * Schreibt einen Wert in ein Register des BMP280.
	 *
	 * @param address Adresse des Registers, in das der Wert
	 * 	geschrieben werden soll
	 * @param data der in das Register zu schreibende Wert
	 * @return true, wenn der Vorgang erfolgreich war
	 */
	virtual bool writeRegister(uint8_t address, uint8_t data) = 0;

	/**
	 * Liste die Werte aus einer Folge von Registern in den übergebenen
	 * Puffer. Implementierungen dieser Schnittstelle müssen das Lesen
	 * als Burst-Read (vergl. Datenblatt) implementieren.
	 *
	 * @param startAddress Adresse des als erstes zu lesenden Registers
	 * @param data Zeiger auf den Puffer, in den die gelesenen Daten
	 *  geschrieben werden sollen
	 * @param bytes Anzahl zu lesender Wert (d.h. Register)
	 * @return true, wenn der Vorgang erfolgreich war
	 */
	virtual bool readRegisters(
			uint8_t startAddress, uint8_t* data, uint8_t bytes) = 0;
};

#endif /* SRC_CBMP280LOWLEVELDRIVER_H_ */
