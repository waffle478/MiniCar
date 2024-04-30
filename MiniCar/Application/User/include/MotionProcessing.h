/*
 * MotionProcessing.h
 *
 *  Created on: 2024. ápr. 4.
 *      Author: Csabi
 */

#ifndef APPLICATION_USER_INCLUDE_MOTIONPROCESSING_H_
#define APPLICATION_USER_INCLUDE_MOTIONPROCESSING_H_

#include "MPU_Handler.h"
#include "main.h"
/* FIXME: This macro only needs one parameter */
#define CALCULATE_MOTION(SCALE, DATA) (float)((int32_t)((int32_t)SCALE * (int32_t)2000 * (int32_t)DATA) / (int32_t)65535)
#define CALCULATE_ROTATION(SCALE, DATA) (float)((float)((float)SCALE * (float)2 * (float)DATA) / (float)65535)
#define INTEGRATION_DELTA_TIME 20u // the number of measurements

/* Acceleration in mg-s or 0.1°/s */
/* FIXME: °/s is SPEED shees.. */
typedef struct{
	float X;
	float Y;
	float Z;
}Acceleration;

/* Speed in m/s */
typedef struct{
	float X;
	float Y;
	float Z;
	uint8_t SpeedReady;
}Speed;

typedef struct{
	Acceleration AccelerationSum;
	Acceleration Acceleration_highDef; /* FIXME Give me a better name */
	Speed Speed;
}Movement;

/* TODO: The gyro data is only contains speed. It only needs speed and position */
typedef struct{
	Movement Linear;
	Movement Agnular;
}Motion; /* FIXME Give me a better name */

void MP_MotionProcessing(void);
void MP_processAccelerometer(void);
void MP_processGyroscope(void);
void MP_calculateSpeed(Movement *accelerationMovement);


#endif /* APPLICATION_USER_INCLUDE_MOTIONPROCESSING_H_ */
