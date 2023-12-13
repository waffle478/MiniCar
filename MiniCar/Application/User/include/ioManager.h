/*
 * pwmManager.h
 *
 *  Created on: 2023. júl. 8.
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_IOMANAGER_H_
#define APPLICATION_USER_INCLUDE_IOMANAGER_H_

#include <MotorController.h>

void IO_StartPWM();
void setPWM(TIM_HandleTypeDef *timer,__IO uint32_t channel, int value);
void IO_ApplySpeedAndDir(void);
void IO_setMotorSpeedAndDir(Motor mot);

#endif /* APPLICATION_USER_INCLUDE_IOMANAGER_H_ */
