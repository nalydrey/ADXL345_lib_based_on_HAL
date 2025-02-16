/*
 * adxl345.c
 *
 *  Created on: Feb 15, 2025
 *      Author: Oleksiy
 */
#include "adxl345.h"


ADXL345 ADXL345_SPI_init(ADXL345_Init_struct * initData){

	ADXL345 device;

	device.CS_port = initData->CS_port;
	device.CS_Pin = initData->CS_Pin;

	clockTheBus(initData->interface_instance);

	device.SPI_Interface.Instance = initData->interface_instance;

	device.SPI_Interface.Init.Mode = SPI_MODE_MASTER;
	device.SPI_Interface.Init.CLKPolarity = SPI_POLARITY_HIGH;
	device.SPI_Interface.Init.CLKPhase = SPI_PHASE_2EDGE;
	device.SPI_Interface.Init.DataSize = SPI_DATASIZE_8BIT;
	device.SPI_Interface.Init.Direction = SPI_DIRECTION_2LINES;
	device.SPI_Interface.Init.FirstBit = SPI_FIRSTBIT_MSB;
	device.SPI_Interface.Init.NSS = SPI_NSS_SOFT;
	device.SPI_Interface.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	device.SPI_Interface.Init.TIMode = SPI_TIMODE_DISABLE;

	HAL_SPI_Init(&device.SPI_Interface);

	__ADXL345_Init_SPI_GPIO(initData->MISO_port, initData->MISO_Pin, initData->interface_instance, MAIN);
	__ADXL345_Init_SPI_GPIO(initData->MOSI_port, initData->MOSI_Pin, initData->interface_instance, MAIN);
	__ADXL345_Init_SPI_GPIO(initData->SCK_port, initData->SCK_Pin, initData->interface_instance,MAIN);
	__ADXL345_Init_SPI_GPIO(initData->CS_port, initData->CS_Pin, initData->interface_instance, CS);

	HAL_GPIO_WritePin(initData->CS_port, initData->CS_Pin, GPIO_PIN_SET);

	return device;
}

uint8_t ADXL345_ReadRegister(ADXL345 * device, uint8_t registerAddress){

	uint8_t firstByte = READ_SINGLE_DATA | registerAddress;
	uint8_t message[2] = {firstByte, EMPTY_BYTE};
	uint8_t tempBuffer[2];

	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&device->SPI_Interface, message, tempBuffer, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_SET);

	return tempBuffer[1];
}

void ADXL345_SetRegister(ADXL345 * device, uint8_t registerAddress, uint8_t byte){

	uint8_t firstByte = WRITE_SINGLE_DATA | registerAddress;
	uint8_t message[2] = {firstByte, byte};

	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&device->SPI_Interface, message, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_SET);
}

ADXL345_Data ADXL345_Read_X_axis(ADXL345 * device){

	uint8_t* tempBuffer = __ADXL345_Read_Registers(device, DATAX0, 2);

	device->data.x.HighByte = tempBuffer[2];
	device->data.x.LowByte = tempBuffer[1];

	free(tempBuffer);

	return device->data.x;
}

ADXL345_Data ADXL345_Read_Y_axis(ADXL345 * device){

	uint8_t* tempBuffer = __ADXL345_Read_Registers(device, DATAY0, 2);

	device->data.y.HighByte = tempBuffer[2];
	device->data.y.LowByte = tempBuffer[1];

	free(tempBuffer);

	return device->data.y;
}

ADXL345_Data ADXL345_Read_Z_axis(ADXL345 * device){
	uint8_t* tempBuffer = __ADXL345_Read_Registers(device, DATAZ0, 2);

	device->data.z.HighByte = tempBuffer[2];
	device->data.z.LowByte = tempBuffer[1];

	free(tempBuffer);

	return device->data.z;
}

ADXL345_AXIS ADXL345_Read_All_axes(ADXL345 * device){
	uint8_t* tempBuffer = __ADXL345_Read_Registers(device, DATAX0, 6);

	device->data.x.HighByte = tempBuffer[2];
	device->data.x.LowByte = tempBuffer[1];
	device->data.y.HighByte = tempBuffer[4];
	device->data.y.LowByte = tempBuffer[3];
	device->data.z.HighByte = tempBuffer[6];
	device->data.z.LowByte = tempBuffer[5];

	free(tempBuffer);

	return device->data;
}

void ADXL345_Start_measurement(ADXL345 * device){

	uint8_t tempBuffer[1];

	tempBuffer[0] = ADXL345_ReadRegister(device, POWER_CTL);

	ADXL345_SetRegister(device, POWER_CTL, tempBuffer[0] | 0x08);
}

void __ADXL345_Init_SPI_GPIO(GPIO_TypeDef  *GPIOx, uint16_t pin, SPI_TypeDef * spi_instance, uint8_t mode){

	GPIO_InitTypeDef gpioInit;

	clockTheBus(GPIOx);

	gpioInit.Pin = pin;
	gpioInit.Mode = mode == CS ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	if(mode == MAIN){
		gpioInit.Alternate = getAlter(spi_instance);
	}

	HAL_GPIO_Init(GPIOx, &gpioInit);
}


int16_t ADXL345_Convert_Axis(ADXL345_Data axis){

	uint8_t tempBuffer[2] = {axis.HighByte, axis.LowByte};

	uint8_t isNegative = axis.HighByte>>7;

		if(isNegative){
			tempBuffer[0] = ~tempBuffer[0];
			tempBuffer[1] = ~tempBuffer[1];
		}

	int16_t value = ((tempBuffer[0] << 8) | tempBuffer[1]);

	return isNegative ? (-1)*value : value;
}

ADXL345_Values ADXL_345_Convert_Axes(ADXL345_AXIS axes){
	ADXL345_Values tempAxes;
	tempAxes.x = ADXL345_Convert_Axis(axes.x);
	tempAxes.y = ADXL345_Convert_Axis(axes.y);
	tempAxes.z = ADXL345_Convert_Axis(axes.z);

	return tempAxes;
}

ADXL345_Values ADXL_345_Convert_Me(ADXL345 * adxl){
	ADXL345_Values tempAxes;
	tempAxes.x = ADXL345_Convert_Axis(adxl->data.x);
	tempAxes.y = ADXL345_Convert_Axis(adxl->data.y);
	tempAxes.z = ADXL345_Convert_Axis(adxl->data.z);

	return tempAxes;
}


// !!! Important !!! When use this function don't forget to apply free();
uint8_t* __ADXL345_Read_Registers(ADXL345 * device, uint8_t registerAddress, uint8_t size){

	uint8_t localSize = size+1;

	uint8_t firstByte = READ_MULTIPLE_DATA | registerAddress;

	uint8_t* message = malloc(localSize * sizeof(uint8_t));

	message[0] = firstByte;
	for(uint8_t i = 1; i < localSize; i++){
		message[i] = EMPTY_BYTE;
	}

	uint8_t* tempBuffer = malloc(localSize * sizeof(uint8_t));


	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&device->SPI_Interface, message, tempBuffer, localSize, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(device->CS_port, device->CS_Pin, GPIO_PIN_SET);

	free(message);

	return tempBuffer;
}





