/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file PCF85063AT.h
 * @brief This file contains the PCF85063AT RTC register definitions, access macros, and
 * device access functions.
 */
#ifndef PCF85063AT_H_
#define PCF85063AT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/**
 **
 **  @brief The PCF85063AT RTC Internal Register Map.
 */
enum
{
	PCF85063AT_CTRL1 = 0x00,
	PCF85063AT_CTRL2 = 0x01,
	PCF85063AT_OFFSET = 0x02,
	PCF85063AT_RAM_BYTE = 0x03,
	PCF85063AT_SECOND = 0x04,
	PCF85063AT_MINUTE = 0x05,
	PCF85063AT_HOUR = 0x06,
	PCF85063AT_DAY = 0x07,
	PCF85063AT_WEEKDAY = 0x08,
	PCF85063AT_MONTH = 0x09,
	PCF85063AT_YEAR = 0x0A,
	PCF85063AT_SECOND_ALARM = 0x0B,
	PCF85063AT_MINUTE_ALARM = 0x0C,
	PCF85063AT_HOUR_ALARM = 0x0D,
	PCF85063AT_DAY_ALARM = 0x0E,
	PCF85063AT_WEEKDAY_ALARM = 0x0F,
	PCF85063AT_TIMER_VALUE = 0x10,
	PCF85063AT_TIMER_MODE = 0x11,
};



#define PCF85063AT_TBOOT_MAX  2                 /*!< Maximum safe value for TBOOT1/2 in ms (1ms, 2ms)= ~2ms. */
#define PCF85063AT_REG_SIZE_BYTE  (1)           /*!< size of register of PCF85063AT in byte. */


/*--------------------------------
 ** Register: Control_1
 ** Enum: PCF85063AT_CTRL1
 ** --
 ** Offset : 0x00 control and status register 1.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t cap_sel : 1; /*  internal oscillator capacitor selection 0- 7pf, 1- 12.5pf */

		uint8_t mode_12_24 : 1; /*  0-24 hour mode selected 1- 12 hour mode selected */

		uint8_t cie : 1; /*  0-no corrrection interrupt generated 1-disabled */

		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)*/

		uint8_t sr : 1; /*  0- no software reset 1- software reset */

		uint8_t stop : 1; /*  0- RTC source Clock runs 1- RTC clock is stopped */

		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)*/

		uint8_t ext_test : 1; /*  0- Normal Mode 1- External Clock Test Mode. */
	} b;
	uint8_t w;
} PCF85063AT_CTRL_1;

/*
 * Control_1 - Bit field mask definitions
 */
#define PCF85063AT_CTRL1_EXT_TEST_MASK   ((uint8_t)0x80)
#define PCF85063AT_CTRL1_EXT_TEST_SHIFT  ((uint8_t)7)

#define PCF85063AT_CTRL1_START_STOP_MASK       ((uint8_t)0x20)
#define PCF85063AT_CTRL1_START_STOP_SHIFT      ((uint8_t)5)

#define PCF85063AT_CTRL1_SR_MASK ((uint8_t)0x10)
#define PCF85063AT_CTRL1_SR_SHIFT ((uint8_t)4)

#define PCF85063AT_CTRL1_CIE_MASK    ((uint8_t)0x04)
#define PCF85063AT_CTRL1_CIE_SHIFT   ((uint8_t)2)

#define PCF85063AT_CTRL1_12_HOUR_24_HOUR_MODE_MASK ((uint8_t)0x02)
#define PCF85063AT_CTRL1_12_HOUR_24_HOUR_MODE_SHIFT ((uint8_t)1)

#define PCF85063AT_CTRL1_CAP_SEL_MASK          ((uint8_t)0x01)
#define PCF85063AT_CTRL1_CAP_SEL_SHIFT         ((uint8_t)0)

/*--------------------------------
 ** Register: Control_2
 ** Enum: PCF85063AT_CTRL2
 ** --
 ** Offset : 0x01 control and status register 2.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t _cof : 3; /* clockout control*/

		uint8_t tf : 1; /* timer flag interrupt 0-disabled, 1-enabled */

		uint8_t _hmi : 1;/*  half minute interrupt 0-disabled, 1-enabled */

		uint8_t _mi : 1; /*  minute interrupt 0-disabled, 1-enabled */

		uint8_t af : 1; /*alarm flag 0-disabled, 1-enabled*/

		uint8_t _aie : 1; /* 0- No Alarm Interrupt Triggered 1- Flag Set When Alarm Triggered (flag must be cleared to clear interrupt)*/

	} b;
	uint8_t w;
} PCF85063AT_CTRL_2;

/*
 * Control_2 - Bit field mask definitions
 */

#define PCF85063AT_CTRL2_AIE_MASK        ((uint8_t)0x80)
#define PCF85063AT_CTRL2_AIE_SHIFT       ((uint8_t)7)

#define PCF85063AT_CTRL2_AF_MASK         ((uint8_t)0x40)
#define PCF85063AT_CTRL2_AF_SHIFT        ((uint8_t)6)

#define PCF85063AT_CTRL2_MI_MASK        ((uint8_t)0x20)
#define PCF85063AT_CTRL2_MI_SHIFT       ((uint8_t)5)

#define PCF85063AT_CTRL2_HMI_MASK        ((uint8_t)0x10)
#define PCF85063AT_CTRL2_HMI_SHIFT       ((uint8_t)4)

#define PCF85063AT_CTRL2_TF_MASK        ((uint8_t)0x08)
#define PCF85063AT_CTRL2_TF_SHIFT       ((uint8_t)3)

#define PCF85063AT_CTRL2_COF_MASK       ((uint8_t)0x07)
#define PCF85063AT_CTRL2_COF_SHIFT      ((uint8_t)0)




/*--------------------------------
 ** Register: OFFSET
 ** Enum: PCF85063AT_OFFSET.
 ** --
 ** Offset : 0x02 OFFSET REGISTER.
 ** ------------------------------*/
//typedef uint8_t PCF85063AT_OFFSET;      /*  */

typedef union
{
	struct
	{
		uint8_t offset : 7;  /*offset value-+*/
		uint8_t offset_mode : 1;      /* 0- Clock integrity is guaranteed 1- Clock integrity is not guaranteed*/
	}b;
	uint8_t w;
}PCF85063AT_OFFSET_MODE;
/*
 * Offset - Bit field mask definitions
 */
#define PCF85063AT_OFFSET_MASK ((uint8_t)0x7F)
#define PCF85063AT_OFFSET_SHIFT ((uint8_t)0)

#define PCF85063AT_OFFSET_MODE_MASK ((uint8_t)0x80)
#define PCF85063AT_OFFSET_MODE_SHIFT ((uint8_t)7)

/*--------------------------------
 ** Register: RAM bytes
 ** Enum: RAM_BYTES.
 ** --
 ** Offset : 0x03 RAM byte register.
 ** ------------------------------*/
typedef uint8_t PCF85063AT_RAM_BYTES; /* RAM bytes */

/*
 * RAM Bytes - Bit field mask definitions
 */
#define PCF85063AT_RAM_BYTE_MASK    ((uint8_t)0xFF)
#define PCF85063AT_RAM_BYTE_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Seconds
 ** Enum: PCF85063AT_SECOND.
 ** --
 ** Offset : 0x04 Seconds register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t seconds : 7;  /* Actual seconds coded in BCD format (0-59)*/
		uint8_t os : 1;      /* 0- Clock integrity is guaranteed 1- Clock integrity is not guaranteed*/
	} b;
	uint8_t w;
} PCF85063AT_SECONDS;

/*
 *  Seconds - Bit field mask definitions
 */
#define PCF85063AT_OS_MASK    ((uint8_t)0x80)
#define PCF85063AT_OS_SHIFT    ((uint8_t)7)

#define PCF85063AT_SECONDS_MASK    ((uint8_t)0x7F)
#define PCF85063AT_SECONDS_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Minutes
 ** Enum: PCF85063AT_MINUTES.
 ** --
 ** Offset : 0x05 Minutes register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t minutes : 7; /* Actual minutes coded in BCD format (0-59)  */
		uint8_t _reserved_ : 1; /* Reserved Bit (Unused) */
	} b;
	uint8_t w;
} PCF85063AT_MINUTES;

/*
 * Minutes - Bit field mask definitions
 */
#define PCF85063AT_MINUTES_MASK    ((uint8_t)0x7F)
#define PCF85063AT_MINUTES_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Hours
 ** Enum: PCF85063AT_HOURS.
 ** --
 ** Offset : 0x06 Hours register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t hours : 6 ;         /* Actual hours coded in BCD format when in 12/24-hour mode*/
		uint8_t _reserved_1 : 1;    /* Reserved Bit (Unused)  */
		uint8_t _reserved_2 : 1;    /* Reserved Bit (Unused) */
	} b;
	uint8_t w;
} PCF85063AT_HOURS;

/*
 * Hours - Bit field mask definitions
 */
#define PCF85063AT_HOURS_MASk_24H    ((uint8_t)0x3F)
#define PCF85063AT_HOURS_MASK_12H    ((uint8_t)0x1F)

#define PCF85063AT_HOURS_SHIFT       ((uint8_t)0)
#define PCF85063AT_AM_PM_SHIFT       ((uint8_t)5)

/*--------------------------------
 ** Register: Days
 ** Enum: PCF85063AT_DAYS.
 ** --
 ** Offset : 0x07 Days register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t days: 6; /* Actual day coded in BCD format (1-31)  */
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)  */

	} b;
	uint8_t w;
} PCF85063AT_DAYS;

/*
 * Days - Bit field mask definitions
 */
#define PCF85063AT_DAYS_MASK    ((uint8_t)0x3F)
#define PCF85063AT_DAYS_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Weekdays
 ** Enum: PCF85063AT_WEEKDAYS.
 ** --
 ** Offset : 0x08 Weekdays register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t weekdays : 3; /* Actual weekday value (0-6)*/
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_3 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_4 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_5 : 1; /* Reserved Bit (Unused)  */
	} b;
	uint8_t w;
} PCF85063AT_WEEKDAYS;

/*
 * Weekdays - Bit field mask definitions
 */
#define PCF85063AT_WEEKDAYS_MASK    ((uint8_t)0x07)
#define PCF85063AT_WEEKDAYS_SHIFT    ((uint8_t)0)


/*--------------------------------
 ** Register: Months
 ** Enum: PCF85063AT_MONTHS.
 ** --
 ** Offset : 0x09 Months register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t months : 5; /* Actual month coded in BCD format (1-12) */
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)  */
		uint8_t _reserved_3 : 1; /* Reserved Bit (Unused)  */
	} b;
	uint8_t w;
} PCF85063AT_MONTHS;

/*
 * Months - Bit field mask definitions
 */
#define PCF85063AT_MONTHS_MASK    ((uint8_t)0x1F)
#define PCF85063AT_MONTHS_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Years
 ** Enum: PCF85063AT_YEAR.
 ** --
 ** Offset : 0x0A Years register.
 ** ------------------------------*/
typedef uint8_t PCF85063AT_YEARS; /* Actual year coded in BCD format*/

/*
 * Years - Bit field mask definitions
 */
#define PCF85063AT_YEARS_MASK    ((uint8_t)0xFF)
#define PCF85063AT_YEARS_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Second_alarm
 ** Enum: PCF85063AT_SECONDS_ALARM.
 ** --
 ** Offset : 0x0B Second_alarm register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t seconds_alarm : 7;     /* Second alarm information coded in BCD format */
		uint8_t aen_s : 1;              /* 0- Second Alarm Enabled 1- Second Alarm Disabled*/
	} b;
	uint8_t w;
} PCF85063AT_SECONDS_ALARM;

/*
 * Second_alarm - Bit field mask definitions
 */
#define PCF85063AT_AEN_S_MASK             ((uint8_t)0x80)
#define PCF85063AT_AEN_S_SHIFT            ((uint8_t)7)

#define PCF85063AT_SECONDS_ALARM_MASK     ((uint8_t)0x7F)
#define PCF85063AT_SECONDS_ALARM_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Minute_alarm
 ** Enum: PCF85063AT_MINUTES_ALARM.
 ** --
 ** Offset : 0x0C Minute_alarm register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t minutes_alarm : 7; /* Minute alarm information coded in BCD format */
		uint8_t aen_m : 1;  /* 0- Minute Alarm Enabled 1- Minute Alarm Disabled*/

	} b;
	uint8_t w;
} PCF85063AT_MINUTES_ALARM;

/*
 * PCF85063AT_MINUTES_ALARM - Bit field mask definitions
 */
#define PCF85063AT_AEN_M_MASK             ((uint8_t)0x80)
#define PCF85063AT_AEN_M_SHIFT            ((uint8_t)7)

#define PCF85063AT_MINUTES_ALARM_MASK     ((uint8_t)0x7F)
#define PCF85063AT_MINUTES_ALARM_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Hour_alarm
 ** Enum: PCF85063AT_HOURS_ALARM.
 ** --
 ** Offset : 0x0D Hour_alarm register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t hour_alarm : 6; /* Hour alarm information coded in BCD format when in 12/24-hour mode*/
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)*/
		uint8_t aen_h : 1; /* 0- Hour Alarm Enabled 1- Hour Alarm Disabled */
	} b;
	uint8_t w;
} PCF85063AT_HOURS_ALARM;

/*
 * Hour_alarm - Bit field mask definitions
 */
#define PCF85063AT_AEN_H_MASK                ((uint8_t)0x80)
#define PCF85063AT_AEN_H_SHIFT               ((uint8_t)7)

#define PCF85063AT_HOURS_ALARM_MASK_24H     ((uint8_t)0x3F)
#define PCF85063AT_HOURS_ALARM_MASK_12H     ((uint8_t)0x1F)

/*--------------------------------
 ** Register: Day_alarm
 ** Enum: PCF85063AT_DAYS_ALARM.
 ** --
 ** Offset : 0x0E Day_alarm register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t days_alarm : 6; /* Day alarm information coded in BCD format*/
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)*/
		uint8_t aen_d : 1;  /* 0- Day Alarm Enabled 1- Day Alarm Disabled */

	} b;
	uint8_t w;
} PCF85063AT_DAYS_ALARM;

/*
 * Day_alarm - Bit field mask definitions
 */
#define PCF85063AT_AEN_D_MASK             ((uint8_t)0x80)
#define PCF85063AT_AEN_D_SHIFT            ((uint8_t)7)

#define PCF85063AT_DAYS_ALARM_MASK     ((uint8_t)0x3F)
#define PCF85063AT_DAYS_ALARM_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: Weekday_alarm
 ** Enum: PCF85063AT_WEEKDAY_ALARM.
 ** --
 ** Offset : 0x0F Weekday_alarm register.
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t weekdays_alarm : 3; /* Weekday alarm information */
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)*/
		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)*/
		uint8_t _reserved_3 : 1; /* Reserved Bit (Unused)*/
		uint8_t _reserved_4 : 1; /* Reserved Bit (Unused)*/
		uint8_t aen_w : 1; /* 0- Weekday Alarm Enabled 1- Weekday Alarm Disabled */
	} b;
	uint8_t w;
} PCF85063AT_WEEKDAYS_ALARM;

/*
 * Weekday_alarm - Bit field mask definitions
 */
#define PCF85063AT_AEN_W_MASK             ((uint8_t)0x80)
#define PCF85063AT_AEN_W_SHIFT            ((uint8_t)7)

#define PCF85063AT_WEEKDAYS_ALARM_MASK     ((uint8_t)0x07)
#define PCF85063AT_WEEKDAYS_ALARM_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: REGISTER TIMER_VALUE
 ** Enum: PCF85063AT_TIMER_value.
 ** --
 ** Offset : 0X10 timer_value register
 ** ------------------------------*/
typedef uint8_t PCF85063AT_TIMER_VALUES; /*COUNTDOWN TIMER VALUE*/


/*
 * Timer_value - Bit field mask definitions
 */
#define PCF85063AT_SECONDS_TS_MASK    ((uint8_t)0xFF)
#define PCF85063AT_SECONDS_TS_SHIFT    ((uint8_t)0)

/*--------------------------------
 ** Register: REGISTER TIMER_MODE
 ** Enum: PCF85063AT_TTIMER_MODE.
 ** --
 ** Offset :0x11 timer_MODE register
 ** ------------------------------*/
typedef union
{
	struct
	{
		uint8_t ti_tp : 1; /* timer interrupt mode, 0-interrupt follows timer flag, 1-interrupt generat pulse */
		uint8_t _tie : 1; /* 1-timer interrupt enable, 0-disabled*/
		uint8_t _te : 1; /* 1-timer enabled, 0-disabled*/
		uint8_t _tcf : 2; /* timer clock frequency*/
		uint8_t _reserved_1 : 1; /* Reserved Bit (Unused)*/
		uint8_t _reserved_2 : 1; /* Reserved Bit (Unused)*/
		uint8_t _reserved_3 : 1; /* Reserved Bit (Unused)*/
	} b;
	uint8_t w;
}PCF85063AT_TIMER_mode;

/*
 * Timer_MODE - Bit field mask definitions
 */
#define PCF85063AT_SECONDS_TI_TP_MASK    ((uint8_t)0x01)
#define PCF85063AT_SECONDS_TI_TP_SHIFT    ((uint8_t)0)

#define PCF85063AT_SECONDS_TIE_MASK    ((uint8_t)0x02)
#define PCF85063AT_SECONDS_TIE_SHIFT    ((uint8_t)1)

#define PCF85063AT_SECONDS_TE_MASK    ((uint8_t)0x04)
#define PCF85063AT_SECONDS_TE_SHIFT    ((uint8_t)2)

#define PCF85063AT_SECONDS_TCF_MASK    ((uint8_t)0x18)
#define PCF85063AT_SECONDS_TCF_SHIFT    ((uint8_t)3)


#endif /* PCF85063AT_H_ */
