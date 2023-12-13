/*
 * pwmManager.c
 *
 *  Created on: 2023. júl. 8.
 *      Author: Csabi
 */

#include <ioManager.h>

void IO_ApplySpeedAndDir(void)
{
	MotorController motorCtrl = MC_GetMotors();

	IO_setMotorSpeedAndDir(motorCtrl.MainMotor);
	IO_setMotorSpeedAndDir(motorCtrl.Steering);
}

void IO_setMotorSpeedAndDir(Motor mot)
{
	/*
	 * output LOW / BRAKE is achieved with setting one of the inputs (the direction) to 1 and the other to PWM.
	 *
	 * output IMPEDANCE state is achieved with setting one inputs (the direction) to PWM and the other to LOW.
	 */

	/* Normal operation */
	if (DECAY_HIGH_IMPEDANCE == mot.Decay)
	{
		switch (mot.Direction) {
			case DIRECTION_FORWARD:
				CCRx_OF(mot, CCR1) = mot.Speed;
				CCRx_OF(mot, CCR2) = 0;
				break;
			case DIRECTION_BACKWARD:
				CCRx_OF(mot, CCR1) = 0;
				CCRx_OF(mot, CCR2) = mot.Speed;
				break;
			case DIRECTION_NONE:
				CCRx_OF(mot, CCR1) = 0;
				CCRx_OF(mot, CCR2) = 0;
				break;
			default:
				/* DO NOTHING */
				break;
		}
	}
	/* Motor braking operation */
	else if (DECAY_SHORT_CIRCUIT == mot.Decay) {
		switch (mot.Direction) {
			case DIRECTION_FORWARD:
				CCRx_OF(mot, CCR1) = 100;
				CCRx_OF(mot, CCR2) = mot.Speed;
				break;
			case DIRECTION_BACKWARD:
				CCRx_OF(mot, CCR1) = mot.Speed;
				CCRx_OF(mot, CCR2) = 100;
				break;
			case DIRECTION_NONE:
				CCRx_OF(mot, CCR1) = 100;
				CCRx_OF(mot, CCR2) = 100;
				break;
			default:
				/* DO NOTHING */
				break;
		}
	}
}

void setPWM(TIM_HandleTypeDef *timer,__IO uint32_t channel, int value)
{

}

void IO_StartPWM()
{
	MotorController motorCtrl = MC_GetMotors();

	HAL_TIM_PWM_Start(motorCtrl.MainMotor.timerHandler, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(motorCtrl.MainMotor.timerHandler, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(motorCtrl.Steering.timerHandler, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(motorCtrl.Steering.timerHandler, TIM_CHANNEL_2);
}
