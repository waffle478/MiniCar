
/*
 * MotorController.c
 *
 *  Created on: 2023. júl. 9.
 *      Author: Csabi
 */

#include <MotorController.h>

MotorController Motors;

MotorController MC_GetMotors(){
	return Motors;
}

void MC_Init(TIM_HandleTypeDef *mainMotTimer, TIM_HandleTypeDef *steeringMotTimer)
{
	Motors.MainMotor.BrakeStatus 	= BRAKE_NONE;
	Motors.MainMotor.Decay 			= DECAY_HIGH_IMPEDANCE;
	Motors.MainMotor.Direction 		= DIRECTION_NONE;
	Motors.MainMotor.Sleep 			= 0;
	Motors.MainMotor.Speed 			= MOTORSPEED_ZERO;
	Motors.MainMotor.timerHandler 	= mainMotTimer;

	Motors.Steering.BrakeStatus 	= BRAKE_NONE;
	Motors.Steering.Decay 			= DECAY_HIGH_IMPEDANCE;
	Motors.Steering.Direction 		= DIRECTION_NONE;
	Motors.Steering.Sleep 			= 0;
	Motors.Steering.Speed 			= MOTORSPEED_ZERO;
	Motors.Steering.timerHandler 	= steeringMotTimer;
}

int MC_SetSpeedAndDirection(SelectedMotor motor, unsigned char speed, unsigned char direction){
	switch (motor) {
		case Steering:
			Motors.Steering.Speed		= speed;
			Motors.Steering.Direction	= direction;
			break;
		case MainMotor:
			Motors.MainMotor.Speed		= speed;
			Motors.MainMotor.Direction	= direction;
			break;
		default:
			/* Should not happen */
			return NOT_OK;
	}
	return OK;
}

int MC_SetDecay(SelectedMotor motor, unsigned char decay){
	switch (motor) {
		case Steering:
			Motors.Steering.Decay	= decay;
			break;
		case MainMotor:
			Motors.MainMotor.Decay	= decay;
			break;
		default:
			/* Should not happen */
			return NOT_OK;
	}
	return OK;
}

int MC_SetSleep(SelectedMotor motor, unsigned char sleep){
	switch (motor) {
		case Steering:
			Motors.Steering.Sleep	= sleep;
			break;
		case MainMotor:
			Motors.MainMotor.Sleep	= sleep;
			break;
		default:
			/* Should not happen */
			return NOT_OK;
	}
	return OK;
}

