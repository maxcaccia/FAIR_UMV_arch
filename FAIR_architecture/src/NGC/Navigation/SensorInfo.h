//
//  SensorInfo.h
//  Sensor info: handles sensor name and kinematics of a device mounted on a rigid-body
//
//  Created by Massimo Caccia on 31/07/24.
//

#ifndef SENSORINFO_H
#define SENSORINFO_H

#include "DevicePose.h"
#include <vector>

struct MeasurementUncertainty{
	// position
	double latitude_stdev; // [m]
	double longitude_stdev; // [m]
	// attitude
	double roll_stdev; // [deg]
	double pitch_stdev; // [deg]
	double yaw_stdev; // [deg]
	// speed
	double speed_stdev; // [m]
	double course_std; // [deg]
	// linear rate
	double surge_rate_stdev; // [m/s]
	double sway_rate_stdev; // [m/s]
	double heave_rate_stdev; // [m/s]
	// angular rate
	double roll_rate_stdev; // [deg/s]
	double pitch_rate_stdev; // [deg/s]
	double yaw_rate_stdev; // [deg/s]

	MeasurementUncertainty & operator = (const MeasurementUncertainty &obj);
	bool fread(FILE *f);
	void reset(void);
	void print(void);

	bool get_value(double &value,string s);
};

struct SensorInfo {
	string mName;
	DevicePose mPose;
	vector<string> mModelName;
	vector<MeasurementUncertainty> mStd;

	void reset(void);
	SensorInfo & operator = (const SensorInfo &obj);
	bool fread(FILE *f);
	bool fread_pose(FILE *f);
	bool fread_models(FILE *f,string configDir);

	bool get_value(double &value,string s);

	void print(void); // { printf("=== sensor %s\n\tx %.3lf\n\ty %.3lf\n\tz %.3lf\n\tphi %.3lf\n\ttheta %.3lf\n\tpsi %.3lf\n",
//			mName.c_str(),mPose.x,mPose.y,mPose.z,mPose.phi,mPose.theta,mPose.psi);fflush(stdout);
//	}
};


#endif /* SENSORINFO_H */
