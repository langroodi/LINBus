/*
 * lin_helper.c
 *
 *  Created on: Jun 6, 2021
 *      Author: armin
 */

//! \include LIN Bus Helper Header
#include "lin_helper.h"

void initializeLin(void) {
    lpuart_config_t config;
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = DEFAULT_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLK_FREQ);

    initializeGpio();

    uint8_t txbuff[] = "Initialization is done.\r\n";
    LPUART_WriteBlocking(DEMO_LPUART, txbuff, sizeof(txbuff) - 1);
}

void initializeGpio(void) {
    // Define the initial state of the GPIO pin
    gpio_pin_config_t _Ledconfig = {
        kGPIO_DigitalOutput, 0,
    };

    // Initialize the GPIO pin
    GPIO_PinInit(BOARD_GPIO_PORT, BOARD_GPIO_PIN, &_Ledconfig);
}

void writeHeader(uint8_t id) {
	setMode(NORMAL_MODE);
	LPUART_WriteBlocking(DEMO_LPUART, syncPayload, 1);
	LPUART_WriteBlocking(DEMO_LPUART, syncPayload + 1, 1);
	LPUART_WriteBlocking(DEMO_LPUART, syncPayload + 2, 1);
	syncPayload[HEADER_OFFSET] = applyParity(id);
	LPUART_WriteBlocking(DEMO_LPUART, syncPayload + HEADER_OFFSET, 1);
	setMode(SLEEP_MODE);
}

 bool tryReadHeader(uint8_t* id) {
	uint8_t _header[HEADER_SIZE];
	status_t _status = LPUART_ReadBlocking(DEMO_LPUART, _header, HEADER_SIZE);

	if (_status == SUCCESSFUL_STATUS) {
		uint8_t _id = _header[HEADER_OFFSET];
		bool _validParity = isValidParity(_id);

		if (_validParity) {
			*id = _id;

			return true;
		}
	}

	return false;
}

void writeResponse(uint8_t* data, size_t length) {
	setMode(NORMAL_MODE);
	uint8_t _checksum = getChecksum(data, length);
	LPUART_WriteBlocking(DEMO_LPUART, data, length);
	LPUART_WriteBlocking(DEMO_LPUART, &_checksum, 1);
	setMode(SLEEP_MODE);
}

uint8_t getChecksum(uint8_t* data, size_t length) {
	uint8_t _result = 0;
	for (size_t i = 0; i < length; i++) {
		_result += data[i];
	}

	return ~_result;
}

bool isValidParity(uint8_t id) {
	uint8_t _expectedId = applyParity(id);
	bool _result = _expectedId == id;

	return _result;
}

void setMode(uint8_t mode) {
	if (mode == SLEEP_MODE) {
		GPIO_ClearPinsOutput(BOARD_GPIO_PORT, 1u << BOARD_GPIO_PIN);
	}
	else {
		GPIO_SetPinsOutput(BOARD_GPIO_PORT, 1u << BOARD_GPIO_PIN);
	}
}

uint8_t applyParity(uint8_t id) {
	uint8_t _bit0 = getBit(id, 0);
	uint8_t _bit1 = getBit(id, 1);
	uint8_t _bit2 = getBit(id, 2);
	uint8_t _bit3 = getBit(id, 3);
	uint8_t _bit4 = getBit(id, 4);
	uint8_t _bit5 = getBit(id, 5);

	uint8_t _parity0 = _bit0 ^ _bit1 ^ _bit2 ^ _bit4;
	uint8_t _parity1 = ~(_bit1 ^ _bit3 ^ _bit4 ^ _bit5);
	uint8_t _parity = (_parity1 << 7) | (_parity0 << 6);

	uint8_t _result = _parity | id;
	return _result;
}

uint8_t getBit(uint8_t byte, size_t offset) {
	uint8_t _result = byte >> offset;
	_result &= LSB_MASK;

	return _result;
}

