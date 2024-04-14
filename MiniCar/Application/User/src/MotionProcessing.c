/*
 * MotionProcessing.c
 *
 *  Created on: 2024. ápr. 4.
 *      Author: Csabi
 */

#include "MotionProcessing.h"
#include "math.h"

Motion CurrentMotion;

void MP_processAccelerometer(void)
{
	AxisRaw rawAccelerometerData = MPU_getAccelerometer();
	CurrentMotion.Linear.Acceleration_highDef.X = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.X_axis);
	CurrentMotion.Linear.Acceleration_highDef.Y = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.Y_axis);
	CurrentMotion.Linear.Acceleration_highDef.Z = CALCULATE_MOTION(rawAccelerometerData.FullScaleSelected, rawAccelerometerData.Z_axis);
}

void MP_processGyroscope(void)
{
	AxisRaw rawGyroscopeData = MPU_getGyroscope();
	CurrentMotion.Linear.Acceleration_highDef.X = CALCULATE_MOTION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.X_axis);
	CurrentMotion.Linear.Acceleration_highDef.Y = CALCULATE_MOTION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.Y_axis);
	CurrentMotion.Linear.Acceleration_highDef.Z = CALCULATE_MOTION(rawGyroscopeData.FullScaleSelected, rawGyroscopeData.Z_axis);

	/* 4 lépéses rungekutta? */
}
