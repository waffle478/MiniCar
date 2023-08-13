
/*
 * MotorController.c
 *
 *  Created on: 2023. júl. 9.
 *      Author: Csabi
 */

#include <MotorController.h>

MotorController Motors;

void MC_GetMotors(){
	//return Motors;

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
