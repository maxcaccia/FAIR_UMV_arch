/*
 * LogicalLinearRateData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "LogicalLinearRateData.h"

LogicalLinearRateData::LogicalLinearRateData(string name):BodyLinearRateData(name) {
	mPacketCode=LOGICAL_LINEAR_RATE_CODE;
	reset();
}

LogicalLinearRateData::LogicalLinearRateData(void):BodyLinearRateData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_LINEAR_RATE_CODE;
	reset();
}

void LogicalLinearRateData::reset(void) {
	BodyLinearRateData::reset();
	mPose.reset();
}

LogicalLinearRateData & LogicalLinearRateData::operator = (const LogicalLinearRateData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	BodyLinearRateData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void LogicalLinearRateData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	BodyLinearRateData::sprint_csv_log_name(strTS);
	sprintf(str,"%s"
			"%s_dx,"					// CF
			"%s_dy,"					// CF
			"%s_dz,"					// CF
			"%s_droll,"				// CF
			"%s_dpitch,"			// CF
			"%s_dyaw_,",				// CF
			strTS
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			,mName.c_str()
			);
	strcpy(s,str);
}

void LogicalLinearRateData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	BodyLinearRateData::sprint_csv_long_name(strTS);
	sprintf(str,"%s"
			"device_x_displacement,"								// CF
			"device_y_displacement,"								// CF
			"device_z_displacement,"									// CF
			"device_roll_displacement,"							// CF
			"device_pitch_displacement,"							// CF
			"device_yaw_displacement,"							// CF
			,strTS);
	strcpy(s,str);
}

int LogicalLinearRateData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	BodyLinearRateData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// body linear velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void LogicalLinearRateData::sprint_csv(char *s) {
	char str[2048],str1[256];

	BodyLinearRateData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// body linear velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void LogicalLinearRateData::sread(char *s) {
	char packetCode[64];
	char name[64];
	uint8_t valid;
	sscanf(s,// packet_code
			"%s "
			// name
			"%s "
			// time stamp
			"%lf "
			// valid flag
			" %" SCNu8
			// velocity
			"%lf %lf %lf "
			// velocity stdev
			" %lf %lf %lf"
			// linear pose
			" %lf %lf %lf"
			// angular pose
			" %lf %lf %lf"
			"",
			// packet code
			packetCode,
			// name
			name,
			// time stamp
			&mTimeStamp,
			// valid flag
			&valid,
			// velocity
			&u,&v,&w,
			// velocity stdev
			&uStd,&vStd,&wStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalLinearRateData::assign(BodyLinearRateData bodyLinearRateData)  {
	// velocity
	u=bodyLinearRateData.u;
	v=bodyLinearRateData.v;
	w=bodyLinearRateData.w;
	// velocity stdev
	uStd=bodyLinearRateData.uStd;
	vStd=bodyLinearRateData.vStd;
	wStd=bodyLinearRateData.wStd;
}

void LogicalLinearRateData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}

