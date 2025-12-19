/*
 * LogicalPositionData.cpp
 *
 *  Created on: 14 January 2025
 *      Author: massimo
 */
 
#include "LogicalPositionData.h"

LogicalPositionData::LogicalPositionData(string name):BodyPositionData(name) {
	mPacketCode=LOGICAL_POSITION_CODE;
	reset();
}

LogicalPositionData::LogicalPositionData(void):BodyPositionData() {
	// TODO Auto-generated constructor stub
	mPacketCode=LOGICAL_POSITION_CODE;
	reset();
}

void LogicalPositionData::reset(void) {
	BodyPositionData::reset();
	mPose.reset();
}

LogicalPositionData & LogicalPositionData::operator = (const LogicalPositionData &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;
	// do the assignment operation
	BodyPositionData::operator =(obj);
	// device pose
	mPose=obj.mPose;
	// return a reference to myself
	return *this;
}


void LogicalPositionData::sprint_csv_log_name(char *s) {
	char strTS[256],str[2048];
	BodyPositionData::sprint_csv_log_name(strTS);
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

void LogicalPositionData::sprint_csv_long_name(char *s) {
	char strTS[256],str[2048];
	BodyPositionData::sprint_csv_long_name(strTS);
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

int LogicalPositionData::sprint(char *s) {
	char str[1024],str1[256];
	int n;

	BodyPositionData::sprint(str1);
	n=sprintf(str,"%s"
			// linear pose
			"%.2lf %.2lf %.2lf "
			// angular pose
			"%.2lf %.2lf %.2lf "
			" ",
			// logical ahrs data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
	return n;
}

void LogicalPositionData::sprint_csv(char *s) {
	char str[2048],str1[256];

	BodyPositionData::sprint_csv(str1);
	sprintf(str,"%s"
			// linear pose
			"%.2lf,%.2lf,%.2lf,"
			// angular pose
			"%.2lf,%.2lf,%.2lf,",
			// logical GNSS position data
			str1,
			// linear pose
			mPose.x,mPose.y,mPose.z,
			// angular pose
			mPose.phi,mPose.theta,mPose.psi
			);
	strcpy(s,str);
}


void LogicalPositionData::sread(char *s) {
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
			// latitude, longitude
			" %lf %lf"
			// latitude,longitude stdev
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
			// latitude, longitude
			&lat,&lon,
			// latitude,longitude stdev
			&latStd,&lonStd,
			// linear pose
			&mPose.x,&mPose.y,&mPose.z,
			// angular pose
			&mPose.phi,&mPose.theta,&mPose.psi
			);			
	mPacketCode=packetCode;
	mName=name;
	mValid=(bool)valid;
}

void LogicalPositionData::assign(BodyPositionData bodyPositionData)  {
	// latitude, longitude
	lat=bodyPositionData.lat; // [deg.dddddddd] (GGA)
	lon=bodyPositionData.lon; // [deg.dddddddd] (GGA)
	// latitude,longitude stdev
	latStd=bodyPositionData.latStd;
	lonStd=bodyPositionData.lonStd;
}

void LogicalPositionData::assign(DevicePose devicePose) {
	// device pose
	mPose=devicePose;
}



//void LogicalPositionData::compute_rigid_body_position_planar(BodyPositionData &rb,double psi) {
//	//
//	// compute rigid body center of mass pose given device position measurement and pose in <B>
//	//
//	//
//	// psi : rigid body heading [deg]
//	//
//	double psiRad=psi*M_PI/180.;
//	double dx=-(mPose.x*cos(psiRad)-mPose.y*sin(psiRad));
//	double dy=-(mPose.x*sin(psiRad)+mPose.y*cos(psiRad));
//	// latitude, longitude
//	lat_lon_plus_x_y(rb.lat,rb.lon,lat,lon,dx,dy);
//	// latitude, longitude
//	rb.latStd = latStd;
//	rb.lonStd = lonStd;
//}



