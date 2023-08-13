/*
 * pwmManager.c
 *
 *  Created on: 2023. júl. 8.
 *      Author: Csabi
 */

#include <ioManager.h>

void setPWM(TIM_HandleTypeDef *timer,int channel, int value){
	timer->Instance->CCR2 = value;
}
