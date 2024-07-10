/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * PCF85063AT_i2c_drv.c
 *
 * @file PCF85063AT_i2c_drv.c
 * @brief The PCF85063AT_i2c_drv.c file implements the PCF85063AT RTC driver interfaces.
 */

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------

#include "gpio_driver.h"
#include "systick_utils.h"
#include "pcf85063at_drv.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
bool repeatedStart = 1;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
//#if (I2C_ENABLE)

uint8_t DecimaltoBcd(uint8_t val)
{
	return (((val/10) << 4) | (((val) % 10) & 0x0f));
}

uint8_t BcdToDecimal(uint8_t val)
{
	return (((val) >> 4) * 10 + ((val) & 0x0f)) ;
}


int32_t PCF85063AT_Initialize(PCF85063AT_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress)
{
	int32_t status;
	uint8_t reg = 0;

	/*! Check the input parameters. */
	if ((pSensorHandle == NULL) || (pBus == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	pSensorHandle->deviceInfo.deviceInstance = index;
	pSensorHandle->deviceInfo.functionParam = NULL;
	pSensorHandle->deviceInfo.idleFunction = NULL;

	/*! Initialize the sensor handle. */
	pSensorHandle->pCommDrv = pBus;
	pSensorHandle->slaveAddress = sAddress;
	pSensorHandle->isInitialized = true;
	return SENSOR_ERROR_NONE;
}

void PCF85063AT_SetIdleTask(PCF85063AT_sensorhandle_t *pSensorHandle,
		registeridlefunction_t idleTask,
		void *userParam)
{
	pSensorHandle->deviceInfo.functionParam = userParam;
	pSensorHandle->deviceInfo.idleFunction = idleTask;
}


int32_t PCF85063AT_ReadData(PCF85063AT_sensorhandle_t *pSensorHandle,
		const registerreadlist_t *pReadList,
		uint8_t *pBuffer)
{
	int32_t status;

	/*! Validate for the correct handle and register read list.*/
	if ((pSensorHandle == NULL) || (pReadList == NULL) || (pBuffer == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before reading sensor data.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Parse through the read list and read the data one by one. */
	status = Sensor_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			pReadList, pBuffer);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_READ;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_WriteData(PCF85063AT_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if ((pSensorHandle == NULL) || (pRegWriteList == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Apply the Sensor Configuration based on the Register Write List */
	status = Sensor_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			pRegWriteList);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Configure(PCF85063AT_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList)
{
	int32_t status;

	/*! Validate for the correct handle and register write list.*/
	if ((pSensorHandle == NULL) || (pRegWriteList == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before applying configuration.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Apply the Sensor Configuration based on the Register Write List */
	status = Sensor_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			pRegWriteList);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Rtc_Start(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Start RTC source clock */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,PCF85063AT_CTRL1,
			(uint8_t)(rtcStart << PCF85063AT_CTRL1_START_STOP_SHIFT ),PCF85063AT_CTRL1_START_STOP_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}


int32_t PCF85063AT_Rtc_Stop(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Stop RTC source clock */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1,(uint8_t)(rtcStop << PCF85063AT_CTRL1_START_STOP_SHIFT ),PCF85063AT_CTRL1_START_STOP_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_SwRst(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1,(0x58),0x00,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

/*! Set Time*/

int32_t PCF85063AT_SetTime(PCF85063AT_sensorhandle_t *pSensorHandle, PCF85063AT_timedata_t *time)
{
	int32_t status;
	Mode12h_24h mode12_24;

	/*! Validate for the correct handle and time read variable.*/
	if ((pSensorHandle == NULL) || (time == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Set Second.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			PCF85063AT_SECOND , DecimaltoBcd(time->second & PCF85063AT_SECONDS_MASK) , PCF85063AT_SECONDS_MASK , repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Minutes.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_MINUTE, DecimaltoBcd(time->minutes & PCF85063AT_MINUTES_MASK ), PCF85063AT_MINUTES_MASK , repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Update AM/PM Bit.*/
	if(time->ampm == AM)
	{
		time->hours = DecimaltoBcd(time->hours & PCF85063AT_HOURS_MASK_12H);
		time->hours = ((time->hours) & (~(PM << PCF85063AT_AM_PM_SHIFT)));
	}
	else if(time->ampm == PM)
	{
		time->hours = DecimaltoBcd(time->hours & PCF85063AT_HOURS_MASK_12H);
		time->hours = (time->hours | (PM << PCF85063AT_AM_PM_SHIFT));
	}
	else
		time->hours = DecimaltoBcd(time->hours & PCF85063AT_HOURS_MASk_24H);

	/*! Set Hour.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_HOUR, time->hours , PCF85063AT_HOURS_MASk_24H, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Day.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_DAY, DecimaltoBcd(time->days & PCF85063AT_DAYS_MASK),PCF85063AT_DAYS_MASK, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set WeekDay.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_WEEKDAY, DecimaltoBcd(time->weekdays &  PCF85063AT_WEEKDAYS_MASK ), PCF85063AT_WEEKDAYS_MASK, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Months.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_MONTH, DecimaltoBcd(time->months & PCF85063AT_MONTHS_MASK),PCF85063AT_MONTHS_MASK, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Year.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_YEAR, DecimaltoBcd(time->years & PCF85063AT_YEARS_MASK) , PCF85063AT_YEARS_MASK, repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}


/*! Get Time*/
int32_t PCF85063AT_GetTime(PCF85063AT_sensorhandle_t *pSensorHandle, const registerreadlist_t *PCF85063ATtimedata, PCF85063AT_timedata_t *time )
{
	int32_t status;
	Mode12h_24h mode12_24;

	/*! Validate for the correct handle and time read variable.*/
	if ((pSensorHandle == NULL) || (time == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Get time.*/
	status = PCF85063AT_ReadData(pSensorHandle, PCF85063ATtimedata, ( uint8_t *)time );
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! after read convert BCD to Decimal */
	time->second = BcdToDecimal(time->second & PCF85063AT_SECONDS_MASK) ;
	time->minutes = BcdToDecimal(time->minutes & PCF85063AT_MINUTES_MASK);

	PCF85063AT_12h_24h_Mode_Get(pSensorHandle, &mode12_24);
	if(mode12_24 ==  mode24H)
	{
		time->hours = BcdToDecimal(time->hours & PCF85063AT_HOURS_MASk_24H) ;
		time->ampm = h24;
	}
	else   /* Set AM/PM */
	{
		if( (time->hours >> 5) & 0x01 )
			time->ampm = PM;
		else
			time->ampm = AM;
		time->hours =  BcdToDecimal(time->hours & PCF85063AT_HOURS_MASK_12H);
	}
	time->days = BcdToDecimal(time->days & PCF85063AT_DAYS_MASK);
	time->weekdays = BcdToDecimal(time->weekdays & PCF85063AT_WEEKDAYS_MASK);
	time->months = BcdToDecimal(time->months & PCF85063AT_MONTHS_MASK);
	time->years = BcdToDecimal(time->years) ;

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_12h_24h_Mode_Set(PCF85063AT_sensorhandle_t *pSensorHandle, Mode12h_24h is_mode12h)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Set 12/24 mode */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1, (uint8_t)(((is_mode12h == mode12H) ? mode12H : mode24H )<< PCF85063AT_CTRL1_12_HOUR_24_HOUR_MODE_SHIFT ), PCF85063AT_CTRL1_12_HOUR_24_HOUR_MODE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_12h_24h_Mode_Get(PCF85063AT_sensorhandle_t *pSensorHandle, Mode12h_24h *pmode_12_24)
{
	int32_t status;
	PCF85063AT_CTRL_1 Ctrl1_Reg;

	/*! Validate for the correct handle */
	if ((pSensorHandle == NULL) || (pmode_12_24 == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Get 12/24 mode */
	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1, PCF85063AT_REG_SIZE_BYTE, (uint8_t *)&Ctrl1_Reg);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_READ;
	}
	*pmode_12_24 = Ctrl1_Reg.b.mode_12_24;

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_capSel_7(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering capacitor selection.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! select 7pF capacitor frequency*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
				PCF85063AT_CTRL1,(uint8_t)(capSel7pf << PCF85063AT_CTRL1_CAP_SEL_SHIFT ),PCF85063AT_CTRL1_CAP_SEL_MASK,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_capSel_12(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering capacitor selection.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! select 12.5pF capacitor frequency*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
				PCF85063AT_CTRL1,(uint8_t)(capSel12pf << PCF85063AT_CTRL1_CAP_SEL_SHIFT ),PCF85063AT_CTRL1_CAP_SEL_MASK,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_normalMode(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering capacitor selection.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Set normal mode*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
				PCF85063AT_CTRL1,(uint8_t)(normalMode << PCF85063AT_CTRL1_EXT_TEST_SHIFT ),PCF85063AT_CTRL1_EXT_TEST_MASK,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_ExtTestMode(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering capacitor selection.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! set external test mode*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
				PCF85063AT_CTRL1,(uint8_t)(extTestMode << PCF85063AT_CTRL1_EXT_TEST_SHIFT ),PCF85063AT_CTRL1_EXT_TEST_MASK,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Check_MinHalfMinCTInt(PCF85063AT_sensorhandle_t *pSensorHandle, IntState *pIntStatus)
{
	int32_t status;
	PCF85063AT_CTRL_2 Ctrl2_Reg;

	/*! Validate for the correct handle and Interrupt status read variable.*/
	if ((pSensorHandle == NULL) || (pIntStatus == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Get Minute/half minute/countdown timer interrupt flag */
	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, PCF85063AT_REG_SIZE_BYTE, (uint8_t *)&Ctrl2_Reg);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_READ;
	}
    *pIntStatus = Ctrl2_Reg.b.tf ;
	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Clear_MinHalfMinCTInt(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! clear Minute/half minute/countdown timer interrupt flag */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intClear << PCF85063AT_CTRL2_TF_SHIFT), PCF85063AT_CTRL2_TF_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_MinInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Minute Interrupt Enable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intEnable << PCF85063AT_CTRL2_MI_SHIFT ), PCF85063AT_CTRL2_MI_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

}

int32_t PCF85063AT_MinInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Minute Interrupt Disable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2,(uint8_t)(intDisable << PCF85063AT_CTRL2_MI_SHIFT ), PCF85063AT_CTRL2_MI_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_HalfMinInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Half Minute Interrupt Enable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intEnable << PCF85063AT_CTRL2_HMI_SHIFT ), PCF85063AT_CTRL2_HMI_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
}

int32_t PCF85063AT_HalfMinInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Half Minute Interrupt Disable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2,(uint8_t)(intDisable << PCF85063AT_CTRL2_HMI_SHIFT ), PCF85063AT_CTRL2_HMI_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}


int32_t PCF85063AT_Clear_AlarmInt(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Clear Alarm flag */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intClear << PCF85063AT_CTRL2_AF_SHIFT), PCF85063AT_CTRL2_AF_MASK,repeatedStart);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Check_AlarmInt(PCF85063AT_sensorhandle_t *pSensorHandle, IntState *pAlarmState)
{
	int32_t status;
	PCF85063AT_CTRL_2 Ctrl2_Reg;

	/*! Validate for the correct handle and Alarm status read variable.*/
	if ((pSensorHandle == NULL) || (pAlarmState == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! check Alarm flag */
	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, PCF85063AT_REG_SIZE_BYTE, (uint8_t *)&Ctrl2_Reg);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_READ;
	}

	*pAlarmState = Ctrl2_Reg.b.af ;

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_AlarmInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle, AlarmType alarmtype)
{
	int32_t status;

	/*! Validate for the correct handle and Interrupt status read variable.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Enable Alarm */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intEnable << PCF85063AT_CTRL2_AIE_SHIFT), PCF85063AT_CTRL2_AIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	switch(alarmtype)
	{
	case A_Seconds:
		/*! Enable/Disable Second Alarm */
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
				PCF85063AT_SECOND_ALARM, (uint8_t)( 0 << PCF85063AT_AEN_S_SHIFT), PCF85063AT_AEN_S_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
	case A_Minute:
		/*! Enable/Disable Minute Alarm */
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
				PCF85063AT_MINUTE_ALARM, (uint8_t)( 0 << PCF85063AT_AEN_M_SHIFT), PCF85063AT_AEN_M_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
	case A_Hour:
		/*! Enable/Disable Hour Alarm */
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
				PCF85063AT_HOUR_ALARM, (uint8_t)( 0 << PCF85063AT_AEN_H_SHIFT), PCF85063AT_AEN_H_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
	case A_Day:
		/*! Enable/Disable Day Alarm */
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
				PCF85063AT_DAY_ALARM, (uint8_t)( 0 << PCF85063AT_AEN_D_SHIFT), PCF85063AT_AEN_D_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
	case A_Weekday:
		/*! Enable/Disable WeekDay Alarm */
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
				PCF85063AT_WEEKDAY_ALARM, (uint8_t)( 0 << PCF85063AT_AEN_W_SHIFT), PCF85063AT_AEN_W_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
	}

}

int32_t PCF85063AT_AlarmInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle and Interrupt status read variable.*/
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Disable Alarm */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL2, (uint8_t)(intDisable << PCF85063AT_CTRL2_AIE_SHIFT), PCF85063AT_CTRL2_AIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_GetAlarmTime(PCF85063AT_sensorhandle_t *pSensorHandle, const registerreadlist_t *PCF85063ATalarmdata , PCF85063AT_alarmdata_t *alarmtime)
{
	int32_t status;
	Mode12h_24h mode12_24;

	/*! Validate for the correct handle and Alarm read variable.*/
	if ((pSensorHandle == NULL) || (alarmtime == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Get Alarm time.*/
	status = PCF85063AT_ReadData(pSensorHandle, PCF85063ATalarmdata, ( uint8_t *)alarmtime);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! after read convert BCD to Decimal */
	alarmtime->second = BcdToDecimal(alarmtime->second & PCF85063AT_SECONDS_ALARM_MASK) ;
	alarmtime->minutes = BcdToDecimal(alarmtime->minutes & PCF85063AT_MINUTES_ALARM_MASK) ;
	PCF85063AT_12h_24h_Mode_Get(pSensorHandle, &mode12_24);
	if(mode12_24 ==  mode24H)
	{
		alarmtime->hours = BcdToDecimal(alarmtime->hours & PCF85063AT_HOURS_ALARM_MASK_24H) ;
		alarmtime->ampm = h24;
	}
	else   /* Set AM/PM */
	{
		if( (alarmtime->hours >> 5) & 0x01 )
			alarmtime->ampm = PM;
		else
			alarmtime->ampm = AM;
		alarmtime->hours =  BcdToDecimal(alarmtime->hours & PCF85063AT_HOURS_ALARM_MASK_12H);
	}
	alarmtime->days = BcdToDecimal(alarmtime->days & PCF85063AT_DAYS_ALARM_MASK);
	alarmtime->weekdays = BcdToDecimal(alarmtime->weekdays & PCF85063AT_WEEKDAYS_ALARM_MASK);

	return SENSOR_ERROR_NONE;
}


int32_t PCF85063AT_SetAlarmTime(PCF85063AT_sensorhandle_t *pSensorHandle, PCF85063AT_alarmdata_t *alarmtime)
{
	int32_t status;


	/*! Validate for the correct handle and Alarm time read variable.*/
	if ((pSensorHandle == NULL) || (alarmtime == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Set Alarm Second.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_SECOND_ALARM, DecimaltoBcd(alarmtime->second & PCF85063AT_SECONDS_ALARM_MASK), PCF85063AT_SECONDS_ALARM_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Alarm Minute.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_MINUTE_ALARM, DecimaltoBcd(alarmtime->minutes & PCF85063AT_MINUTES_ALARM_MASK), PCF85063AT_MINUTES_ALARM_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Update AM/PM Bit.*/
	if(alarmtime->ampm == AM)
	{
		alarmtime->hours = DecimaltoBcd(alarmtime->hours & PCF85063AT_HOURS_ALARM_MASK_12H);
		alarmtime->hours = ((alarmtime->hours) & (~(PM << PCF85063AT_AM_PM_SHIFT)));
	}
	else if(alarmtime->ampm == PM)
	{
		alarmtime->hours = DecimaltoBcd(alarmtime->hours & PCF85063AT_HOURS_ALARM_MASK_12H);
		alarmtime->hours = (alarmtime->hours | (PM << PCF85063AT_AM_PM_SHIFT));
	}
	else
		alarmtime->hours = DecimaltoBcd(alarmtime->hours & PCF85063AT_HOURS_ALARM_MASK_24H);

	/*! Set Alarm Hours.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_HOUR_ALARM, alarmtime->hours , PCF85063AT_HOURS_ALARM_MASK_24H,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Alarm Day.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_DAY_ALARM, DecimaltoBcd(alarmtime->days & PCF85063AT_DAYS_ALARM_MASK) , PCF85063AT_DAYS_ALARM_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	/*! Set Alarm WeekDay.*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_WEEKDAY_ALARM, DecimaltoBcd(alarmtime->weekdays & PCF85063AT_WEEKDAYS_ALARM_MASK) , PCF85063AT_WEEKDAYS_ALARM_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_TimerInt_Enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer Interrupt Enable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE, (uint8_t)(intEnable << PCF85063AT_SECONDS_TIE_SHIFT ), PCF85063AT_SECONDS_TIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
}

int32_t PCF85063AT_TimerInt_Disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer Interrupt Disable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(uint8_t)(intDisable << PCF85063AT_SECONDS_TIE_SHIFT ), PCF85063AT_SECONDS_TIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Check_TI_TP(PCF85063AT_sensorhandle_t *pSensorHandle, TI_TP_State *pTI_TPState)
{
	int32_t status;
	PCF85063AT_TIMER_mode TimerMode_Reg;

	/*! Validate for the correct handle and Interrupt status read variable.*/
	if ((pSensorHandle == NULL) || (pTI_TPState == NULL))
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Check Timer Interrupt Mode */
	status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE, PCF85063AT_REG_SIZE_BYTE, (uint8_t *)&TimerMode_Reg);

	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_READ;
	}
	*pTI_TPState = TimerMode_Reg.b.ti_tp ;

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_TI_TP_Enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer Interrupt mode Enable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE, (uint8_t)( pulse << PCF85063AT_SECONDS_TI_TP_SHIFT ), PCF85063AT_SECONDS_TI_TP_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
}

int32_t PCF85063AT_TI_TP_Disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer Interrupt mode Disable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(uint8_t)(timer_flag << PCF85063AT_SECONDS_TI_TP_SHIFT ), PCF85063AT_SECONDS_TI_TP_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_TestFreeRAMByte(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*!Free RAM Byte */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_RAM_BYTE,0x3c,0x00,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Normal_OffsetMode(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Normal offset mode */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_OFFSET,(uint8_t)(normal_mode << PCF85063AT_OFFSET_MODE_SHIFT),PCF85063AT_OFFSET_MODE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Course_OffsetMode(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Course offset mode */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_OFFSET,(course_mode << PCF85063AT_OFFSET_MODE_SHIFT),PCF85063AT_OFFSET_MODE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_CI_enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Correction Interrupt enable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1,(intEnable << PCF85063AT_CTRL1_CIE_SHIFT),PCF85063AT_CTRL1_CIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_CI_disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Correction Interrupt Disable */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_CTRL1,(intDisable << PCF85063AT_CTRL1_CIE_SHIFT),PCF85063AT_CTRL1_CIE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_timer_enable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer enabled */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(intEnable << PCF85063AT_SECONDS_TE_SHIFT),PCF85063AT_SECONDS_TE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_timer_disable(PCF85063AT_sensorhandle_t *pSensorHandle)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Timer Disabled */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(intDisable << PCF85063AT_SECONDS_TE_SHIFT),PCF85063AT_SECONDS_TE_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_SetTimerClockFreq(PCF85063AT_sensorhandle_t *pSensorHandle, int32_t tcf)
{
	int32_t status;
	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*setting timer clock frequency*/
	switch (tcf)
	{
	case 1: /*4.096 kHz timer source clock*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(timer1 << PCF85063AT_SECONDS_TCF_SHIFT),PCF85063AT_SECONDS_TCF_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
		return SENSOR_ERROR_NONE;


	case 2: /*64 Hz timer source clock*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(timer2 << PCF85063AT_SECONDS_TCF_SHIFT),PCF85063AT_SECONDS_TCF_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
		return SENSOR_ERROR_NONE;

	case 3: /*1 Hz timer source clock*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(timer3 << PCF85063AT_SECONDS_TCF_SHIFT),PCF85063AT_SECONDS_TCF_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
		return SENSOR_ERROR_NONE;


	case 4: /*1⁄60 Hz timer source clock*/
		status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_MODE,(timer4 << PCF85063AT_SECONDS_TCF_SHIFT),PCF85063AT_SECONDS_TCF_MASK,repeatedStart);
		if (ARM_DRIVER_OK != status)
		{
			return SENSOR_ERROR_WRITE;
		}
		return SENSOR_ERROR_NONE;
	}

}

int32_t PCF85063AT_Countdown_timer_value(PCF85063AT_sensorhandle_t *pSensorHandle, int32_t CT_value)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! Set Countdown Timer value */
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_TIMER_VALUE,(CT_value << PCF85063AT_SECONDS_TS_SHIFT),PCF85063AT_SECONDS_TS_MASK,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}
	return SENSOR_ERROR_NONE;
}

int32_t PCF85063AT_Set_offset(PCF85063AT_sensorhandle_t *pSensorHandle, int8_t offset)
{
	int32_t status;

	/*! Validate for the correct handle */
	if (pSensorHandle == NULL)
	{
		return SENSOR_ERROR_INVALID_PARAM;
	}

	/*! Check whether sensor handle is initialized before triggering sensor reset.*/
	if (pSensorHandle->isInitialized != true)
	{
		return SENSOR_ERROR_INIT;
	}

	/*! To choose offset*/
	status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo,pSensorHandle->slaveAddress,
			PCF85063AT_OFFSET, offset, PCF85063AT_OFFSET_MASK ,repeatedStart);
	if (ARM_DRIVER_OK != status)
	{
		return SENSOR_ERROR_WRITE;
	}

	return SENSOR_ERROR_NONE;
}
