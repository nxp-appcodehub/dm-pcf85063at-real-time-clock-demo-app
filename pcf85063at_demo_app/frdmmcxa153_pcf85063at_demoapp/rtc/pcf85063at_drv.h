/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * pcf85063at_drv.h
 */

#ifndef PCF85063AT_DRV_H_
#define PCF85063AT_DRV_H_

//#define unit8_t tcf

#include "pcf85063at.h"
#include "sensor_io_i2c.h"
#include "register_io_i2c.h"

/*--------------------------------
 ** Enum: IntMask
 ** @brief: To mask interrupts
 ** ------------------------------*/
typedef enum INTMASK
{
	INT_UNMASK = 0x00,
	INT_MASK = 0x01,
}IntMask;

/*--------------------------------
 ** Enum: ALARMTYPE
 ** @brief: store different alarm type
 ** ------------------------------*/
typedef enum ALARMTYPE
{
	A_Seconds = 0x01,
	A_Minute = 0x02,
	A_Hour = 0x03,
	A_Day = 0x04,
	A_Weekday = 0x05,
}AlarmType;

/*--------------------------------
 ** Enum: CAPSEL
 ** @brief: Capacitor selection frequency
 ** ------------------------------*/
typedef enum CAPSEL
{
	capSel7pf = 0x00, /*7 pF frequency of capacitor is selected*/
	capSel12pf = 0x01, /*12.5 pF frequency of capacitor is selected*/
}CAPSEL;

/*--------------------------------
 ** Enum: EXTTEST
 ** @brief: External text clock frequency
 ** ------------------------------*/
typedef enum EXTTEST
{
	normalMode = 0x00, /*normal mode is selected*/
	extTestMode = 0x01, /*external clock test mode is selected*/
}EXTTEST;

/*--------------------------------
 ** Enum: TIMER_INT_MODE
 ** @brief: To decide Interrupt dependency
 ** ------------------------------*/
typedef enum TIMER_INT_MODE
{
	timer_flag = 0x00, /*interrupt follows timer flag*/
	pulse = 0x01, /*interrupt will generate pulse*/
}TIMER_INT_MODE;

/*--------------------------------
 ** Enum: OFFSET_MODE
 ** @brief: to choose when will correction of clock is done
 ** ------------------------------*/
typedef enum OFFSET_MODE
{
	normal_mode = 0x00, /*offset is made once in 2 hours*/
	course_mode = 0x01, /*offset is made once in 4 minutes*/
}OFFSET_MODE;

/*--------------------------------
 ** Enum: TCF
 ** @brief: To choose timer clock frequency
 ** ------------------------------*/
typedef enum TCF
{
	timer1 = 0x00, /*4.096 kHz timer source clock*/
	timer2 = 0x01, /*64 Hz timer source clock*/
	timer3 = 0x02, /*1 Hz timer source clock*/
	timer4 = 0x03, /*1⁄60 Hz timer source clock*/
}TCF;

/*--------------------------------
 ** Enum: IntStatus
 ** @brief: store Interrupt status
 ** ------------------------------*/
typedef enum INTSTATUS
{
	intEnable = 0x01,      /* Interrupt Enable */
	intDisable = 0x00,     /* Interrupt Disable */
	intClear = 0x00,       /* Clear the Interrupt */
}IntStatus;

/*--------------------------------
 ** Enum: IntState
 ** @brief: store Interrupt state
 ** ------------------------------*/
typedef enum INTSTATE
{
	nointOccurred = 0x00,    /* No Interrupt Occurred */
	intOccurred = 0x01,      /* Interrupt Occurred */
}IntState;

/*--------------------------------
 ** Enum: TI_TP_State
 ** @brief: store Timer interrupt mode state
 ** ------------------------------*/
typedef enum TI_TP_State
{
	followTimerFlag = 0x00,    /* Interrupt follows timer flag */
	generatePulse = 0x01,      /* Interrupt generates pulse */
}TI_TP_State;


/*--------------------------------
 ** Enum: Mode12h_24h
 ** @brief: store 12h/24h mode
 ** ------------------------------*/
typedef enum MODE12H_24H
{
	mode24H = 0x00,         /* 24 Hour Mode Set*/
	mode12H = 0x01,         /* 12 Hour Mode Set */
}Mode12h_24h;


/*--------------------------------
 ** Enum: RtcOnOff
 ** @brief RTC on/off
 ** ------------------------------*/
typedef enum RTCONOFF
{
	rtcStart = 0x00,        /* Start Real Time Clock */
	rtcStop = 0x01,         /* Stop Real Time Clock */
}RtcOnOff;

/*--------------------------------
 ** Enum: LowBatInt
 ** @brief Low Battery Interrupt
 ** 		  Enable/Disable
 ** ------------------------------*/
typedef enum LOWBATINT
{
	lbIntEnable = 0x01,      /* Low Battery Interrupt Enable */
	lbIntDisable = 0x00,     /* Low Battery Interrupt Disable */
}LowBatInt;


/*--------------------------------
 ** Enum: SwRst
 ** @brief Software Reset
 ** ------------------------------*/
typedef enum SWRST
{
	SR = 0x00,             /* Software Reset */
}SwRst;

/*--------------------------------
 ** Enum: AmPm
 ** @brief Store AM PM mode
 ** ------------------------------*/
typedef enum AMPM
{
	AM = 0x00,             /* AM in 12H Mode */
	PM = 0x01,             /* PM in 12H Mode */
	h24 = 0x02,            /* 24H Mode*/
}AmPm;


/*--------------------------------
 ** Enum: WeekDays
 ** @brief store Weekday
 ** ------------------------------*/
typedef enum WEEKDAYS
{
	Sunday = 0x00,         /* 0 Corresponds to Sunday */
	Monday  = 0x01,        /* 1 Corresponds to Monday */
	Tuesday = 0x02,        /* 2 Corresponds to Tuesday */
	Wednesday = 0x03,      /* 3 Corresponds to Wednesday */
	Thursday  = 0x04,      /* 4 Corresponds to Thursday */
	Friday   = 0x05,       /* 5 Corresponds to Friday */
	Saturday = 0x06,       /* 6 Corresponds to Saturday */
}WeekDays;

/*--------------------------------
 ** Enum: Months
 ** @brief Months
 ** ------------------------------*/
typedef enum MONTHS
{
	January = 0x00,         /* 0 Corresponds to January */
	February = 0x01,        /* 1 Corresponds to February */
	March  = 0x02,          /* 2 Corresponds to March */
	April = 0x03,           /* 3 Corresponds to April */
	May = 0x05,             /* 4 Corresponds to May */
	June = 0x06,            /* 5 Corresponds to June */
	July = 0x07,            /* 6 Corresponds to July */
	August = 0x08,          /* 7 Corresponds to August */
	September = 0x09,       /* 8 Corresponds to September */
	October = 0x0A,         /* 9 Corresponds to October */
	November = 0x0B,        /* 10 Corresponds to November */
	December = 0x0C,        /* 11 Corresponds to December */
}Months;

/*! @brief This structure defines the Alarm Data in Seconds, Minutes, Hours, Days, Weekdays.*/
typedef struct
{
	uint8_t  second;
	uint8_t  minutes;
	uint8_t  hours;
	uint8_t  days;
	uint8_t  weekdays;
	AmPm     ampm;
} PCF85063AT_alarmdata_t;

/*! @brief This structure defines the Time Data in Seconds to Years.*/
typedef struct
{
	uint8_t  second;
	uint8_t  minutes;
	uint8_t  hours;
	uint8_t  days;
	uint8_t  weekdays;
	uint8_t  months;
	uint8_t  years;
	AmPm     ampm;
} PCF85063AT_timedata_t;


/*! @def    PCF85063AT_TIME_SIZE_BYTE
 *  @brief  The size of time. */
#define PCF85063AT_TIME_SIZE_BYTE    (7)

/*! @def    PCF85063AT_ALARM_TIME_SIZE_BYTE
 *  @brief  The size of Alarm time. */
#define PCF85063AT_ALARM_TIME_SIZE_BYTE    (5)

/*! @def    PCF85063AT_12h_Mode
 *  @brief  By default 12h mode Enable. */
#define PCF85063AT_12h_Mode    (0x04)

/*! @def    PCF85063AT_WD_TS_TP
 *  @brief  Generate a pulsed signal on INTB when TF flag is set. */
#define PCF85063AT_WD_TS_TP        (0x20)


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief This defines the sensor specific information for SPI.
 */
typedef struct
{
	registerDeviceInfo_t deviceInfo;      /*!< SPI device context. */
	ARM_DRIVER_I2C *pCommDrv;        /*!< Pointer to the i2c driver. */
	bool isInitialized;                   /*!< Whether sensor is intialized or not.*/
	uint16_t slaveAddress;           /*!< slave address.*/
}  PCF85063AT_sensorhandle_t;


/*******************************************************************************
 * APIs
 ******************************************************************************/

/*! @brief       Preprocesses a read command for the PCF85063AT RTC.
 *  @details     Prepares a read command to be sent to the RTC.
 *  @param[in]   pCmdOut  		Pointer to the command output buffer.
 *  @param[in]   offset   		Offset for the read command.
 *  @param[in]   size     		Size of the read command.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant  No
 */
void PCF85063AT_ReadPreprocess(void *pCmdOut, uint32_t offset, uint32_t size);

/*! @brief       Preprocesses a write command for the PCF85063AT RTC.
 *  @details     Prepares a write command to be sent to the RTC.
 *  @param[in]   pCmdOut  		Pointer to the command output buffer.
 *  @param[in]   offset  		Offset for the write command.
 *  @param[in]   size     		Size of the write command.
 *  @param[in] 	 pWritebuffer 	Pointer to the buffer containing data to be written.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant  No
 */
void PCF85063AT_WritePreprocess(void *pCmdOut, uint32_t offset, uint32_t size, void *pWritebuffer);

/*! @brief       Initializes the PCF85063AT RTC.
 *  @details     Initializes the PCF85063AT sensor and its handle.
 *  @param[in]   pSensorHandle  Pointer to sensor handle structure.
 *  @param[in]   pBus  			Pointer to CMSIS API compatible SPI bus object.
 *  @param[in]   index     		Index of the sensor.
 *  @param[in] 	 pSlaveSelect 	Pointer to the slave select pin.
 *  @constraints This should be the first API to be called.
 *				 Application has to ensure that previous instances of these APIs have exited before invocation.
 *  @reentrant   No
 *  @return      ::PCF85063AT_Initialize() returns the status
 */
int32_t PCF85063AT_Initialize(PCF85063AT_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress);

/*! @brief       Sets an idle task for the PCF85063AT RTC.
 *  @details     Sets a function to be called when the sensor is in idle state.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   idleTask  			Function Pointer to the idle task.
 *  @param[in]   userParam     		Pointer to user defined parameter for the idle task.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 */
void PCF85063AT_SetIdleTask(PCF85063AT_sensorhandle_t *pSensorHandle, registeridlefunction_t idleTask, void *userParam);

/*! @brief       Configures the PCF85063AT RTC.
 *  @details     Initializes the PCF85063AT sensor and its handle.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   pRegWriteList      Pointer to the list of register write operations.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Configue() returns the status.
 */
int32_t PCF85063AT_Configure(PCF85063AT_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList);

/*! @brief       De-initializes the PCF85063AT RTC.
 *  @details     De-initializes the PCF85063AT sensor and its handle.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return     ::PCF85063AT_Deinit() returns the status.
 */
int32_t PCF85063AT_Deinit(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Enables the second interrupt source for the PCF85063AT RTC.
 *  @details     Enables the specified interrupt source for second interrupt generation.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   intsrc  			Source of the interrupt to be enabled.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_SecInt_Enable() returns the status.
 */
int32_t PCF85063AT_HalfMinInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Disables the second interrupt source for the PCF85063AT RTC.
 *  @details     Disables the specified interrupt source for second interrupt generation.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_SecInt_Disable() returns the status.
 */
int32_t PCF85063AT_HalfMinInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Check the status of minute/second interrupt.
 *  @details     Checks whether minute/second interrupt is active or not.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[out]  pIntStatus       	Pointer to store the interrupt status.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Check_MinSecInt() returns the status.
 */
int32_t PCF85063AT_Check_MinHalfMinCTInt(PCF85063AT_sensorhandle_t *pSensorHandle, IntState *pIntStatus);

/*! @brief       Clears the minute/second interrupt.
 *  @details     Clears the minute/second interrupt status.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Clear_MinSecInt() returns the status.
 */
int32_t PCF85063AT_Clear_MinHalfMinCTInt(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Enables the minute/second interrupt for the PCF85063AT RTC.
 *  @details     Enables the specified interrupt source for minute/second interrupt generation.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   intsrc  			Source of the interrupt to be enabled.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return     ::PCF85063AT_MinInt_Enable() returns the status.
 */
int32_t PCF85063AT_MinInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Disables the minute/second interrupt for the PCF85063AT RTC.
 *  @details     Disables the specified interrupt source for minute/second interrupt generation.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_MinInt_Disable() returns the status.
 */
int32_t PCF85063AT_MinInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Start the real time clock (RTC)  for the PCF85063AT RTC.
 *  @details     Start the real time clock (RTC) of the RTC.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Rtc_Start() returns the status.
 */
int32_t PCF85063AT_Rtc_Start(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Stops the real time clock (RTC) for the PCF85063AT RTC.
 *  @details     Stops the real time clock (RTC) of the RTC.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Rtc_Stop() returns the status.
 */
int32_t PCF85063AT_Rtc_Stop(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Performs a Software Reset for the PCF85063AT RTC.
 *  @details     Resets the RTC using software reset.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   swrst  			Type of software reset to perform.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_SwRst() returns the status.
 */
int32_t PCF85063AT_SwRst(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Turns on the timestamp feature for the PCF85063AT RTC.
 *  @details     Enables the timestamp feature for the specified timestamp number.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   tsnum  			Number of the timestamp to be enabled.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_TimeStamp_On() returns the status.
 */

int32_t PCF85063AT_GetTime(PCF85063AT_sensorhandle_t *pSensorHandle,const registerreadlist_t  *PCF85063ATtimedata, PCF85063AT_timedata_t *time );

/*! @brief       Sets the time from the PCF85063AT RTC.
 *  @details     Sets the current time in the RTC registers.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   time    			Pointer to the time data to be set.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_SetTime() returns the status.
 */
int32_t PCF85063AT_SetTime(PCF85063AT_sensorhandle_t *pSensorHandle, PCF85063AT_timedata_t *time);

/*! @brief       Gets the timestamp from the PCF85063AT RTC.
 *  @details     Reads the timestampfor the specified timestamp number.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   PCF85063ATtsdata    	Pointer to the list of register read operations for timestamp data.
 *  @param[in]   tsnum    			Number of the timestamp to be read.
 *  @param[out]  timestamp    	    Pointer to store the read timestamp data.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_GetTs() returns the status.
 */

int32_t PCF85063AT_12h_24h_Mode_Set(PCF85063AT_sensorhandle_t *pSensorHandle, Mode12h_24h is_mode12h);

/*! @brief       Gets the current time mode from the PCF85063AT RTC.
 *  @details     Retrieves the current time mode (12-hour or 24-hour) from the PCF85063AT RTC.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[out]  pmode_12_24   		Pointer to store the current time mode.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_12h_24h_Mode_Get() returns the status.
 */
int32_t PCF85063AT_12h_24h_Mode_Get(PCF85063AT_sensorhandle_t *pSensorHandle, Mode12h_24h *pmode_12_24);

/*! @brief       Sets the 100th second mode for the PCF85063AT RTC.
 *  @details     Sets the 100th second mode(Enable/Disable) for the PCF85063AT RTC.
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[out]  is_s100h   		100th Second Mode: Enable or Disable.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Sec100TH_Mode() returns the status.
 */

int32_t PCF85063AT_Clear_AlarmInt(PCF85063AT_sensorhandle_t *pSensorHandle);


/*! @brief       Checks Alarm Interrupts for PCF85063AT RTC.
 *  @details     Checks Alarm Interrupts for PCF85063AT RTC on INTA/INTB pin .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   pAlarmState			Interrupt state checks whether interrupt occurred or not.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Check_AlarmInt() returns the status.
 */
int32_t PCF85063AT_Check_AlarmInt(PCF85063AT_sensorhandle_t *pSensorHandle, IntState *pAlarmState);

/*! @brief       Enables Alarm Interrupts for PCF85063AT RTC.
 *  @details     Enables Alarm Interrupts functionality for PCF85063AT RTC .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   intsrc             Interrupt Source (INTA or INTB)
 *  @param[in]   alarmtype          Alarm Type (Second,Minute,Hour,Day,Weekday).
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_AlarmInt_Enable() returns the status.
 */
int32_t PCF85063AT_AlarmInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle, AlarmType alarmtype);

/*! @brief       Disables Alarm Interrupts for PCF85063AT RTC.
 *  @details     Disables Alarm Interrupts functionality for PCF85063AT RTC .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_AlarmInt_Disable() returns the status.
 */
int32_t PCF85063AT_AlarmInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle);

/*! @brief       Get Alarm Time for PCF85063AT RTC.
 *  @details     Get Alarm functionality for PCF85063AT RTC .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   PCF85063ATalarmdata   Alarm Data
 *  @param[in]   alarmtime          Time to be set for alarm functionality
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_GetAlarmTime() returns the status.
 */
int32_t PCF85063AT_GetAlarmTime(PCF85063AT_sensorhandle_t *pSensorHandle,const registerreadlist_t *PCF85063ATalarmdata , PCF85063AT_alarmdata_t *alarmtime);


/*! @brief       Get Alarm Time for PCF85063AT RTC.
 *  @details     Get Alarm functionality for PCF85063AT RTC .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @param[in]   PCF85063ATalarmdata   Alarm Data
 *  @param[in]   alarmtime          Time to be set for alarm functionality
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_SetAlarmTime() returns the status.
 */
int32_t PCF85063AT_SetAlarmTime(PCF85063AT_sensorhandle_t *pSensorHandle, PCF85063AT_alarmdata_t *alarmtime);

/*! @brief       Disable Battery Switch Over Timestamps for PCF85063AT RTC.
 *  @details     Disable Battery Switch Over Timestamps functionality for PCF85063AT RTC .
 *  @param[in]   pSensorHandle  	Pointer to sensor handle structure.
 *  @constraints This can be called any number of times only after PCF85063AT_Initialize().
 *				 Application has to ensure that previous instances of these APIs have exited before invocation
 *  @reentrant   No
 *  @return      ::PCF85063AT_Bat_SwitchOverTS_Disable() returns the status.
 */

int32_t PCF85063AT_ExtTestMode(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_normalMode(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_capSel_12(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_capSel_7(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_TimerInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_TimerInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_TI_TP_Enable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_TI_TP_Disable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_TestFreeRAMByte(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_Normal_OffsetMode(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_Course_OffsetMode(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_CI_enable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_CI_disable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_timer_enable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_timer_disable(PCF85063AT_sensorhandle_t *pSensorHandle);

int32_t PCF85063AT_SetTimerClockFreq(PCF85063AT_sensorhandle_t *pSensorHandle, int32_t tcf);

int32_t PCF85063AT_Countdown_timer_value(PCF85063AT_sensorhandle_t *pSensorHandle, int32_t CT_value);

int32_t PCF85063AT_Set_offset(PCF85063AT_sensorhandle_t *pSensorHandle, int8_t offset);

int32_t PCF85063AT_Check_TI_TP(PCF85063AT_sensorhandle_t *pSensorHandle, TI_TP_State *pTI_TPState);

#endif /* PCF85063AT_DRV_H_ */
