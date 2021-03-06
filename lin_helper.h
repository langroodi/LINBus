/*
 * lin_header.h
 *
 *  Created on: Jun 6, 2021
 *      Author: armin
 */

#ifndef LIN_HELPER_H_
#define LIN_HELPER_H_

//! \include Board Settings Header file
#include "board.h"

//! \include Standard Integer Header File
#include "fsl_lpuart.h"

//! \include GPIO Header File
#include "fsl_gpio.h"

#define BOARD_GPIO_PORT BOARD_LED_RED_GPIO
#define BOARD_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define DEBUG_LPUART LPUART0
#define DEBUG_UART_BAUDRATE 115200U
#define BOARD_LPUART LPUART1
#define BOARD_UART_BAUDRATE 19200U
#define LPUART_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv2Clk)

uint8_t syncPayload[3] = { 0x00, 0x80, 0x55 }; //!< Sync Break Payload

//! \define LIN Header Size
#define HEADER_SIZE 4
//! \define LIN Header Offset from the Sync Break Payload
#define HEADER_OFFSET HEADER_SIZE - 1
//! \define Least Significant Byte Mask
#define LSB_MASK 0x01

//! \define Transceiver Sleep Mode
#define SLEEP_MODE 0x00
//! \define Transceiver Normal Mode
#define NORMAL_MODE 0x01

//! \define LPUART Reading Successful Status
#define SUCCESSFUL_STATUS 0

//!============================================================================

void initializeLin(void); //!< Initialize
void writeHeader(uint8_t id /*!< [in] Slave ID */); //!< Write Header on the Bus
bool tryReadHeader(uint8_t* id /*!< [out] Header ID */); //!< Try to Read Header from the Bus
void writeResponse(
		uint8_t* data /*!< [in] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Write Response on the Bus
bool tryReadResponse(
		uint8_t* data /*!< [out] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Try to Read Response from the Bus

//!============================================================================

void initializeGpio(void); //!< Initialize GPIO
uint8_t applyParity(uint8_t id /*!< [in] Slave ID without Parity Bits */); //!< Apply Parity Bit on the Input ID
uint8_t getBit(
		uint8_t byte /*!< [in] Input Byte */,
		size_t offset /*!< [in] Bit Extraction Offset */); //!< Extract a Bit from a Byte
uint8_t getChecksum(
		uint8_t* data /*!< [in] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Compute Checksum based on the Input Data
bool isValidParity(uint8_t id /*!< [in] Slave ID with Parity Bits */); //!< Validate ID Parity
bool isValidChecksum(
		uint8_t* data /*!< [in] Response Data excluding Checksum */,
		size_t length /*!< [in] Data Length excluding Checksum */,
		uint8_t checksum /*!< [in] Received Checksum*/); //!< Validate Data Checksum
void setMode(uint8_t mode /*!< [in] Operation Mode */); //!< Set Transceiver Operation Mode

//!============================================================================

#endif /* LIN_HELPER_H_ */
