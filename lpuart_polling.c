/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lpuart.h"

#include "pin_mux.h"
#include "clock_config.h"

//! \include LIN Bus Helper Header File
#include "lin_helper.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    initializeLin();

    writeHeader(0x11);

    while (1) {
        asm("NOP");
    }
}
