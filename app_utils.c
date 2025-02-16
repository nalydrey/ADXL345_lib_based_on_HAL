/*
 * app_utils.c
 *
 *  Created on: Feb 15, 2025
 *      Author: Oleksiy
 */
#include "app_utils.h"



void clockTheBus(void* instance){
	if(ifSPI(instance)){
		if(instance == SPI1){
			__HAL_RCC_SPI1_CLK_ENABLE();
			return;
		}
	}

	if(ifGPIO(instance)){
		if(instance == GPIOA){
			__HAL_RCC_GPIOA_CLK_ENABLE();
			return;
		}
		if(instance == GPIOC){
			__HAL_RCC_GPIOC_CLK_ENABLE();
			return;
		}
	}

}


uint8_t getAlter(void* instance){
	if(instance == SPI1) return GPIO_AF5_SPI1;
	if(instance == SPI2) return GPIO_AF5_SPI2;
	if(instance == SPI3) return GPIO_AF5_SPI3;
	if(instance == SPI4) return GPIO_AF5_SPI4;

}

uint8_t ifSPI(void* instance){
	SPI_TypeDef * list[] = {SPI1, SPI2};
	uint8_t length = sizeof(list)/sizeof(list[0]);
	for(uint8_t i = 0; i < length; i++){
		if(list[i] == instance) return TRUE;
	}
	return FALSE;
}

uint8_t ifGPIO(void* instance){
	GPIO_TypeDef * list[] = {GPIOA, GPIOB, GPIOC};
	uint8_t length = sizeof(list)/sizeof(list[0]);
	for(uint8_t i = 0; i < length; i++){
		if(list[i] == instance) return TRUE;
	}
	return FALSE;
}



