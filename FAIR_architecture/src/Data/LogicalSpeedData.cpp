/*
 * LogicalSpeedData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "LogicalSpeedData.h"

LogicalSpeedData::LogicalSpeedData(string name):BodySpeedData(name) {
	mPacketCode=LOGICAL_SPEED_CODE;
	reset();
}

LogicalSpeedData::LogicalSpeedData(void):BodySpeedData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_SPEED_CODE;
	reset();
}

void LogicalSpeedData::reset(void) {
	BodySpeedData::reset();
	mPose.reset();
}

LogicalSpeedData & LogicalSpeedData::operator = (const LogicalSpeedData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	BodySpeedData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void LogicalSpeedData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	BodySpeedData::sprint_csv_log_name(strTS);
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

void LogicalSpeedData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	BodySpeedData::sprint_csv_long_name(strTS);
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

int LogicalSpeedData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	BodySpeedData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// logical GNSS velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void LogicalSpeedData::sprint_csv(char *s) {
	char str[2048],str1[256];

	BodySpeedData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// logical GNSS velocity data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void LogicalSpeedData::sread(char *s) {
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
			// speed, track
			" %lf %lf"
			// speed,track stdev
			" %lf %lf"
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
			// speed, track
			&speed,&track,
			// speed,track stdev
			&speedStd,&trackStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalSpeedData::assign(BodySpeedData bodySpeedData)  {
	// speed, track
	speed=bodySpeedData.speed;
	track=bodySpeedData.track;
	// speed,track stdev
	speedStd=bodySpeedData.speedStd;
	trackStd=bodySpeedData.trackStd;
}

void LogicalSpeedData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}




