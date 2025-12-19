//
//  DevicePose.h
//  Device pose: handles kinematics of a device mounted on a rigid-body
//
//  Created by Massimo Caccia on 29/01/24.
//

#ifndef DEVICEPOSE_H
#define DEVICEPOSE_H

#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "nVector.h"
//#include "GnssPositionData.h"

struct DevicePose {
	// device offset in the vehicle-fixed reference frame
	double x; // [m]
	double y; // [m]
	double z; // [m]
	// device yaw-pitch-roll rotations in the vehicle-fixed reference frame
    double psi; // [deg]
	double theta; // [deg]
	double phi; // [deg]

	void reset(void);
	DevicePose & operator = (const DevicePose &obj);
	bool fread(FILE *f);
	void print(void);
};

void compute_device_pose(
		LinearPosition &devPos,AngularPosition &devAttitude,LinearVelocity &devLinVel,AngularVelocity &devAngVel,
		DevicePose mPose,
		LinearPosition rbPos,AngularPosition rbAttitude,LinearVelocity rbLinVel,AngularVelocity rbAngVel);
//		Kinematics &mDev,DevicePose mPose,Kinematics mRB);


#endif /* DEVICEPOSE_H */
