/*
 * app_utils.h
 *
 *  Created on: Feb 15, 2025
 *      Author: Oleksiy
 */

#ifndef INC_APP_UTILS_H_
#define INC_APP_UTILS_H_
#include "main.h"



enum return_check_result {
	FALSE,
	TRUE
};

void clockTheBus(void* instance);

uint8_t getAlter(void* instance);
uint8_t ifSPI(void* instance);
uint8_t ifGPIO(void* instance);

#endif /* INC_APP_UTILS_H_ */
