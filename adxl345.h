/*
 * adxl345.h
 *
 *  Created on: Feb 15, 2025
 *      Author: Oleksiy
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "main.h"
#include "app_utils.h"
#include <stdlib.h>

#define EMPTY_BYTE 0xFF

enum registers {
	DEVID, 					// Device ID
	THRESH_TAP = 0x1D,		// Tap threshold
	OFSX,					// X-axis offset
	OFSY,					// Y-axis offset
	OFSZ,					// Z-axis offset
	DUR,					// Tap duration
	Latent,					// Tap latency
	Window,					// Tap window
	THRESH_ACT,				// Activity threshold
	THRESH_INACT,			// Inactivity threshold
	TIME_INACT,				// Inactivity time
	ACT_INACT_CTL,			// Axis enable control for activity and inactivity detection
	THRESH_FF,				// Free-fall threshold
	TIME_FF,				// Free-fall time
	TAP_AXES,				// Axis control for single tap/double tap
	ACT_TAP_STATUS,			// Source of single tap/double tap
	BW_RATE,				// Data rate and power mode control
	POWER_CTL,				// Power-saving features control
	INT_ENABLE,				// Interrupt enable control
	INT_MAP,				// Interrupt mapping control
	INT_SOURCE,				// Source of interrupts
	DATA_FORMAT,			// Data format control
	DATAX0,					// X-Axis Data 0
	DATAX1,					// X-Axis Data 1
	DATAY0,					// Y-Axis Data 0
	DATAY1,					// Y-Axis Data 1
	DATAZ0,					// Z-Axis Data 0
	DATAZ1,					// Z-Axis Data 1
	FIFO_CTL,				// FIFO control
	FIFO_STATUS				// FIFO status
};

enum read_write_mode {
	READ_SINGLE_DATA = 1<<7,
	READ_MULTIPLE_DATA = (1<<6) | (1<<7),
	WRITE_SINGLE_DATA = 0,
	WRITE_MULTIPLE_DATA = 1<<6
};

enum modes{
	MAIN,
	CS
};

typedef struct {
	uint8_t LowByte;
	uint8_t HighByte;
} ADXL345_Data;

typedef struct {
	ADXL345_Data x;
	ADXL345_Data y;
	ADXL345_Data z;
} ADXL345_AXIS;

typedef struct {
	ADXL345_AXIS data;
	uint8_t rxDataBuffer[10];
	GPIO_TypeDef * CS_port;
	uint16_t CS_Pin;
	SPI_HandleTypeDef SPI_Interface;
} ADXL345;

typedef struct {
	GPIO_TypeDef * MISO_port;
	uint16_t MISO_Pin;
	GPIO_TypeDef * MOSI_port;
	uint16_t MOSI_Pin;
	GPIO_TypeDef * SCK_port;
	uint16_t SCK_Pin;
	GPIO_TypeDef * CS_port;
	uint16_t CS_Pin;
	SPI_TypeDef * interface_instance;
} ADXL345_Init_struct;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} ADXL345_Values;




void __ADXL345_Init_SPI_GPIO(GPIO_TypeDef  *GPIOx, uint16_t pin, SPI_TypeDef * spi_instance, uint8_t mode);
uint8_t* __ADXL345_Read_Registers(ADXL345 * device, uint8_t registerAddress, uint8_t size);
ADXL345 ADXL345_SPI_init(ADXL345_Init_struct * initData);
uint8_t ADXL345_ReadRegister(ADXL345 * device, uint8_t registerAddress);
void ADXL345_SetRegister(ADXL345 * device, uint8_t registerAddress, uint8_t byte);
void ADXL345_Start_measurement(ADXL345 * device);
ADXL345_Data ADXL345_Read_X_axis(ADXL345 * device);
ADXL345_Data ADXL345_Read_Y_axis(ADXL345 * device);
ADXL345_Data ADXL345_Read_Z_axis(ADXL345 * device);
int16_t ADXL345_Convert_Axis(ADXL345_Data axis);
ADXL345_AXIS ADXL345_Read_All_axes(ADXL345 * device);
#endif /* INC_ADXL345_H_ */
