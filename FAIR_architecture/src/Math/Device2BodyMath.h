/*
 * Device2BodyMath.h
 *
 *  Created on: Jan 8, 2025
 *      Author: mc
 */

#ifndef DEVICE2BODYMATH_H_
#define DEVICE2BODYMATH_H_

#include <math.h>

#include "DevicePose.h"
//#include "LinearPosition.h"
#include "AngularPosition.h"
//#include "LinearVelocity.h"
#include "AngularVelocity.h"
//#include "nVector.h"
#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "AhrsData.h"
#include "DvlData.h"

//
// compute rigid-body center of mass position given GNSS position measurement (Earth-fixed NED frame)
//
void compute_GNSS2RB_position(GnssPositionData &rbPos,
							GnssPositionData devPos,
							DevicePose mPose,AngularPosition rbAttitude);

//
// compute rigid-body attitude given AHRS measurement
//
void compute_AHRS2RB_attitude(AhrsData &rbAttitude,
								AhrsData devAttitude,
								DevicePose mPose);

//
// compute rigid-body center of mass linear velocity given GNSS speed measurement (Earth-fixed NED frame)
//
void compute_GNSS2RB_speed(GnssVelocityData &rbVel,
						GnssVelocityData devVel,
						DevicePose mPose,AngularPosition rbAttitude,AngularVelocity rbAngVel);

//
// compute rigid-body center of mass linear velocity given DVL speed measurement (body-fixed frame)
//
void compute_DVL2RB_speed(DvlData &rbVel,
						DvlData devVel,
						DevicePose mPose,AngularVelocity rbAngVel);


#endif /* DEVICE2BODYMATH_H_ */
