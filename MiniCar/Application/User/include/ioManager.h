/*
 * pwmManager.h
 *
 *  Created on: 2023. júl. 8.
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_IOMANAGER_H_
#define APPLICATION_USER_INCLUDE_IOMANAGER_H_

#include <stm32l4xx_hal.h>
#include <MotorController.h>

void setPWM(TIM_HandleTypeDef *timer,int channel, int value);

#endif /* APPLICATION_USER_INCLUDE_IOMANAGER_H_ */
