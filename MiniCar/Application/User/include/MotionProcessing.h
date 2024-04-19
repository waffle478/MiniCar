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

#define CALCULATE_MOTION(SCALE, DATA) (int16_t)((int32_t)((int32_t)SCALE * (int32_t)2000 * (int32_t)DATA) / (int32_t)65535);
#define CALCULATE_ROTATION(SCALE, DATA) (int16_t)((int32_t)((int32_t)SCALE * (int32_t)2 * (int32_t)DATA) / (int32_t)65535);
#define INTEGRATION_DELTA_TIME 20u // the number of measurements

/* Acceleration in mg-s or 0.1°/s */
typedef struct{
	int16_t X;
	int16_t Y;
	int16_t Z;
}Acceleration;

/* Speed in m/s */
typedef struct{
	int16_t X;
	int16_t Y;
	int16_t Z;
	uint8_t SpeedReady;
}Speed;

typedef struct{
	Acceleration AccelerationSum;
	uint8_t AccelerationCount;
	Acceleration Acceleration_highDef; /* FIXME Give me a better name */
	Speed Speed;
}Movement;

typedef struct{
	Movement Linear;
	Movement Agnular;
}Motion; /* FIXME Give me a better name */

void MP_MotionProcessing(void);
void MP_processAccelerometer(void);
void MP_processGyroscope(void);
void MP_calculateSpeed(Movement *accelerationMovement);


#endif /* APPLICATION_USER_INCLUDE_MOTIONPROCESSING_H_ */
