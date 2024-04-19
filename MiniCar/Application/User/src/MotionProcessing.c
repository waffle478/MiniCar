/*
 * MotionProcessing.c
 *
 *  Created on: 2024. ápr. 4.
 *      Author: Csabi
 */

#include "MotionProcessing.h"
#include "math.h"

Motion CurrentMotion;

void MP_MotionProcessing(void)
{
	if (MPU_getReadyGyroscope()) {
		MP_processGyroscope();
		MP_calculateSpeed(&CurrentMotion.Agnular); // it is linear position tho
	}
	if (MPU_getReadyAccelerometer()) {
		MP_processAccelerometer();
		MP_calculateSpeed(&CurrentMotion.Linear);
	}

}

void MP_processAccelerometer(void)
{
	AxisRaw rawAccelerometerData = MPU_getAccelerometer();
	/* Calculate linear acceleration */
	CurrentMotion.Linear.Acceleration_highDef.X = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.X_axis);
	CurrentMotion.Linear.Acceleration_highDef.Y = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.Y_axis);
	CurrentMotion.Linear.Acceleration_highDef.Z = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.Z_axis);
}

/* TODO: use these functions in a main motion processing func and give current motion as a parameter */
void MP_processGyroscope(void)
{
	AxisRaw rawGyroscopeData = MPU_getGyroscope();
	/* Calculate angular acceleration */
	CurrentMotion.Agnular.Acceleration_highDef.X = CALCULATE_ROTATION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.X_axis);
	CurrentMotion.Agnular.Acceleration_highDef.Y = CALCULATE_ROTATION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.Y_axis);
	CurrentMotion.Agnular.Acceleration_highDef.Y += 4;  // correction
	CurrentMotion.Agnular.Acceleration_highDef.Z = CALCULATE_ROTATION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.Z_axis);

	/* 4 lépéses rungekutta? */
}

void MP_calculateSpeed(Movement *accelerationMovement)
{
	(*accelerationMovement).Speed.X += (*accelerationMovement).Acceleration_highDef.X;
	(*accelerationMovement).Speed.Y += (*accelerationMovement).Acceleration_highDef.Y;
	(*accelerationMovement).Speed.Z += (*accelerationMovement).Acceleration_highDef.Z;
	//(*accelerationMovement).AccelerationCount++;

	/*if ((*accelerationMovement).AccelerationCount >= INTEGRATION_DELTA_TIME)
	{
		(*accelerationMovement).Speed.X = (*accelerationMovement).AccelerationSum.X / (uint16_t)(*accelerationMovement).AccelerationCount;
		(*accelerationMovement).Speed.Y = (*accelerationMovement).AccelerationSum.Y / (uint16_t)(*accelerationMovement).AccelerationCount;
		(*accelerationMovement).Speed.Z = (*accelerationMovement).AccelerationSum.Z / (uint16_t)(*accelerationMovement).AccelerationCount;
		(*accelerationMovement).AccelerationCount = 0;

		(*accelerationMovement).AccelerationSum.X = 0;
		(*accelerationMovement).AccelerationSum.Y = 0;
		(*accelerationMovement).AccelerationSum.Z = 0;
	}*/
}
