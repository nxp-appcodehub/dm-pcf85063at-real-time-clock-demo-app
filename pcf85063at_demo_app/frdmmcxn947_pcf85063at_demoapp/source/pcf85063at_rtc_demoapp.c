/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file  pcf85063at_rtc_demoapp.c
 *  @brief The PCF85063AT_rtc_demoapp.c file implements the ISSDK PCF85063AT SPI and I2C RTC driver
 *         example demonstration for SPI and I2C Mode with interrupt mode and EDMA mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "frdmmcxn947.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------

#include "issdk_hal.h"
#include "gpio_driver.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "PCF85063AT.h"
#include "Driver_GPIO.h"
#include "pcf85063at_drv.h"


// Seize of RX/TX buffer
#define PCF85063AT_DATA_SIZE       2

#define ERROR_NONE       0
#define ERROR            1

/*! @brief Default Register settings. */
const registerwritelist_t PCF85063ATConfigDefault[] = {
		/* Set 12h mode. */
		{PCF85063AT_CTRL1, PCF85063AT_12h_Mode, PCF85063AT_CTRL1_12_HOUR_24_HOUR_MODE_MASK},
		/* generate a pulsed signal on MSF flag. */
		__END_WRITE_DATA__};

/*! @brief Address of Second Register for Time. */

const registerreadlist_t PCF85063ATtimedata[] = {{.readFrom = PCF85063AT_SECOND, .numBytes = PCF85063AT_TIME_SIZE_BYTE}, __END_READ_DATA__};

/*! @brief Address of Second Register for Alarm. */
const registerreadlist_t PCF85063ATalarmdata[] = {{.readFrom = PCF85063AT_SECOND_ALARM, .numBytes = PCF85063AT_ALARM_TIME_SIZE_BYTE}, __END_READ_DATA__};

/*! @brief Address of Second Register for Time Stamp. */
/*
const registerreadlist_t PCF85063ATTsSw1data[] = {{.readFrom = PCF85063AT_TIMESTAMP1_SECONDS, .numBytes = PCF85063AT_TS_SIZE_BYTE}, __END_READ_DATA__};
const registerreadlist_t PCF85063ATTsSw2data[] = {{.readFrom = PCF85063AT_TIMESTAMP2_SECONDS, .numBytes = PCF85063AT_TS_SIZE_BYTE}, __END_READ_DATA__};
const registerreadlist_t PCF85063ATTsSw3data[] = {{.readFrom = PCF85063AT_TIMESTAMP3_SECONDS, .numBytes = PCF85063AT_TS_SIZE_BYTE}, __END_READ_DATA__};
const registerreadlist_t PCF85063ATTsSw4data[] = {{.readFrom = PCF85063AT_TIMESTAMP4_SECONDS, .numBytes = PCF85063AT_TS_SIZE_BYTE}, __END_READ_DATA__};
*/

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------

GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;
static gpioConfigKSDK_t gpioConfigDefault = {
		.pinConfig = {kGPIO_DigitalInput, 1}, .portPinConfig = {0}, .interruptMode = kGPIO_InterruptFallingEdge};


void PCF85063AT_INTB_ISR(void)
{
	//Clear external interrupt flag.
	GPIO_GpioClearInterruptFlags(INTB_PIN.base, 1U << INTB_PIN.pinNumber);
	PRINTF("\r\n Interrupt Occurred on INTB Pin....Please Clear the interrupt\r\n");

	SDK_ISR_EXIT_BARRIER;
}


/*! -----------------------------------------------------------------------
 *  @brief       Initialize PCF85063AT Interrupt INTA Source and Enable IRQ
 *  @details     This function initializes PCF85063AT interrupt pin
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
void init_PCF85063AT_wakeup_intB(void)
{
	pGpioDriver->pin_init(&INTB_PIN, GPIO_DIRECTION_IN, NULL, NULL, NULL);
	EnableIRQ(PCF85063AT_INTB_IRQ);
}


/*!@brief        Print Alarm Time.
 *  @details     Print Alarm Time set by user.
 *  @param[in]   timeAlarm   Structure holding alarm data.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void printAlarmTime(PCF85063AT_alarmdata_t timeAlarm)
{
	PRINTF("\r\n TIME :- %d:%d:%d", timeAlarm.hours, timeAlarm.minutes, timeAlarm.second);

	if(timeAlarm.ampm == AM)
		PRINTF(" AM \r\n");
	else if(timeAlarm.ampm == PM)
		PRINTF(" PM \r\n");
	else
		PRINTF(" 24H Mode \r\n");

	PRINTF("\r\n DATE :- %d \r\n",timeAlarm.days);

	switch(timeAlarm.weekdays)
	{
	case 0:
		PRINTF("\r\n SUNDAY\r\n");
		break;
	case 1:
		PRINTF("\r\n MONDAY\r\n");
		break;
	case 2:
		PRINTF("\r\n TUESDAY\r\n");
		break;
	case 3:
		PRINTF("\r\n WEDNESDAY\r\n");
		break;
	case 4:
		PRINTF("\r\n THURSDAY\r\n");
		break;
	case 5:
		PRINTF("\r\n FRIDAY\r\n");
		break;
	case 6:
		PRINTF("\r\n SATURDAY\r\n");
		break;
	default:
		break;
	}
}

/*!@brief        Print Time.
 *  @details     Print Time set by user.
 *  @param[in]   timeAlarm   Structure holding time data.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void printTime(PCF85063AT_timedata_t timeData)
{
	PRINTF("\r\n TIME :- %02d:%02d:%02d", timeData.hours, timeData.minutes, timeData.second);

	if(timeData.ampm == AM)
		PRINTF(" AM\r\n");
	else if(timeData.ampm == PM)
		PRINTF(" PM\r\n");
	else
		PRINTF(" 24H Mode\r\n");

	PRINTF("\r\n DATE [DD/MM/YY]:- %02d/%02d/%02d\r\n",timeData.days, timeData.months, timeData.years );

	switch(timeData.weekdays)
	{
	case 0:
		PRINTF("\r\n SUNDAY\r\n");
		break;
	case 1:
		PRINTF("\r\n MONDAY\r\n");
		break;
	case 2:
		PRINTF("\r\n TUESDAY\r\n");
		break;
	case 3:
		PRINTF("\r\n WEDNESDAY\r\n");
		break;
	case 4:
		PRINTF("\r\n THURSDAY\r\n");
		break;
	case 5:
		PRINTF("\r\n FRIDAY\r\n");
		break;
	case 6:
		PRINTF("\r\n SATURDAY\r\n");
		break;
	default:
		break;
	}
}


/*!@brief        Get Alarm Type.
 *  @details     Get Alarm Type (Second,Minute,Hour,Day,Weekday).
 *  @param[in]   None.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Alarm Type.
 */
int32_t getAlarmType(void)
{
	uint8_t  alarmmode;
	PRINTF("\r\n Enter Alarm Type :-\r\n ");
	PRINTF("\r\n 1. Second Alarm\r\n");
	PRINTF("\r\n 2. Minute Alarm\r\n");
	PRINTF("\r\n 3. Hour Alarm\r\n");
	PRINTF("\r\n 4. Day Alarm\r\n");
	PRINTF("\r\n 5. Weekday Alarm\r\n");
	do{
		alarmmode = GETCHAR();
		alarmmode -= 48;
		PRINTF("%d\r\n",alarmmode);
		if( alarmmode >= 1 && alarmmode <=4)
			return alarmmode;
		else
			PRINTF("\r\nInvalid Alarm Type\r\n");
	}
	while(alarmmode < 0 || alarmmode > 5);
}

/*!@brief        Clear interrupts.
 *  @details     Clear interrupts (Seconds, Minute,Alarm).
 *  @param[in]   PCF85063ATDriver   Pointer to spi sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Status of the operation.
 */
int32_t clearInterrupts(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	IntState intstate;
	int32_t status;

	status = PCF85063AT_Check_MinHalfMinCTInt(PCF85063ATDriver, &intstate);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Check Minutes or Half Minute Interrupt Failed\r\n");
		return ERROR;
	}
	if(intstate == 0x01)
	{
		PRINTF("\r\n Minutes, Half Minute or Countdown Timer interrupt occurred: %x \r\n", intstate);
		status = PCF85063AT_Clear_MinHalfMinCTInt(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Minutes, Half Minute or Countdown Timer Interrupt clear Failed\r\n");
			return ERROR;
		}
		else
			PRINTF("\r\n Minutes, Half Minute or Countdown Timer Interrupt cleared\r\n");
	}

	status = PCF85063AT_Check_AlarmInt(PCF85063ATDriver, &intstate );
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Check Alarm Interrupt Failed\r\n");
		return ERROR;
	}
	if(intstate == 0x01)
	{
		PRINTF("\r\n Alarm Interrupt occurred: %x \r\n", intstate);
		status = PCF85063AT_Clear_AlarmInt(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Alarm Interrupt clear Failed\r\n");
			return ERROR;
		}
		else
			PRINTF("\r\n Alarm Interrupt cleared\r\n");
	}

}

/*!@brief        Set Alarm Time.
 *  @details     Set Alarm Time by taking input from user.
 *  @param[in]   PCF85063ATDriver   Pointer to spi sensor handle structure.
 *  @param[in]   timeAlarm       Alarm Time to be set by user.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Status of the operation.
 */
int32_t setAlarmTime(PCF85063AT_sensorhandle_t *PCF85063ATDriver, PCF85063AT_alarmdata_t *timeAlarm )
{
	int32_t status;
	uint8_t temp;
	Mode12h_24h mode12_24;

	/* Get Days from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Day value between 1 to 31 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 31)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 31);
	timeAlarm->days = temp;

	/* Get Weekdays from User and update its internal Time Structure */
	/* 0 - Sunday
	 * 1 - Monday
	 * 2 - Tuesday
	 * 3 - Wednesday
	 * 4 - Thursday
	 * 5 - Friday
	 * 6 - Saturday
	 */
	do{

		PRINTF("\r\n Enter Weekday value between 0 to 6 (0 - Sunday.....6 - Saturday)-");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);

		if(temp < 0 || temp > 6)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 6);
	timeAlarm->weekdays = temp;

	/* Get hour from User and update its internal Time Structure */
	do
	{
		/* Get 12h/24h hour format */
		PCF85063AT_12h_24h_Mode_Get(PCF85063ATDriver, &mode12_24);
		if(mode12_24 ==  mode24H)
			PRINTF("\r\n Enter Hour value between 0 to 23 :- ");
		else
			PRINTF("\r\n Enter Hour value between 1 to 12 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);

		if((mode12_24 ==  mode24H) && (temp < 0 || temp > 23))
			PRINTF("\r\nInvalid Value\r\n");

		if((mode12_24 ==  mode12H) && (temp < 1 || temp > 12))
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}
	while((mode12_24 ==  mode24H) && (temp < 0 || temp > 23) ||
			(mode12_24 ==  mode12H) && (temp < 1 || temp > 12));
	timeAlarm->hours = temp;

	/* Get Minutes from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Minute value between 0 to 59 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 0 || temp > 59)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 59);
	timeAlarm->minutes = temp;

	/* Get Second from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Second value between 0 to 59 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 0 || temp > 59)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 59);
	timeAlarm->second = temp;

	/* Get 12h/24h hour format */
	PCF85063AT_12h_24h_Mode_Get(PCF85063ATDriver, &mode12_24);
	if(mode12_24 !=  mode24H)
	{
		PRINTF("\r\n 1. AM \r\n");
		PRINTF("\r\n 2. PM \r\n");

		PRINTF("\r\n Enter Your choice :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		switch(temp)  /* Update AM/PM */
		{
		case 1:
			timeAlarm->ampm =  AM;
			break;
		case 2:
			timeAlarm->ampm = PM;
			break;
		default:
			PRINTF("\r\nInvalid choice\r\n");
			break;
		}
	}
	else
		timeAlarm->ampm = h24;

	/* Set Time */
	status = PCF85063AT_SetAlarmTime(PCF85063ATDriver,timeAlarm);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Set Time Failed\r\n");
		return ERROR;
	}

	return ERROR_NONE;
}

/*!@brief        Set Time.
 *  @details     Set Time by taking input from user.
 *  @param[in]   PCF85063ATDriver   Pointer to spi sensor handle structure.
 *  @param[in]   timeData        Time to be set by user.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Status of the operation.
 */
int32_t setTime(PCF85063AT_sensorhandle_t *PCF85063ATDriver, PCF85063AT_timedata_t *timeData)
{
	int32_t status;
	uint8_t temp;
	Mode12h_24h mode12_24;
	//S100thMode s100thmode;

	/* Get Days from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Day value between 1 to 31 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 31)
			PRINTF("\r\nInvalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 31);
	timeData->days = temp;

	/* Get Months from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Month value between 1 to 12 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 12)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 12);
	timeData->months = temp;

	/* Get Years from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Year value between 0 to 99 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 0 || temp > 99)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 99);
	timeData->years = temp;

	/* Get Weekdays from User and update its internal Time Structure */
	/* 0 - Sunday
	 * 1 - Monday
	 * 2 - Tuesday
	 * 3 - Wednesday
	 * 4 - Thursday
	 * 5 - Friday
	 * 6 - Saturday
	 */
	do{

		PRINTF("\r\n Enter Weekday value between 0 to 6 (0 - Sunday.....6 - Saturday)-");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);


		if(temp < 0 || temp > 6)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 6);
	timeData->weekdays = temp;
	/* Get hour from User and update its internal Time Structure */
	do
	{
		/* Get 12h/24h hour format */
		PCF85063AT_12h_24h_Mode_Get(PCF85063ATDriver, &mode12_24);
		if(mode12_24 ==  mode24H)
			PRINTF("\r\n Enter Hour value between 0 to 23 :- ");
		else
			PRINTF("\r\n Enter Hour value between 1 to 12 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);

		if((mode12_24 ==  mode24H) && (temp < 0 || temp > 23))
			PRINTF("\r\nInvalid Value\r\n");

		if((mode12_24 ==  mode12H) && (temp < 1 || temp > 12))
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while((mode12_24 ==  mode24H) && (temp < 0 || temp > 23) ||
			(mode12_24 ==  mode12H) && (temp < 1 || temp > 12));
	timeData->hours = temp;

	/* Get Minutes from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Minute value between 0 to 59 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 0 || temp > 59)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 59);
	timeData->minutes = temp;

	/* Get Second from User and update its internal Time Structure */
	do{
		PRINTF("\r\n Enter Second value between 0 to 59 :- ");
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 0 || temp > 59)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 0 || temp > 59);
	timeData->second = temp;


	/* Get 12h/24h hour format */
	PCF85063AT_12h_24h_Mode_Get(PCF85063ATDriver, &mode12_24);
	if(mode12_24 !=  mode24H)
	{
		PRINTF("\r\n 1. AM \r\n");
		PRINTF("\r\n 2. PM \r\n");

		PRINTF("\r\n Enter Your choice :- ");
		do
		{
			SCANF("%d",&temp);
			PRINTF("%d\r\n",temp);
			if(temp < 0 || temp > 2)
				PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
		}
		while(temp < 0 || temp > 2);

		switch(temp)  /* Update AM/PM */
		{
		case 1:
			timeData->ampm =  AM;
			break;
		case 2:
			timeData->ampm = PM;
			break;
		default:
			PRINTF("\r\nInvalid choice\r\n");
			break;
		}
	}
	else
		timeData->ampm = h24;

	/* Stop RTC */
	status = PCF85063AT_Rtc_Stop(PCF85063ATDriver);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n RTC Stop Failed\r\n");
		return ERROR;
	}

	/* Set Time */
	status = PCF85063AT_SetTime(PCF85063ATDriver,timeData);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Set Time Failed\r\n");
		return ERROR;
	}

	/* Start RTC */
	status = PCF85063AT_Rtc_Start(PCF85063ATDriver);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n RTC Start Failed\r\n");
		return ERROR;
	}

	return ERROR_NONE;
}


/*!@brief        Get Time data.
 *  @details     Get Time and date from corresponding Registers and
 *  				store back in internal structure.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @param[out]  timeData   		Structure holding time data.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Status of the operation.
 */
int32_t getTime(PCF85063AT_sensorhandle_t *PCF85063ATDriver, PCF85063AT_timedata_t *timeData)
{
	int32_t status;

	/* Get Time */
	status = PCF85063AT_GetTime(PCF85063ATDriver,(const registerreadlist_t *)&PCF85063ATtimedata , timeData);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Get Time Failed\r\n");
		return ERROR;
	}
	return ERROR_NONE;
}

/*!@brief        Get Alarm Time data.
 *  @details     Get Alarm Time from corresponding Registers and
 *  			 store back in internal structure.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @param[out]  timeAlarm   		Structure holding alarm data.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      Status of the operation.
 */
int32_t getAlarmTime(PCF85063AT_sensorhandle_t *PCF85063ATDriver, PCF85063AT_alarmdata_t *timeAlarm)
{
	int32_t status;

	/* Get Time */
	status = PCF85063AT_GetAlarmTime(PCF85063ATDriver,(const registerreadlist_t *)&PCF85063ATalarmdata , timeAlarm);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Get Alarm Failed\r\n");
		return ERROR;
	}
	return ERROR_NONE;
}


/*!@brief        Software Reset.
 *  @details     Clear Prescaler, Time Stamp and Software Reset.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void swReset(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	int32_t status;

	status = PCF85063AT_SwRst(PCF85063ATDriver);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Software Reset Failed\r\n");
	}
	else
	{
		PRINTF("\r\n Software Reset Done.....\r\n");
	}
}

/*!@brief        HalfMinuteInterrupt
 *  @details     Get Interrupt on INTA/INTB Pin every second.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void HalfMinuteInterrupt(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	uint8_t temp;
	int32_t status;
	IntState intstate;

	PRINTF("\r\n 1.Enable Half Minute Interrupt\r\n");
	PRINTF("\r\n 2.Disable Half Minute Interrupt\r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /*! Clear Pre-scaler. */
		status = PCF85063AT_Check_MinHalfMinCTInt(PCF85063ATDriver, &intstate);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Half Minute Interrupt Check Failed\r\n");
		}

		if(intstate == 0x01)
		{
			status = PCF85063AT_Clear_MinHalfMinCTInt(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Half Minute Interrupt Clear Failed\r\n");
			}
			else
				PRINTF("\r\n Half Minute Interrupt Cleared........ %x \r\n", intstate);
		}


		status = PCF85063AT_HalfMinInt_Enable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Half Minute Interrupt Enabled Failed\r\n");
		}
		else
			PRINTF("\r\n Half Minute Interrupt Enabled \r\n");

		break;
	case 2: /*! Clear Time Stamp. */
		status = PCF85063AT_HalfMinInt_Disable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Half Minute Interrupt Disable Failed\r\n");
		}
		else
			PRINTF("\r\n Half Minute Interrupt Disabled\r\n");
		break;
	default:
		PRINTF("\r\nInvalid Number\r\n");
		break;
	}

}

/*!@brief        Minutes Interrupt.
 *  @details     Get Interrupt on INTA/INTB Pin every Minute.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void minutesInterrupt(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	uint8_t temp;
	IntState intstate;
	int32_t status;

	PRINTF("\r\n 1.Enable Minutes Interrupt\r\n");
	PRINTF("\r\n 2.Disable Minutes Interrupt\r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /*! Clear Pre-scaler. */
		status = PCF85063AT_Check_MinHalfMinCTInt(PCF85063ATDriver, &intstate);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Minutes Interrupt Check Failed\r\n");
		}

		if(intstate == 0x01)
		{
			status = PCF85063AT_Clear_MinHalfMinCTInt(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Minutes Interrupt clear Failed\r\n");
			}
			else
				PRINTF("\r\n Minutes Interrupt Cleared........ %x \r\n", intstate);
		}

		status = PCF85063AT_MinInt_Enable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Minutes Interrupt Enable Failed\r\n");
		}
		else
			PRINTF("\r\n Minutes Interrupt Enabled \r\n");

		break;
	case 2: /*! Clear Time Stamp. */
		status = PCF85063AT_MinInt_Disable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Minutes Interrupt Disable Failed\r\n");
		}
		else
			PRINTF("\r\n Minutes Interrupt Disabled\r\n");
		break;
	default:
		PRINTF("\r\nInvalid Number\r\n");
		break;
	}
}

/*!@brief        alarm interrupt.
 *  @details     To get an alarm interrupt.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 */
int32_t alarmInterrupt(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	uint8_t temp;
	AlarmType alarmtypeInp;
	int32_t status;

	PRINTF("\r\n 1.Enable Alarm Interrupt\r\n");
	PRINTF("\r\n 2.Disable Alarm Interrupt\r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /*! Clear Pre-scaler. */
		alarmtypeInp = getAlarmType();
		status = PCF85063AT_Clear_AlarmInt(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Alarm Interrupt clear Failed\r\n");
		}

		status = PCF85063AT_AlarmInt_Enable(PCF85063ATDriver, alarmtypeInp);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Alarm Interrupt Enable Failed\r\n");
		}
		else
			PRINTF("\r\n Alarm Interrupt Enabled\r\n");
		break;
	case 2: /*! Clear Time Stamp. */
		status = PCF85063AT_Clear_AlarmInt(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n  Alarm Interrupt Clear Failed\r\n");
		}

		status = PCF85063AT_AlarmInt_Disable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Alarm Interrupt Disable Failed\r\n");
		}
		else
			PRINTF("\r\n Alarm Interrupt Disabled\r\n");
		break;
	default:
		PRINTF("\r\nInvalid Number\r\n");
		break;
	}
}
/*!@brief        Set mode (12h/24h).
 *  @details     set 12 hour / 24 hour format.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void setmode12h_24h(PCF85063AT_sensorhandle_t *PCF85063ATDriver)
{
	uint8_t temp;
	int32_t status;

	PRINTF("\r\n 1. 12H mode\r\n");
	PRINTF("\r\n 2. 24H mode\r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /* Set 12h clock format */
		status = PCF85063AT_12h_24h_Mode_Set(PCF85063ATDriver, mode12H);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n 12h clock format Set Failed\r\n");
		}
		else
			PRINTF("\r\n 12h mode is set\r\n");
		break;
	case 2: /* Set 24h clock format */
		status = PCF85063AT_12h_24h_Mode_Set(PCF85063ATDriver, mode24H);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n 24h clock format Set Failed\r\n");
		}
		else
			PRINTF("\r\n 24h mode is set\r\n");
		break;
	default:
		PRINTF("\r\n Invalid choice\r\n");
		break;
	}
}

/*!@brief        Set Timer Configuration
 *  @details     To set timer clock frequency, interrupt mode, timer interrupt mode and start/stop of timer.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @return      yes
 */

int32_t SetTimerConfig(PCF85063AT_sensorhandle_t *PCF85063ATDriver){

	float temp;
	int32_t temp1;
	int32_t temp2;
	int32_t status;
	int32_t CT_value;
	IntState intstate;
	TI_TP_State pTI_TPState;


	PRINTF("\r\n 1. Choose Timer Clock Frequency \r\n");
	PRINTF("\r\n 2. Set Timer Interrupt Mode \r\n");
	PRINTF("\r\n 3. Enable/Disable Timer Interrupt \r\n");
	PRINTF("\r\n 4. Start/Stop Timer \r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp2);
		PRINTF("%d\r\n",temp2);
		if(temp2 < 1 || temp2 > 4)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp2 < 1 || temp2 > 4);

	switch(temp2){

	case 1: //To set the Timer Clock Frequency

		PRINTF("\r\n Choose Timer Clock Frequency \r\n");
		PRINTF("\r\n 1. 4.096 kHz (244 microSec - 62.256 ms) \r\n");
		PRINTF("\r\n 2. 64 Hz (15.625 ms - 3.984 s) \r\n");
		PRINTF("\r\n 3. 1 Hz (1 s - 255 s) \r\n");
		PRINTF("\r\n 4. 1⁄60 Hz (60 s - 4 hr 15 min) \r\n");

		PRINTF("\r\n Enter your choice :- ");
		do{
			SCANF("%d",&temp1);
			PRINTF("%d\r\n",temp1);
			if(temp1 < 1 || temp1 > 4)
				PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
		}
		while(temp1 < 1 || temp1 > 4);

		status = PCF85063AT_SetTimerClockFreq(PCF85063ATDriver, temp1);

		PRINTF("\r\n Timer Clock Frequency is Selected\r\n");

		if(temp1==1){
			PRINTF("\r\n Enter clock frequency value (0.000244-0.062256) :- ");
			do{
				SCANF("%f",&temp);
				PRINTF("%f\r\n",temp);
				if(temp < 0.000244 || temp > 0.062256)
					PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
			}
			while(temp < 0.000244 || temp > 0.062256);
			// To find the countdown timer value
			CT_value = (temp/0.000244);
			status = PCF85063AT_Countdown_timer_value(PCF85063ATDriver, CT_value);

			PRINTF("\r\n Timer for %f s is set\r\n ", temp);
		}
		if(temp1==2){
			PRINTF("\r\n Enter clock frequency value (0.015625-3.984) :- ");
			do{
				SCANF("%f",&temp);
				PRINTF("%f\r\n",temp);
				if(temp < 0.015625 || temp > 3.984)
					PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
			}
			while(temp < 0.015625 || temp > 3.984);
			// To find the countdown timer value
			CT_value = (temp/0.015625);
			status = PCF85063AT_Countdown_timer_value(PCF85063ATDriver, CT_value);

			PRINTF("\r\n Timer for %f s is set\r\n ", temp);
		}
		if(temp1==3){
			PRINTF("\r\n Enter clock frequency value (1-255) :- ");
			do{
				SCANF("%f",&temp);
				PRINTF("%f\r\n",temp);
				if(temp < 1 || temp > 255)
					PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
			}
			while(temp < 1 || temp > 255);
			// To find the countdown timer value
			CT_value = (temp/1);
			status = PCF85063AT_Countdown_timer_value(PCF85063ATDriver, CT_value);

			PRINTF("\r\n Timer for %f s is set\r\n ", temp);
		}
		if(temp1==4){
			PRINTF("\r\n Enter clock frequency value (60-15300) :- ");
			do{
				SCANF("%f",&temp);
				PRINTF("%f\r\n",temp);
				if(temp < 60 || temp > 15300)
					PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
			}
			while(temp < 60 || temp > 15300);
			// To find the countdown timer value
			CT_value = (temp/60);
			status = PCF85063AT_Countdown_timer_value(PCF85063ATDriver, CT_value);

			PRINTF("\r\n Timer for %f s is set\r\n ", temp);
		}

	case 2: // To set Timer Interrupt Mode

		PRINTF("\r\n Set Timer Interrupt Mode \r\n");

		PRINTF("\r\n 1. Interrupt generates Pulse \r\n");
		PRINTF("\r\n 2. Interrupt follows Timer Flag \r\n");

		PRINTF("\r\n Enter your choice :- ");
		do{
			SCANF("%d",&temp1);
			PRINTF("%d\r\n",temp1);
			if(temp1 < 1 || temp1 > 2)
				PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
		}
		while(temp1 < 1 || temp1 > 2);

		switch(temp1)
		{
		case 1: /* Enable Timer Interrupt Mode to get interrupt as Pulse */

			status = PCF85063AT_TI_TP_Enable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer Interrupt Mode enabling is Failed\r\n");
			}
			else
				PRINTF("\r\n Timer Interrupt Mode is enabled \r\n");
			break;

		case 2: /* Disable Timer Interrupt Mode to follow Timer flag */

			status = PCF85063AT_TI_TP_Disable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer Interrupt Mode disabling is Failed \r\n");
			}
			else
				PRINTF("\r\n Timer Interrupt Mode is disabled \r\n");
			break;
		default:
			PRINTF("\r\n Invalid choice\r\n");
			break;
		}

	case 3: // To set Timer Interrupts

		PRINTF("\r\n Set Timer Interrupt  \r\n");

		PRINTF("\r\n 1. Enable Timer Interrupt  \r\n");
		PRINTF("\r\n 2. Disable Timer Interrupt\r\n");

		PRINTF("\r\n Enter your choice :- ");
		do{
			SCANF("%d",&temp1);
			PRINTF("%d\r\n",temp1);
			if(temp1 < 1 || temp1 > 2)
				PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
		}
		while(temp1 < 1 || temp1 > 2);

		switch(temp1)
		{
		case 1: /* Enable Timer Interrupt */

			status = PCF85063AT_TimerInt_Enable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer Interrupt enabling is Failed\r\n");
			}
			else
				PRINTF("\r\n Timer Interrupt is enabled \r\n");
			break;

		case 2: /* Disable Timer Interrupt */

			status = PCF85063AT_TimerInt_Disable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer Interrupt disabling is Failed \r\n");
			}
			else
				PRINTF("\r\n Timer Interrupt is disabled \r\n");
			break;
		default:
			PRINTF("\r\n Invalid choice\r\n");
			break;
		}

	case 4: // Set Timer

		PRINTF("\r\n Set Timer \r\n");
		PRINTF("\r\n 1. Enable Timer  \r\n");
		PRINTF("\r\n 2. Disable Timer \r\n");

		PRINTF("\r\n Enter your choice :- ");
		do{
			SCANF("%d",&temp1);
			PRINTF("%d\r\n",temp1);
			if(temp1 < 1 || temp1 > 2)
				PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
		}
		while(temp1 < 1 || temp1 > 2);

		switch(temp1)
		{
		case 1: /* Enable Timer */
			status = PCF85063AT_timer_enable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer enabling is Failed\r\n");
			}
			else
				PRINTF("\r\n Timer is enabled \r\n");
			break;

		case 2: /* Disable Timer */

			status = PCF85063AT_timer_disable(PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n Timer disabling is Failed \r\n");
			}
			else
				PRINTF("\r\n Timer is disabled \r\n");
			break;
		default:
			PRINTF("\r\n Invalid choice\r\n");
			break;
		}
	}
}

/*!@brief        Set offset mode
 *  @details     set normal/cousre mode for correction of clock pulse.
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */

void SetOffsetMode(PCF85063AT_sensorhandle_t *PCF85063ATDriver){

	uint8_t temp;
	int8_t offset;
	int32_t status;

	PRINTF("\r\n 1. Normal Mode \r\n");
	PRINTF("\r\n 2. Course Mode \r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /* Set Normal Mode */
		status = PCF85063AT_Normal_OffsetMode(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Normal Mode Setup Failed\r\n");
		}
		else
			PRINTF("\r\n Normal Mode is set\r\n");
		break;
	case 2: /* Set Course Mode */
		status = PCF85063AT_Course_OffsetMode(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Course Mode Setup Failed\r\n");
		}
		else
			PRINTF("\r\n Course Mode is set\r\n");
		break;
	default:
		PRINTF("\r\n Invalid choice\r\n");
		break;
	}

	/*To correct/tune the clock pulse, offset needs to be selected. */
	PRINTF("\r\n 1. Set Offset Value (B/w -64 To +63) \r\n");
	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&offset);
		PRINTF("%d\r\n",offset);
		if(offset < -64 || offset > +64)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(offset < -64 || offset > +63);

	if(!((offset >= 0) && (offset <= 63)))
		offset = (~offset) + 1;

	status = PCF85063AT_Set_offset(PCF85063ATDriver, offset);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n Correction Failed\r\n");
	}
	else
		PRINTF("\r\n Correction is done \r\n");

}

/*!@brief        correction interrupt
 *  @details     enable/disable correction interrupts
 *  @param[in]   PCF85063ATDriver   Pointer to sensor handle structure.
 *  @constraints None
 *
 *  @reentrant   No
 *  @return      No
 */
void Correction_INT(PCF85063AT_sensorhandle_t *PCF85063ATDriver){

	int32_t status;
	int32_t temp;

	/*To view the corrected clock pulse, correction interrupt should be enabled */

	PRINTF("\r\n 1. Enable Correction interrupt  \r\n");
	PRINTF("\r\n 2. Disable Correction interrupt \r\n");

	PRINTF("\r\n Enter your choice :- ");
	do{
		SCANF("%d",&temp);
		PRINTF("%d\r\n",temp);
		if(temp < 1 || temp > 2)
			PRINTF("\r\n Invalid Value, Please enter correct value\r\n");
	}
	while(temp < 1 || temp > 2);

	switch(temp)
	{
	case 1: /* Enable correction interrupt to view the corrected clock pulse on INT pin */
		status = PCF85063AT_CI_enable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Correction interrupt enabling is Failed\r\n");
		}
		else
			PRINTF("\r\n Correction interrupt is enabled \r\n");
		break;

	case 2: /* Disable correction interrupt */

		status = PCF85063AT_CI_disable(PCF85063ATDriver);
		if (SENSOR_ERROR_NONE != status)
		{
			PRINTF("\r\n Correction interrupt disabling is Failed \r\n");
		}
		else
			PRINTF("\r\n Correction interrupt is disabled \r\n");
		break;
	default:
		PRINTF("\r\n Invalid choice\r\n");
		break;
	}

}
/*! @brief      This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the RTC sensor and
 *               finally enters an endless loop to continuously Get/Set RTC data.
 *  @param[in]   void This is no input parameter.
 *  @constraints None
 *  @reentrant   No
 *  @return      Error.
 */

int main(void)
{
	PCF85063AT_timedata_t timeData;
	PCF85063AT_alarmdata_t timeAlarm;
	AlarmType alarmtype;
	int32_t status;
	uint8_t character;
	int32_t tcf;
	uint8_t data[PCF85063AT_DATA_SIZE];
	char dummy;
	PCF85063AT_sensorhandle_t PCF85063ATDriver;

	/* Enable EDMA for I2C */
#if (RTE_I2C2_DMA_EN)
	/* Enable DMA clock. */
	CLOCK_EnableClock(EXAMPLE_DMA_CLOCK);
	edma_config_t edmaConfig = {0};
	EDMA_GetDefaultConfig(&edmaConfig);
	EDMA_Init(EXAMPLE_DMA_BASEADDR, &edmaConfig);
#endif


 	ARM_DRIVER_I2C *pdriver = &I2C_S_DRIVER;

	/*! Initialize the MCU hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_SystickEnable();
	BOARD_InitDebugConsole();


	/*! Register ISR for  INTB */
	init_PCF85063AT_wakeup_intB();

	/*! Initialize the driver. */

	PRINTF("\r\n ISSDK PCF85063AT RTC driver example demonstration.\r\n");


	/*! Initialize the driver. */
	status = pdriver->Initialize(I2C_S_SIGNAL_EVENT);

	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n I2C Initialization Failed\r\n");
		return -1;
	}

	/*! Set the Power mode. */
	status = pdriver->PowerControl(ARM_POWER_FULL);
	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n Driver Power Mode setting Failed\r\n");
		return -1;
	}

	/*! Set the Slave speed. */
	status = pdriver->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);

	if (ARM_DRIVER_OK != status)
	{
		PRINTF("\r\n Driver Control Mode setting Failed\r\n");
		return -1;
	}

	/*! Initialize the PCF85063AT RTC driver. */
	status = PCF85063AT_Initialize(&PCF85063ATDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, PCF85063AT_I2C_ADDR);

	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n PCF85063AT RTC Initialization Failed\r\n");
		return -1;
	}

	/*! Configure the PCF85063AT RTC driver. */
	status = PCF85063AT_Configure(&PCF85063ATDriver, PCF85063ATConfigDefault);
	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n PCF85063AT RTC Configuration Failed, Err = %d\r\n", status);
		return -1;
	}

	PRINTF("\r\n Successfully Applied PCF85063AT RTC Configuration\r\n");

	/*Initial software reset to read/write the registers */
	status = PCF85063AT_SwRst(&PCF85063ATDriver);

	if (SENSOR_ERROR_NONE != status)
	{
		PRINTF("\r\n PCF85063AT RTC Software Reset failed \r\n");
		return -1;
	}

	PRINTF("\r\n PCF85063AT RTC Initial Software Reset completed \r\n");

	do
	{
		PRINTF("\r\n");
		PRINTF("\r\n *********** Main Menu ***************\r\n");
		PRINTF("\r\n 1. RTC Start \r\n");
		PRINTF("\r\n 2. RTC Stop \r\n");
		PRINTF("\r\n 3. Get Time and Date \r\n");
		PRINTF("\r\n 4. Set Time and Date \r\n");
		PRINTF("\r\n 5. Software Reset \r\n");
		PRINTF("\r\n 6. Minutes Interrupt \r\n");
		PRINTF("\r\n 7. Half Minute Interrupt\r\n");
		PRINTF("\r\n 8. Get Alarm Time \r\n");
		PRINTF("\r\n 9. Set Alarm Time \r\n");
		PRINTF("\r\n 10. Alarm Interrupt \r\n");
		PRINTF("\r\n 11. Timer configuration \r\n");
		PRINTF("\r\n 12. Correction Interrupt \r\n");
		PRINTF("\r\n 13. Set Offset/Correction Mode \r\n");
		PRINTF("\r\n 14. Clear Interrupts\r\n");
		PRINTF("\r\n 15. Exit \r\n");
		PRINTF("\r\n");

		PRINTF("\r\n Enter your choice :- ");
		SCANF("%d",&character);
		PRINTF("%d\r\n",character);

		switch (character)
		{
		case 1: /* RTC Start */
			status = PCF85063AT_Rtc_Start(&PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n RTC Start Failed\r\n");
				continue;
			}
			PRINTF("\r\n RTC Started\r\n");
			break;
		case 2: /* RTC Stop */
			status = PCF85063AT_Rtc_Stop(&PCF85063ATDriver);
			if (SENSOR_ERROR_NONE != status)
			{
				PRINTF("\r\n  RTC Stop Failed\r\n");
				continue;
			}
			PRINTF("\r\n RTC Stopped\r\n");
			break;
		case 3: /* Get Time */
			memset(&timeData, '0',sizeof(PCF85063AT_timedata_t));
			status = getTime(&PCF85063ATDriver, &timeData);
			if (ERROR  != status)
			{
				printTime(timeData);  // print time data
			}
			break;
		case 4: /* Set Time */
			setmode12h_24h(&PCF85063ATDriver);  /* Set 12h/24h mode */
			memset(&timeData, '0',sizeof(PCF85063AT_timedata_t));
			setTime(&PCF85063ATDriver, &timeData);    // set time data
			break;
		case 5: /*  Software Reset */
			swReset(&PCF85063ATDriver);
			break;
		case 6: /* minute Interrupt */
			minutesInterrupt(&PCF85063ATDriver);
			break;
		case 7: /* Half minute Interrupt */
			HalfMinuteInterrupt(&PCF85063ATDriver);
			break;
		case 8: /* Get Alarm Time */
			memset(&timeAlarm, '0',sizeof(PCF85063AT_alarmdata_t));
			status = getAlarmTime(&PCF85063ATDriver, &timeAlarm);
			if (ERROR  != status)
			{
				printAlarmTime(timeAlarm);  // print time data
			}
			break;
		case 9: /* Set Alarm Time */
			memset(&timeAlarm, '0',sizeof(PCF85063AT_alarmdata_t));
			setAlarmTime(&PCF85063ATDriver, &timeAlarm);    // set time data
			break;
		case 10:  /* Alarm Interrupt */
			alarmInterrupt(&PCF85063ATDriver);
			break;
    	case 11:   //Timer configuration
			PRINTF("\r\n Set Timer Configuration!!\r\n");
			SetTimerConfig(&PCF85063ATDriver);
			break;
    	case 12: /* Correction Interrupt */
			PRINTF("\r\n Correction Interrupt!!\r\n");
			Correction_INT(&PCF85063ATDriver);
			break;
		case 13:  /* Set Offset Mode */
			PRINTF("\r\n Make sure, correction interrupt is enabled before!! \r\n");
			PRINTF("\r\n Set Offset/Correction Mode!!\r\n");
			SetOffsetMode(&PCF85063ATDriver);
			break;
		case 14:  /* Clear Interrupts */
			PRINTF("\r\n Clearing Interrupts!!\r\n");
			clearInterrupts(&PCF85063ATDriver);
			break;
		case 15:  /* Exit */
			PRINTF("\r\n .....Bye\r\n");
			exit(0);
			break;
		default:
			PRINTF("\r\n Invalid option...chose correct one from Main Menu\r\n");
			break;
}
		PRINTF("\r\n Press Enter to goto Main Menu\r\n");
		do
		{
			dummy = GETCHAR();
		} while(dummy != 13);

	}while(1);
	return 0;
}
