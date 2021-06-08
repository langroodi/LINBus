/*
 * lin_header.h
 *
 *  Created on: Jun 6, 2021
 *      Author: armin
 */

#ifndef LIN_HELPER_H_
#define LIN_HELPER_H_

//! \include Standard Integer Header File
#include "fsl_lpuart.h"

#define DEMO_LPUART LPUART0
#define DEMO_LPUART_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv2Clk)
#define DEFAULT_UART_BAUDRATE 115200U

uint8_t syncPayload[4] = { 0x00, 0x80, 0x55, 0x00 }; //!< Sync Break Payload

//! \define LIN Header Offset from the Sync Break Payload
#define HEADER_OFFSET 3
//! \define Least Significant Byte Mask
#define LSB_MASK 0x01

//!============================================================================

void initializeLin(void); //!< Initialize
void writeHeader(uint8_t id /*!< [in] Slave ID */); //!< Write Header on the Bus
uint8_t readHeader(); //!< Read Header from the Bus
void writeResponse(
		uint8_t* data /*!< [in] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Write Response on the Bus
uint8_t readResponse(
		uint8_t* data /*!< [out] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Read Response from the Bus

//!============================================================================

uint8_t applyParity(uint8_t id /*!< [in] Slave ID without Parity Bits */); //!< Apply Parity Bit on the Input ID
uint8_t getBit(
		uint8_t byte /*!< [in] Input Byte */,
		size_t offset /*!< [in] Bit Extraction Offset */); //!< Extract a Bit from a Byte
uint8_t getChecksum(
		uint8_t* data /*!< [in] Response Data */,
		size_t length /*!< [in] Data Length */); //!< Compute Checksum based on the Input Data
uint8_t isValidParity(uint8_t id /*!< [in] Slave ID with Parity Bits */); //!< Validate ID Parity
uint8_t isValidChecksum(
		uint8_t* data /*!< [in] Response Data including Checksum */,
		size_t length /*!< [in] Data Length including Checksum */); //!< Validate Data Checksum
void setSleepMode(uint8_t mode /*!< [in] Sleep Mode */); //!< Set Sleep Mode

//!============================================================================

#endif /* LIN_HELPER_H_ */
