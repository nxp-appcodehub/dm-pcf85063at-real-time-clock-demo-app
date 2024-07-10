/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! File: frdm_stbi_PCF85063AT_shield.h
* @brief The frdm_stbi_PCF85063AT_shield.h file declares arduino pin mapping for frdm_stbi_PCF85063AT_shield expansion board.
*/

#ifndef _FRDM_PCF85063AT_SHIELD_H_
#define _FRDM_PCF85063AT_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-PCF85063AT"

// PCF85063AT RTC Information

#define PCF85063AT_CS       D10
#define PCF85063AT_MOSI     D11
#define PCF85063AT_MISO     D12
#define PCF85063AT_SCLK     D13

#define PCF85063AT_I2C_ADDR 0x51

#endif /* _FRDM_PCF85063AT_SHIELD_H_ */
