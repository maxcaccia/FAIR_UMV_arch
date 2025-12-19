/*
 * LogicalAngularRateData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "LogicalAngularRateData.h"

LogicalAngularRateData::LogicalAngularRateData(string name):BodyAngularRateData(name) {
	mPacketCode=LOGICAL_ANGULAR_RATE_CODE;
	reset();
}

LogicalAngularRateData::LogicalAngularRateData(void):BodyAngularRateData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_ANGULAR_RATE_CODE;
	reset();
}

void LogicalAngularRateData::reset(void) {
	BodyAngularRateData::reset();
	mPose.reset();
}

LogicalAngularRateData & LogicalAngularRateData::operator = (const LogicalAngularRateData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	BodyAngularRateData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void LogicalAngularRateData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	BodyAngularRateData::sprint_csv_log_name(strTS);
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

void LogicalAngularRateData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	BodyAngularRateData::sprint_csv_long_name(strTS);
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

int LogicalAngularRateData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	BodyAngularRateData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// body angular rate data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void LogicalAngularRateData::sprint_csv(char *s) {
	char str[2048],str1[256];

	BodyAngularRateData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// body angular rate data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void LogicalAngularRateData::sread(char *s) {
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
			// angular velocity
			" %lf %lf %lf"
			// angular velocity stdev
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
			// angular velocity
			&p,&q,&r,
			// angular velocity stdev
			&pStd,&qStd,&rStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalAngularRateData::assign(BodyAngularRateData bodyAngularRateData)  {
	// angular velocity
	p=bodyAngularRateData.p;
	q=bodyAngularRateData.q;
	r=bodyAngularRateData.r;
	// angular velocity stdev
	pStd=bodyAngularRateData.pStd;
	qStd=bodyAngularRateData.qStd;
	rStd=bodyAngularRateData.rStd;
}

void LogicalAngularRateData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}




