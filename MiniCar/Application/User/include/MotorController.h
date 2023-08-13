/*
 * MotorController.h
 *
 *  Created on: 2023. júl. 9.
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_MOTORCONTROLLER_H_
#define APPLICATION_USER_INCLUDE_MOTORCONTROLLER_H_

#include <Statuses.h>

#define FAST_DECAY 			0u	//Normal operation. Motor does not brake
#define SLOW_DECAY 			1u	//Motor moves according to pwm but it will brake

#define BRAKE_NONE			0u
#define BRAKE_ACTIVE		1u

#define DIRECTION_NONE		0u
#define DIRECTION_FORWARD	1u
#define DIRECTION_BACKWARD	2u

#define MOTORSPEED_ZERO		0u
#define MOTORSPEED_MAX		100u

typedef unsigned char MotorDirection_Type;
typedef unsigned char MotorSleep_Type;
typedef unsigned char MotorDecay_Type;
typedef unsigned char MotorBrake_Type;

typedef unsigned int MotorCurrent_Type;
typedef unsigned int MotorSpeedValue_Type;

typedef struct{
	MotorSpeedValue_Type	Speed;
	MotorDirection_Type		Direction;
	MotorDecay_Type			Decay;
	MotorBrake_Type			BrakeStatus; 	//This for slowing down the motor. Disables driving but not resetting the values.
	MotorSleep_Type			Sleep;			//Makes the motor's IC go to sleep mode.
#ifdef ENABLE_CURRENT_SENSE
	MotorCurrent_Type		Current;
#endif
}Motor;

typedef enum{
	Steering,
	MainMotor
}SelectedMotor;

/* TODO: Give a better name for the Motors struct and the MainMotor
 *
 * */

typedef struct{
	Motor Steering;
	Motor MainMotor;
}MotorController;

void MC_GetMotors();

int MC_SetSpeedAndDirection(SelectedMotor motor, unsigned char speed, unsigned char direction);
int MC_SetDecay(SelectedMotor motor, unsigned char decay);
int MC_SetSleep(SelectedMotor motor, unsigned char sleep);

extern MotorController Motors;

#endif /* APPLICATION_USER_INCLUDE_MOTORCONTROLLER_H_ */
